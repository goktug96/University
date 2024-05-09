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

#include "messagebox_ioctl.h"

#define MESSAGEBOX_MAJOR 0
#define MESSAGEBOX_NR_DEVS 4
#define MESSAGEBOX_INCLUDE_READ 0
#define MESSAGEBOX_MESSAGE_NUMBER 1000
#define MESSAGEBOX_UNREAD_LIMIT 10 /* maksimum okunmamış mesaj sayısı -> 11. mesaj gönderilmeye çalışıldığında "quota exceed" hatası */

int messagebox_major = MESSAGEBOX_MAJOR;
int messagebox_minor = 0;
int messagebox_nr_devs = MESSAGEBOX_NR_DEVS;
int messagebox_include_read = MESSAGEBOX_INCLUDE_READ;
int messagebox_message_number = MESSAGEBOX_MESSAGE_NUMBER;
int messagebox_unread_limit = MESSAGEBOX_UNREAD_LIMIT;

module_param(messagebox_major, int, S_IRUGO);
module_param(messagebox_minor, int, S_IRUGO);
module_param(messagebox_nr_devs, int, S_IRUGO);
module_param(messagebox_include_read, int, S_IRUGO);
module_param(messagebox_message_number, int, S_IRUGO);
module_param(messagebox_unread_limit, int, S_IRUGO);

MODULE_AUTHOR("Alessandro Rubini, Jonathan Corbet");
MODULE_LICENSE("Dual BSD/GPL");


struct message
{
    char *message;
    char *to;
    char *from;
    int read;
};

struct messagebox_dev
{
    struct message **data;
    int message_number;
    unsigned long size;
    struct semaphore sem;
    struct cdev cdev;
};

struct messagebox_dev *messagebox_devices;

int atoi(char *array, int size)
{
    int i;
    int ret = 0;
    int digit = 1;
    for( i = size - 1; i >= 0; i--) {
        ret += (array[i] - '0')*digit;
        digit *= 10;
    }
    return ret;
}

void delete_messages(struct file *filp, username_t *username)
{
    struct messagebox_dev *dev = filp->private_data;
    struct message *msg;
    int i, j;

    for (i = 0; i < dev->message_number; i++) {
        msg = dev->data[i];

        if (strcmp(msg->to, username->buf)) {
            kfree(msg->message);
            kfree(msg->to);
            kfree(msg->from);
            kfree(msg);

            for(j = i+1; j < dev->message_number; j++) {
                dev->data[j-1] = dev->data[j];
            }
            dev->message_number--;
            i--;
        }
    }
}

char *get_user_name(void)
{
    int i = 0, ui = 0, idi = 0;
    int colon = 0;
    loff_t offset = 0;
    int max_size = 200;
    mm_segment_t oldfs;
    struct file *filp;
    
    char *buffer;
    char *username;
    char *uid_str;

    kuid_t uid = get_current_user()->uid;

    oldfs = get_fs();
    set_fs(KERNEL_DS);

    /* We are using /etc/passwd file to acquire the name of the current user */
    filp = filp_open("/etc/passwd", O_RDONLY, 0);

    buffer = kmalloc(max_size * sizeof(char *), GFP_KERNEL);
    username = kmalloc(50 * sizeof(char * ), GFP_KERNEL);
    uid_str = kmalloc(10 * sizeof(char * ), GFP_KERNEL);

    while(vfs_read(filp, buffer + i, 1, &offset)) {
        /* If we consumed the line, check if this is current user */
        if ( buffer[i] == '\n' ) {

            username[ui] = 0;
            uid_str[idi] = 0;

            if (atoi(uid_str, idi) == uid.val)
            {
                set_fs(oldfs);
                filp_close(filp, NULL);
                kfree(buffer);
                kfree(uid_str);
                return username;
            }

            i = ui = idi = colon = 0;
            continue;
        }

        if (buffer[i] == ':') {
            colon++;
            i++;
            continue;
        }

        switch (colon) {
            case 0:
                username[ui++] = buffer[i];
                break;

            case 2:
                uid_str[idi++] = buffer[i];
                break;

            default:
                break;
        }
        i++;
    }

    set_fs(oldfs);
    filp_close(filp, NULL);
    kfree(buffer);
    kfree(username);
    kfree(uid_str);
    return NULL;
}

int messagebox_trim(struct messagebox_dev *dev)
{
    int i;

    if (dev->data) {
        for (i = 0; i < dev->message_number; i++) {
            if (dev->data[i])
            kfree(dev->data[i]);
        }
        kfree(dev->data);
    }
    dev->data = NULL;
    dev->message_number = 0;
    dev->size = messagebox_message_number;
    return 0;
}


