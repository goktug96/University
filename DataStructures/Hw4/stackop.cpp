#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

using namespace std;

void Stack::create(){
    head = NULL;
}

bool Stack::isEmpty(){
    return head==NULL;
}
void Stack::make_empty(){
    node *p;
    while(head==NULL){
         p=head;
         head = head->next;
         delete p;
    }
}
void Stack::print(){
    cout << "Stack contents: ";
    node *p=head;
    while(p!=NULL){
        cout << p->data << " ";
        p = p->next;
    }
    cout<<endl;
}
void Stack::push(char c){
    node *newnode = new node;
    newnode->data = c;
    newnode->next = head;
    head = newnode;
}
char Stack::pop(){
    if(head == NULL){
        return '\0';
    }
    node *topstack;
    char toReturn;
    topstack = head;
    toReturn = head->data;
    head = head->next;
    delete topstack;
    return toReturn;
}
char Stack::getHeadChar(){
    if(head == NULL){
        return '\0';
    }
    return head->data;
}
