#include <stdio.h>

//simple stack structure
typedef struct node{
    int data;
    struct node* next;
}Stack;


void init(struct node* head){
    head = NULL;
}


struct node* push(struct node* head,int data){
    struct node* tmp = (struct node*)malloc(sizeof(struct node));
    tmp->data = data;
    tmp->next = head;
    head = tmp;
    return head;
}

struct node* pop(struct node *head,int *element){
    struct node* tmp = head;
    *element = head->data;
    head = head->next;
    free(tmp);
    return head;
}

int empty(struct node* head){
    return head == NULL ? 1 : 0;
}