int messagebox_open(struct inode *inode, struct file *filp)
{
    struct messagebox_dev *dev;

    dev = container_of(inode->i_cdev, struct messagebox_dev, cdev);
    filp->private_data = dev;

    /* trim the device if open was write-only */
    if ((filp->f_flags & O_APPEND) == 0 && (filp->f_flags & O_ACCMODE) == O_WRONLY) {
        if (down_interruptible(&dev->sem))
            return -ERESTARTSYS;
        messagebox_trim(dev);
        up(&dev->sem);
    }
    return 0;
}


int messagebox_release(struct inode *inode, struct file *filp)
{
    return 0;
}


ssize_t messagebox_read(struct file *filp, char __user *buf, size_t count,
                   loff_t *f_pos)
{
    struct messagebox_dev *dev = filp->private_data;
    struct message *msg;
    int message_pos = (long)*f_pos;
    ssize_t retval = 0;
    int pos, line = 0, buf_pos = 0, prev = 0;
    
    char *this_user;
    char *local_buffer;

    if (down_interruptible(&dev->sem))
        return -ERESTARTSYS;

    if (*f_pos >= dev->message_number)
        goto out;

    if (dev->data == NULL || !dev->data[message_pos])
        goto out;

    this_user = get_user_name();
    local_buffer = kmalloc((count + 50) * sizeof(char), GFP_KERNEL);
    memset(local_buffer, 0, (count + 50) * sizeof(char));

    for(pos = message_pos; pos < dev->message_number; pos++) {
        msg = dev->data[pos];

        if (strcmp(msg->to, this_user))
            continue;
        if (!messagebox_include_read && msg->read)
			continue;

        prev = buf_pos;

        line = -1;
        while (msg->from[++line])
            local_buffer[buf_pos++] = msg->from[line];

        local_buffer[buf_pos++] = ':';
        local_buffer[buf_pos++] = ' ';

        line = -1;
        while (msg->message[++line])
            local_buffer[buf_pos++] = msg->message[line];
		
		msg->read = 1;
		
        if (buf_pos > count) {
            buf_pos = prev;
            break;
        }
    }

    if (copy_to_user(buf, local_buffer, buf_pos)) {
        retval = -EFAULT;
        goto out;
    }
    *f_pos += buf_pos;
    retval = buf_pos;

  out:
    up(&dev->sem);
    return retval;
}

int get_unread_count(struct messagebox_dev *dev, char *username)	/* bir kullanıcının okunmamış mesaj sayısı */
{
	struct message *msg;
	int pos, unread_count = 0;
	for(pos = 0; pos < dev->message_number; pos++) {
        msg = dev->data[pos];
        if (strcmp(msg->to, username) || msg->read)
            continue;
		unread_count++;
    }
	return unread_count;
}

ssize_t messagebox_write(struct file *filp, const char __user *buf, size_t count,
                    loff_t *f_pos)
{
    struct messagebox_dev *dev = filp->private_data;
    ssize_t retval = -ENOMEM;
    /* We don't use f_pos because echo sends f_pos as 0 when appending */
    int message_pos = dev->message_number;
    char *local_buffer;
    char *from_username;
    char *to_username;
    char *message;
    
    int pos = 0;
    int m_pos = 0;
    int unread_number;

    if (down_interruptible(&dev->sem))
        return -ERESTARTSYS;

    if (message_pos >= dev->size) {
        goto out;
    }

    if (!dev->data) {
        dev->data = kmalloc(dev->size * sizeof(struct message *), GFP_KERNEL);
        if (!dev->data)
            goto out;
        memset(dev->data, 0, dev->size * sizeof(struct message *));
    }
    if (!dev->data[message_pos]) {
        dev->data[message_pos] = kmalloc(sizeof(struct message), GFP_KERNEL);
        if (!dev->data[message_pos])
            goto out;
    }

    local_buffer = kmalloc(count * sizeof(char), GFP_KERNEL);

    if (copy_from_user(local_buffer, buf, count) || local_buffer[0] != '@') {
        retval = -EFAULT;
        goto out;
    }

    from_username = get_user_name();
    to_username = kmalloc(50 * sizeof(char), GFP_KERNEL);
    message = kmalloc(100 * sizeof(char), GFP_KERNEL);

    while (local_buffer[++pos] != ' ')
        to_username[pos-1] = local_buffer[pos];
    to_username[pos-1] = 0;
    
    /* to_username'in okunmamış mesaj sayısını messagebox_unread_limit ile karşılaştır.
     * eğer limite ulaşılmışsa mesaj yazmaya izin verme ve buna uygun hata mesajı üret. */
     /* EDQUOT          122   Quota exceeded */
     unread_number = get_unread_count(dev, to_username);
     if(unread_number >= messagebox_unread_limit){
		 retval = -EDQUOT;
		 goto out;
	 }

    while(++pos < count)
        message[m_pos++] = local_buffer[pos];
    message[m_pos] = 0;

    dev->data[message_pos]->message = message;
    dev->data[message_pos]->to = to_username;
    dev->data[message_pos]->from = from_username;
    dev->data[message_pos]->read = 0;

    dev->message_number++;
    (*f_pos)++;

    retval = count;
    kfree(local_buffer);
  out:
    up(&dev->sem);
    return retval;
}

