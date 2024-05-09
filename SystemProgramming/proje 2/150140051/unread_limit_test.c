#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "messagebox_ioctl.h"

int main(int argc, char *argv[]){
	unsigned long limit; 
	int status = -1;
	
	if(argc != 2 || atol(argv[1]) <= 0){
		errno = EINVAL;
		perror("Cannot change unread limit!");
		
	}
	else{
		limit = atol(argv[1]);
		int fd = open("/dev/messagebox", O_RDWR);
		status = ioctl(fd, MESSAGEBOX_SET_UNREAD_LIMIT, limit);
	
		if(status == -1)
			perror("Cannot change unread limit!");
	}
	
	return status;
}
