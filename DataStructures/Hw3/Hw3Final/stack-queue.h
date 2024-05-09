struct node{
    char data[30];
    node *next;
};
struct Stack{
    node *head;
    void create();
    void push(char*);
    char* pop();
    bool isEmpty();
};
struct Queue{
    node *front;
    node *back;
    void create();

    void enqueue(char*);
    char* dequeue();
    char* backnode();
    bool isempty();
};
void Stack::create(){
    head=NULL;
}
void Queue::create(){
    front=NULL; back=NULL;
}
bool Stack::isEmpty(){
    return head == NULL;
}
bool Queue::isempty(){
    return front==NULL;
}
void Stack::push(char*c){
    node* newnode = new node;
    char* newdata = new char[30];
    strcpy(newdata, c);
    strcpy(newnode->data, newdata);
    newnode->next=head;
    head=newnode;
};
void Queue::enqueue(char* newdata){
    node *newnode=new node;
    strcpy(newnode->data, newdata);
    newnode->next=NULL;
    if(isempty()){
        back=newnode;
        front=back;
    }
    else{
        back->next =newnode;
        back=newnode;
    }
}
char* Stack::pop(){
    if(head==NULL){
        return NULL;
    }
    node *temp;
    char *tempchar = new char[30];
    temp = head;
    strcpy(tempchar, head->data);
    head = head->next;
    delete temp;
    return tempchar;
};
char* Queue::dequeue(){
    node *topnode;
    char* temp = new char[30];
    topnode=front;
    front=front->next;
    strcpy(temp, topnode->data);
    delete topnode;
    return temp;
}
char* Queue::backnode(){
    char* temp = new char[30];
    strcpy(temp, back->data);
    return temp;
}
