#ifndef __PQUEUE_H
#define __PQUEUE_H

#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */


#define PQUEUE_IOC_MAGIC  'k'

#define PQUEUE_POP	_IO(PQUEUE_IOC_MAGIC,	0)

#endif
