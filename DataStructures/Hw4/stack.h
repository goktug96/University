
struct node{
    char data;
    node* next;
};

struct Stack{
    node* head;
    void create();
    char pop();
    void push(char);
    void make_empty();
    char getHeadChar();
    bool isEmpty();
    void print();
};
