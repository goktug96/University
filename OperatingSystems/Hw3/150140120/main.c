#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "Stack.h"

#define KEYSHM1 1
#define KEYSHM2 2
#define KEYSHM3 3
#define SEMKEY 4
#define SEMKEY2 5
#define SEMKEY3 6



void sem_signal(int semid, int val){
    struct sembuf semaphore;
    semaphore.sem_num = 0;
    semaphore.sem_op = val;
    semaphore.sem_flg = 1;
    semop(semid, &semaphore, 1);
}

void sem_wait(int semid, int val){
    struct sembuf semaphore;
    semaphore.sem_num = 0;
    semaphore.sem_op = (-1*val);
    semaphore.sem_flg = 1;
    semop(semid, &semaphore, 1);
}

void PrintCurrent(int* total){
    printf("Current water level %d cups\n", *total);
}

int main(int argc, char* argv[]){
    char *filename = argv[1];
    Stack* temp = NULL; //dynamic stack for holding costumers temporarily
    init(temp);
    int* Costumers; //array for holding costumers in shared memory
    //3 shared memory for totalCup number, counter, costumers
    int shmid1=0, shmid2 = 0, shmid3=0, key_a, key_b, key_c;
    int initCup;
    int *totalCup;
    int *counter;
    int i=0, f;
    FILE* fp;
    int element, nofElements = 0;

    //key_a and key_b are for synchronizing costumers
    /*key_b is necessary because after filler fills the machine and
    releases the key we need to ensure that other costumers won't lock this key before
    the costumer that waits for filler operation in queue*/
    key_a = semget(SEMKEY, 1, 0700|IPC_CREAT);
    semctl(key_a, 0, SETVAL, 1);
    key_b = semget(SEMKEY2, 1, 0700|IPC_CREAT);
    semctl(key_b, 0, SETVAL, 1);
    /*key_c is used for communication with filler, other approach
    could be sending signals but I prefered semaphore method*/
    key_c = semget(SEMKEY3, 1, 0700|IPC_CREAT);
    semctl(key_c, 0, SETVAL, 0);


    /*Shared memory for counter variable, this counter is used to inform filler that
    all costumers have got their orders and it doesn't have to sleep anymore*/
    shmid2 = shmget(KEYSHM2, 1*sizeof(int), 0700|IPC_CREAT);
    counter = (int *)shmat(shmid2, NULL, 0);
    *counter = 0;
    shmdt(counter);

    fp = fopen(filename, "r");

     //shared memory for tracking amount of cups remaining
    shmid1 = shmget(KEYSHM1, 1*sizeof(int), 0700|IPC_CREAT);
    totalCup = (int *)shmat(shmid1, NULL, 0);
    fscanf (fp, "%d", totalCup); //read first line = total cup number
    printf("SIMULATION BEGINS\n");
    PrintCurrent(totalCup); //function for printing current amount of cups
    initCup = *totalCup;
    shmdt(totalCup);

    while(!feof(fp)){  //read costumers from and push them to the temporary stack
        fscanf(fp, "%d", &i);
        nofElements++; //count number of costumers to take enough shared memory
        temp = push(temp, i);
    }

    //create the shared memory for costumers
    shmid3 = shmget(KEYSHM3, nofElements*sizeof(int), 0700|IPC_CREAT);
    i=0;
    Costumers = (int*)shmat(shmid3, NULL, 0);
    while(empty(temp)==0){
        temp = pop(temp, &element); //temp is emptied
        Costumers[i] = element; //add elements of stack to costumer array
        i++;
    }
    shmdt(Costumers);
    fclose(fp);

    for(i=0; i<nofElements; i++){
 		f=fork();
		if (f==0)
			break;
	}
	if(f>0){  //filler process
        counter = (int *)shmat(shmid2, NULL, 0);
        while(*counter<nofElements){  //until all costumers finish their job
            key_c = semget(SEMKEY3, 1, 0);
            sem_wait(key_c, 1); //wait until key_c is released, which means machine needs to be filled
            counter = (int *)shmat(shmid2, NULL, 0);
            printf("Employee %d wakes up and fills the coffee machine\n", getpid());
            shmid1 = shmget(KEYSHM1, 1*sizeof(int), 0);
            totalCup = (int *)shmat(shmid1, NULL, 0);
            *totalCup = initCup;  //fill machine
            shmdt(totalCup);
            sem_signal(key_b, 1); //release lock for the waiting costumer
        }

        printf("SIMULATION ENDS \n");
        semctl(key_a, 0, IPC_RMID, 0);  //release semaphores and shared memories
        semctl(key_b, 0, IPC_RMID, 0);
        semctl(key_c, 0, IPC_RMID, 0);
        shmctl(shmid1, IPC_RMID, 0);
        shmctl(shmid2, IPC_RMID, 0);
        shmctl(shmid3, IPC_RMID, 0);
        exit(0);
	}
	else{ //costumer processes
        int turn = i;
        key_a = semget(SEMKEY, 1, 0);
        key_c = semget(SEMKEY3, 1, 0);
        key_b = semget(SEMKEY2, 1, 0);
        sem_wait(key_a, 1);  //lock key_a
        sem_wait(key_b, 1);  //lock key_b

        shmid3 = shmget(KEYSHM3, 0, 0);
        Costumers = shmat(shmid3, NULL, 0);
        int order = Costumers[turn]; // get orders of costumer
        shmdt(Costumers);
        totalCup = (int *)shmat(shmid1, NULL, 0);  //get current values
        counter = (int *)shmat(shmid2, NULL, 0);


        printf("Employee %d wants coffee type %d\n", getpid(), order);

        if (*totalCup-order>0){ //if there is enough cup
            printf("Employee %d SERVED\n", getpid());
            *counter=*counter + 1;  //increase counter
            *totalCup = *totalCup - order;  //update cup amount
            PrintCurrent(totalCup);
            sem_signal(key_a, 1); //release keys
            sem_signal(key_b, 1);
        }
        else{  //if there is not enough cups
            printf("Employee %d WAITS\n", getpid());
            sem_signal(key_c, 1);  //wake filler up
            /* Note that key b is released but process still holds key a, therefore
            other processes can not lock neither key_a, nor key_b*/
            sem_wait(key_b, 1); //wait until filler finishes his job and releases key
            PrintCurrent(totalCup);
            *totalCup = *totalCup - order;  //update current cup
            *counter=*counter + 1; //increase counter
            printf("Employee %d SERVED\n", getpid());
            PrintCurrent(totalCup);
            sem_signal(key_a, 1);  //release keys
            sem_signal(key_b, 1);
        }


        if(*counter == nofElements)  //if all costumers are served, wake filler up
            sem_signal(key_c, 1);
        shmdt(totalCup);
        shmdt(counter);
    }

	return 0;
}
