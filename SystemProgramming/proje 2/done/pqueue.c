#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/device.h>

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

#define PQUEUE_MAJOR 0
#define PQUEUE_NR_DEVS 4


int pqueue_major = PQUEUE_MAJOR;
int pqueue_minor = 0;
int pqueue_nr_devs = PQUEUE_NR_DEVS;


module_param(pqueue_major, int, S_IRUGO);
module_param(pqueue_minor, int, S_IRUGO);
module_param(pqueue_nr_devs, int, S_IRUGO);

MODULE_AUTHOR("Alessandro Rubini, Jonathan Corbet");
MODULE_LICENSE("Dual BSD/GPL");

typedef struct Node myNode;

struct Node{
    char *text;
    myNode *next;
};

struct pqueue_dev{
    myNode *head;
    int size;
    struct semaphore sem;
    struct cdev cdev;
};

struct pqueue_dev *pqueue_devices;


int pqueue_trim(struct pqueue_dev *dev){
    int i;

    if (dev->head) {
        for (i = 0; i < dev->size; i++) {
            myNode *temp = dev->head;
            dev->head = dev->head->next;
            kfree(temp->text);
            kfree(temp);
        }
    }
    dev->size=0;
    return 0;
}


int pqueue_open(struct inode *inode, struct file *filp){
    struct pqueue_dev *dev;

    dev = container_of(inode->i_cdev, struct pqueue_dev, cdev);
    filp->private_data = dev;

    return 0;
}


int pqueue_release(struct inode *inode, struct file *filp){
    return 0;
}

ssize_t pqueue_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos){
    struct pqueue_dev *dev = filp->private_data;
    myNode *cur_node;
    ssize_t retval = 0;
    int line, i, buf_pos = 0;
    char *local_buffer;
    int min_num;

    min_num = iminor(filp->f_path.dentry->d_inode);
    if(min_num == 0){
        return -EACCES;
    }

    if (down_interruptible(&dev->sem))
        return -ERESTARTSYS;

    if (*f_pos >= dev->size)
        goto out;

    if (dev->head == NULL)
        goto out;

    local_buffer = kmalloc((count + 50) * sizeof(char), GFP_KERNEL);
    memset(local_buffer, 0, (count + 50) * sizeof(char));

    cur_node = dev->head;
    for(i=0; i<dev->size; i++) {
        line = 0;
        while (cur_node->text[line]){
            local_buffer[buf_pos++] = cur_node->text[line];
            line++;
        }
        local_buffer[buf_pos-1]='\0';
        local_buffer[buf_pos++] = ' ';
        if (buf_pos > count) {
            count = buf_pos;
        }
        cur_node = cur_node->next;
    }
    local_buffer[buf_pos++] = '\n';
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

ssize_t pqueue_write(struct file *filp, const char __user *buf, size_t count,loff_t *f_pos){
    struct pqueue_dev *dev = filp->private_data;
    ssize_t retval = -ENOMEM;
    char *local_buffer;
    char *text;
    myNode *cur_node;
    myNode *tempNode;
    int pos = 0;
    int min_num;

    min_num = iminor(filp->f_path.dentry->d_inode);
    if(min_num == 0){
        return -EACCES;
    }

    if (down_interruptible(&dev->sem))
        return -ERESTARTSYS;

    local_buffer = kmalloc(count * sizeof(char), GFP_KERNEL);

    if (copy_from_user(local_buffer, buf, count)) {
        retval = -EFAULT;
        goto out;
    }

    text = kmalloc(100 * sizeof(char), GFP_KERNEL);

    while(pos < count){
        text[pos] = local_buffer[pos];
        pos++;
    }

    text[pos] = 0;

    if (dev->head==NULL) {
        dev->head = kmalloc(sizeof(struct myNode *), GFP_KERNEL);
        dev->head->next = NULL;
        dev->head->text = text;
        if (!dev->head)
            goto out;
    }
    else{
        cur_node = dev->head;
        while(cur_node->next!=NULL)
            cur_node = cur_node->next;

        tempNode = kmalloc(sizeof(struct myNode *), GFP_KERNEL);
        tempNode->next = NULL;
        tempNode->text = text;
        cur_node->next = tempNode;
    }

    (*f_pos)++;
    dev->size++;
    retval = count;
    kfree(local_buffer);
  out:
    up(&dev->sem);
    return retval;
}

int pop(void){
    int i;
    myNode *temp;
    printk("inside pop \n");
    for(i=1; i<pqueue_nr_devs; i++){
        struct pqueue_dev *dev;
        dev = &pqueue_devices[i];
        printk("device %d, size: %d\n", i, dev->size);
        if(dev->size){
            temp = dev->head;
            if(temp){
                printk("head taken to temp. temp->text: %s\n", temp->text);
            }
            dev->head = dev->head->next;
            printk("head is head->next. new head's text: %s\n", dev->head->text);
            kfree(temp->text);
            printk("temp text freed\n");
            kfree(temp);
            printk("temp freed\n");
            dev->size--;
            break;
        }
    }
    return 0;
}

long pqueue_ioctl(struct file *filp, unsigned int cmd, unsigned long arg){
    int err = 0;
	int retval = 0;
	int min_num;

	/*
	 * extract the type and number bitfields, and don't decode
	 * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
	 */
	if (_IOC_TYPE(cmd) != PQUEUE_IOC_MAGIC) return -ENOTTY;

	/*
	 * the direction is a bitmask, and VERIFY_WRITE catches R/W
	 * transfers. `Type' is user-oriented, while
	 * access_ok is kernel-oriented, so the concept of "read" and
	 * "write" is reversed
	 */
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) return -EFAULT;

	switch(cmd) {
		case PQUEUE_POP:
            printk("Calling pop\n");
            min_num = iminor(filp->f_path.dentry->d_inode);
            if(min_num != 0)
				return -EPERM;
			pop();
			break;
		default:  /* redundant, as cmd was checked against MAXNR */
			return -ENOTTY;
	}
	return retval;
}


loff_t pqueue_llseek(struct file *filp, loff_t off, int whence){
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


int pqueue_init_module(void){
    int result, i;
    int err;
    dev_t devno = 0;
    struct pqueue_dev *dev;
    if (pqueue_major) {
        devno = MKDEV(pqueue_major, pqueue_minor);
        result = register_chrdev_region(devno, pqueue_nr_devs, "pqueue");
    }else {
        result = alloc_chrdev_region(&devno, pqueue_minor, pqueue_nr_devs, "pqueue");
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
        dev->size = 0;
        dev->head = NULL;
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
