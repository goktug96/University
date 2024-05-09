#ifndef __MESSAGEBOX_H
#define __MESSAGEBOX_H

#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */

typedef struct username {
    int size;
    char buf[50];
}username_t;

#define MESSAGEBOX_IOC_MAGIC  'k'
#define MESSAGEBOX_IOEXCLUDE_READ	_IO(MESSAGEBOX_IOC_MAGIC,	0)
#define MESSAGEBOX_IOINCLUDE_READ	_IO(MESSAGEBOX_IOC_MAGIC,   1)
#define MESSAGEBOX_SET_UNREAD_LIMIT	_IO(MESSAGEBOX_IOC_MAGIC,   2)
#define MESSAGEBOX_IODMESSAGES      _IOW(MESSAGEBOX_IOC_MAGIC,   3, username_t)
#define MESSAGEBOX_IOC_MAXNR 3

#endif
