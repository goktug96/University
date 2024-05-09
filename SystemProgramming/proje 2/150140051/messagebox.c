#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>

#include <asm/switch_to.h>		/* cli(), *_flags */
#include <asm/uaccess.h>	/* copy_*_user */

#include <linux/sched.h>    /* get_current_user() */
#include <linux/syscalls.h>
#include <linux/kernel.h>

#include "pqueue_ioctl.h"

#define pqueue_MAJOR 0
#define pqueue_NR_DEVS 4
#define pqueue_INCLUDE_READ 0
#define pqueue_NODE_NUMBER 100
#define pqueue_UNREAD_LIMIT 10 /* maksimum okunmamış mesaj sayısı -> 11. mesaj gönderilmeye çalışıldığında "quota exceed" hatası */

int pqueue_major = PQUEUE_MAJOR;
int pqueue_minor = 0;
int pqueue_nr_devs = PQUEUE_NR_DEVS;
int pqueue_include_read = PQUEUE_INCLUDE_READ;
int pqueue_node_limit = PQUEUE_NODE_NUMBER;
int pqueue_unread_limit = PQUEUE_UNREAD_LIMIT;

module_param(pqueue_major, int, S_IRUGO);
module_param(pqueue_minor, int, S_IRUGO);
module_param(pqueue_nr_devs, int, S_IRUGO);
module_param(pqueue_include_read, int, S_IRUGO);
module_param(pqueue_node_limit, int, S_IRUGO);
module_param(pqueue_unread_limit, int, S_IRUGO);

MODULE_AUTHOR("Alessandro Rubini, Jonathan Corbet");
MODULE_LICENSE("Dual BSD/GPL");


typedef struct node
{
    char *text;
    struct node *next;
};

struct pqueue_dev
{
    struct node *head;
    int size;
    unsigned long maxSize;
    struct semaphore sem;
    struct cdev cdev;
};

struct pqueue_dev *pqueue_devices;

/*int atoi(char *array, int size)
{
    int i;
    int ret = 0;
    int digit = 1;
    for( i = size - 1; i >= 0; i--) {
        ret += (array[i] - '0')*digit;
        digit *= 10;
    }
    return ret;
}*/

/*void delete_messages(struct file *filp, username_t *username)
{
    struct pqueue_dev *dev = filp->private_data;
    struct node *msg;
    int i, j;

    for (i = 0; i < dev->node_amount; i++) {
        msg = dev->head;

        if (strcmp(msg->to, username->buf)) {
            kfree(msg->message);
            kfree(msg->to);
            kfree(msg->from);
            kfree(msg);

            for(j = i+1; j < dev->node_amount; j++) {
                dev->head[j-1] = dev->head[j];
            }
            dev->node_amount--;
            i--;
        }
    }
}*/

int pqueue_trim(struct pqueue_dev *dev)
{
    int i;

    if (dev->head) {
        node *temp = def->head;
        for (i = 0; i < dev->size; i++) {
            if (temp->next){
                kfree(temp->next->text);
                temp = temp->next;
            }else{
                break;
            }
        }
        kfree(dev->head->text);
        kfree(dev->head);
    }
    dev->head = NULL;
    dev->size = 0;
    dev->maxSize = pqueue_node_limit;
    return 0;
}


int pqueue_open(struct inode *inode, struct file *filp)
{
    struct pqueue_dev *dev;

    dev = container_of(inode->i_cdev, struct pqueue_dev, cdev);
    filp->private_data = dev;

    /* trim the device if open was write-only */
    if ((filp->f_flags & O_APPEND) == 0 && (filp->f_flags & O_ACCMODE) == O_WRONLY) {
        if (down_interruptible(&dev->sem))
            return -ERESTARTSYS;
        pqueue_trim(dev);
        up(&dev->sem);
    }
    return 0;
}


int pqueue_release(struct inode *inode, struct file *filp)
{
    return 0;
}


ssize_t pqueue_read(struct file *filp, char __user *buf, size_t count,
                   loff_t *f_pos)
{
    struct pqueue_dev *dev = filp->private_data;
    struct node *cur_node;
    int node_pos = (long)*f_pos;
    ssize_t retval = 0;
    int line = 0, buf_pos = 0;

    char *local_buffer;

    if (down_interruptible(&dev->sem))
        return -ERESTARTSYS;

    if (*f_pos >= dev->size)
        goto out;

    if (dev->head == NULL)
        goto out;

    local_buffer = kmalloc((count + 50) * sizeof(char), GFP_KERNEL);
    memset(local_buffer, 0, (count + 50) * sizeof(char));

    cur_node = dev->head;
    while(cur_node->next!=NULL) {
        line = 0;
        while (cur_node->text[line++])
            local_buffer[buf_pos++] = cur_node->text[line];
        local_buffer[buf_pos++] = ' ';
        if (buf_pos > count) {
            count = buf_pos;
        }
        cur_node = cur_node->next;
    }

    if (copy_to_user(buf, local_buffer, buf_pos)) {
        retval = -EFAULT;
        goto out;
    }

    count = buf_pos;
    *f_pos += count;
    retval = count;

  out:
    up(&dev->sem);
    return retval;
}

