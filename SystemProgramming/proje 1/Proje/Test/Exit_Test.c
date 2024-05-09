#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>
#define set_myFlag 355


int main(){

    int i, f=1, children[3], myOrder;

    for (i=0; i<3; i++){
        if(f>0)
           f = fork();
        if(f==-1){
            printf("fork error ");
            exit(1);
        }
        if(f==0)
            break;
        else
            children[i] = f;
    }

    if(f>0){
        int mypid = getpid();
        printf("I am the parent process. My pid is: %d\n", mypid);
        while(1);
 
    }
    else{
        myOrder = i;
        printf("I am the child %d, My pid is: %d My parent's pid is: %d\n", myOrder, getpid(), getppid());
        while(1);
    }
}
