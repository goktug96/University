#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

void mysignal(int signum){
	printf("Received signal: %d\n", signum);
}

void mysigset(int num){
	struct sigaction mysigaction;
	mysigaction.sa_handler = (void *) mysignal;
	mysigaction.sa_flags= 0;
	sigaction(num, &mysigaction, NULL);
}

int main(){
    mysigset(12);
    int i, f=1, children[3], myOrder;
    pause();

    for (i=0; i<3; i++){
        if(f>0)
           f = fork();
        if(f<0){
            printf("ERROR: %s\n ", strerror(errno));
        }
        if(f==0)
            break;
        else
            children[i] = f;
    }

    if(f>0){
        printf("I am the parent process. My pid is: %d\n", getpid());
    }
    else if(f==0){
        myOrder = i;
        printf("I am the child %d, My pid is: %d My parent's pid is: %d\n", myOrder, getpid(), getppid());
    }else{
	printf("I am the parent process, child process could not be produced\n");
    }
}
