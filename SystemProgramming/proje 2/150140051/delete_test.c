#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "messagebox_ioctl.h"

int main(int argc, char *argv[]){

    username_t username;

    strncpy(username.buf, argv[1], strlen(argv[1]));

	int fd = open("/dev/messagebox", O_RDWR);
	int status = ioctl(fd, MESSAGEBOX_IODMESSAGES, &username);
	
	if(status == -1)
		perror("Cannot change reading mode!");
		
	return status;
}