ssize_t pqueue_write(struct file *filp, const char __user *buf, size_t count,
                    loff_t *f_pos)
{
    struct pqueue_dev *dev = filp->private_data;
    ssize_t retval = -ENOMEM;
    int node_amount = dev->size;
    char *local_buffer;
    char *text;

    int pos = 0;

    if (down_interruptible(&dev->sem))
        return -ERESTARTSYS;

    if (node_amount >= dev->maxSize) {
        goto out;
    }

    local_buffer = kmalloc(count * sizeof(char), GFP_KERNEL);

    if (copy_from_user(local_buffer, buf, count)) {
        retval = -EFAULT;
        goto out;
    }

    text = kmalloc(100 * sizeof(char), GFP_KERNEL);

    while(pos++ < count)
        text[pos] = local_buffer[pos];

    text[pos] = 0;

    if (dev->head==NULL) {
        dev->head = kmalloc(sizeof(struct node *), GFP_KERNEL);
        if (!dev->head)
            goto out;
        memset(dev->head, 0, sizeof(struct node *));
    }

    node *cur_node = dev->head;
    while(cur_node->next!=NULL)
        cur_node = cur_node->next;

    cur_node->next =  kmalloc(sizeof(struct node *), GFP_KERNEL);
    cur_node->next->text = text;
    dev->size++;
    (*f_pos)++;

    retval = count;
    kfree(local_buffer);
  out:
    up(&dev->sem);
    return retval;
}

long pqueue_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

}


loff_t pqueue_llseek(struct file *filp, loff_t off, int whence)
{
    struct pqueue_dev *dev = filp->private_data;
    loff_t newpos;

    switch(whence) {
        case 0: /* SEEK_SET */
            newpos = off;
            break;

        case 1: /* SEEK_CUR */
            newpos = filp->f_pos + off;
            break;

        case 2: /* SEEK_END */
            newpos = dev->size + off;
            break;

        default: /* can't happen */
            return -EINVAL;
    }
    if (newpos < 0)
        return -EINVAL;
    filp->f_pos = newpos;
    return newpos;
}


struct file_operations pqueue_fops = {
    .owner =    THIS_MODULE,
    .llseek =   pqueue_llseek,
    .read =     pqueue_read,
    .write =    pqueue_write,
    .unlocked_ioctl =  pqueue_ioctl,
    .open =     pqueue_open,
    .release =  pqueue_release,
};


void pqueue_cleanup_module(void)
{
    int i;
    dev_t devno = MKDEV(pqueue_major, pqueue_minor);

    if (pqueue_devices) {
        for (i = 0; i < pqueue_nr_devs; i++) {
            pqueue_trim(pqueue_devices + i);
            cdev_del(&pqueue_devices[i].cdev);
        }
    kfree(pqueue_devices);
    }

    unregister_chrdev_region(devno, pqueue_nr_devs);
}


int pqueue_init_module(void)
{
    int result, i;
    int err;
    dev_t devno = 0;
    struct pqueue_dev *dev;

    if (pqueue_major) {
        devno = MKDEV(pqueue_major, pqueue_minor);
        result = register_chrdev_region(devno, pqueue_nr_devs, "pqueue");
    } else {
        result = alloc_chrdev_region(&devno, pqueue_minor, pqueue_nr_devs,
                                     "pqueue");
        pqueue_major = MAJOR(devno);
    }
    if (result < 0) {
        printk(KERN_WARNING "pqueue: can't get major %d\n", pqueue_major);
        return result;
    }

    pqueue_devices = kmalloc(pqueue_nr_devs * sizeof(struct pqueue_dev), GFP_KERNEL);
    if (!pqueue_devices) {
        result = -ENOMEM;
        goto fail;
    }
    memset(pqueue_devices, 0, pqueue_nr_devs * sizeof(struct pqueue_dev));

    /* Initialize each device. */
    for (i = 0; i < pqueue_nr_devs; i++) {
        dev = &pqueue_devices[i];
        dev->maxSize = pqueue_node_limit;
        dev->size = 0;
        dev->head = NULL;
        dev->head->next = NULL;
        sema_init(&dev->sem,1);
        devno = MKDEV(pqueue_major, pqueue_minor + i);
        cdev_init(&dev->cdev, &pqueue_fops);
        dev->cdev.owner = THIS_MODULE;
        dev->cdev.ops = &pqueue_fops;
        err = cdev_add(&dev->cdev, devno, 1);
        if (err)
            printk(KERN_NOTICE "Error %d adding pqueue%d", err, i);
    }

    return 0; /* succeed */

  fail:
    pqueue_cleanup_module();
    return result;
}

module_init(pqueue_init_module);
module_exit(pqueue_cleanup_module);