long messagebox_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

	int err = 0;
	int retval = 0;

	/*
	 * extract the type and number bitfields, and don't decode
	 * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
	 */
	if (_IOC_TYPE(cmd) != MESSAGEBOX_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > MESSAGEBOX_IOC_MAXNR) return -ENOTTY;

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
		case MESSAGEBOX_IOEXCLUDE_READ:
			if (! capable (CAP_SYS_ADMIN))
				return -EPERM;
			messagebox_include_read = 0;
			break;
		case MESSAGEBOX_IOINCLUDE_READ:
			if (! capable (CAP_SYS_ADMIN))
				return -EPERM;
			messagebox_include_read = 1;
			break;
		case MESSAGEBOX_SET_UNREAD_LIMIT:
			if (! capable (CAP_SYS_ADMIN))
				return -EPERM;
			messagebox_unread_limit = arg;
			break;
		case MESSAGEBOX_IODMESSAGES:
			if (! capable (CAP_SYS_ADMIN))
				return -EPERM;
			username_t username;
			if (copy_from_user(&username, (char __user*)arg, sizeof(username_t)))
				return -EFAULT;
			delete_messages(filp, &username);
			break;
		default:  /* redundant, as cmd was checked against MAXNR */
			return -ENOTTY;
	}
	return retval;
}


loff_t messagebox_llseek(struct file *filp, loff_t off, int whence)
{
    struct messagebox_dev *dev = filp->private_data;
    loff_t newpos;

    switch(whence) {
        case 0: /* SEEK_SET */
            newpos = off;
            break;

        case 1: /* SEEK_CUR */
            newpos = filp->f_pos + off;
            break;

        case 2: /* SEEK_END */
            newpos = dev->message_number + off;
            break;

        default: /* can't happen */
            return -EINVAL;
    }
    if (newpos < 0)
        return -EINVAL;
    filp->f_pos = newpos;
    return newpos;
}


struct file_operations messagebox_fops = {
    .owner =    THIS_MODULE,
    .llseek =   messagebox_llseek,
    .read =     messagebox_read,
    .write =    messagebox_write,
    .unlocked_ioctl =  messagebox_ioctl,
    .open =     messagebox_open,
    .release =  messagebox_release,
};


void messagebox_cleanup_module(void)
{
    int i;
    dev_t devno = MKDEV(messagebox_major, messagebox_minor);

    if (messagebox_devices) {
        for (i = 0; i < messagebox_nr_devs; i++) {
            messagebox_trim(messagebox_devices + i);
            cdev_del(&messagebox_devices[i].cdev);
        }
    kfree(messagebox_devices);
    }

    unregister_chrdev_region(devno, messagebox_nr_devs);
}


int messagebox_init_module(void)
{
    int result, i;
    int err;
    dev_t devno = 0;
    struct messagebox_dev *dev;

    if (messagebox_major) {
        devno = MKDEV(messagebox_major, messagebox_minor);
        result = register_chrdev_region(devno, messagebox_nr_devs, "messagebox");
    } else {
        result = alloc_chrdev_region(&devno, messagebox_minor, messagebox_nr_devs,
                                     "messagebox");
        messagebox_major = MAJOR(devno);
    }
    if (result < 0) {
        printk(KERN_WARNING "messagebox: can't get major %d\n", messagebox_major);
        return result;
    }

    messagebox_devices = kmalloc(messagebox_nr_devs * sizeof(struct messagebox_dev),
                            GFP_KERNEL);
    if (!messagebox_devices) {
        result = -ENOMEM;
        goto fail;
    }
    memset(messagebox_devices, 0, messagebox_nr_devs * sizeof(struct messagebox_dev));

    /* Initialize each device. */
    for (i = 0; i < messagebox_nr_devs; i++) {
        dev = &messagebox_devices[i];
        dev->size = messagebox_message_number;
        dev->message_number = 0;
        sema_init(&dev->sem,1);
        devno = MKDEV(messagebox_major, messagebox_minor + i);
        cdev_init(&dev->cdev, &messagebox_fops);
        dev->cdev.owner = THIS_MODULE;
        dev->cdev.ops = &messagebox_fops;
        err = cdev_add(&dev->cdev, devno, 1);
        if (err)
            printk(KERN_NOTICE "Error %d adding messagebox%d", err, i);
    }

    return 0; /* succeed */

  fail:
    messagebox_cleanup_module();
    return result;
}

module_init(messagebox_init_module);
module_exit(messagebox_cleanup_module);
