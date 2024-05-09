#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define set_myFlag 355

int main(int argc, char* argv[]){
	int result;
	int pid = atoi(argv[1]);
	int flag_value = atoi(argv[2]);

	result = syscall(set_myFlag, pid, flag_value);
	if(result == 0)
		printf("Success\n");
	else
		printf("ERROR: %s\n", strerror(errno));

	return result;
}
