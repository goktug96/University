#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "messagebox_ioctl.h"

int main(int argc, char *argv[]){
	int mode;
	
	if(atoi(argv[1]) == 1)
		mode = MESSAGEBOX_IOINCLUDE_READ;
	else if(atoi(argv[1]) == 0)
		mode = MESSAGEBOX_IOEXCLUDE_READ;
		
	int fd = open("/dev/messagebox", O_RDWR);
	int status = ioctl(fd, mode, 0);
	
	if(status == -1)
		perror("Cannot change reading mode!");
		
	return status;
}
