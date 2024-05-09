#include "Node.h"
#include "ARTraverser.h"

using namespace std;

class AdderRemover{
protected:
    Node *head;
    Node *tail;
    ARTraverser* traverser;
    string name;
    int nodeCount;
public:
    AdderRemover();
    virtual void add(int)=0;
    virtual void remove()=0;
    virtual void setTraverser()=0;
    void display();
    void removeAll();
    void traverse();
};
AdderRemover::AdderRemover(){ //constructor
    head = NULL;
    tail = NULL;
    nodeCount = 0;
};
void AdderRemover::removeAll(){

    Node *temp;
    while(head!=NULL){  //remove all nodes
        temp = head;
        head = head->next;
        delete temp;
    }
    nodeCount = 0;
}
void AdderRemover::traverse(){
    while(traverser->hasNode()){  //if hasNode function sends true traverse by next function
    cout << traverser->next()->data<<endl;
    }
}
void AdderRemover::display(){  //display all elements
    cout <<  name << " | NodeCount: " << nodeCount << endl;
    cout << "------" << endl;
    if(head==NULL){   //if there is no element
        cout<< "There is no element to print!" << endl<<endl;
    }
    Node *temp;
    temp = head;
    while(temp != NULL){
        cout << temp->data << endl;
        temp = temp->next;
    }
}

class FAFR : public AdderRemover{
public:
    FAFR();
    void add(int);
    void setTraverser();
    void remove();
};
FAFR::FAFR(){  //constructor
    name = "FAFR";
}
void FAFR::remove(){   //remove first element
    Node* temp=head;
    head = head->next;
    head->prev = NULL;
    delete temp;
    nodeCount--;
}
void FAFR::setTraverser(){  // set to ARTF, since we will traverse from beginning current node is head
    traverser = new ARTF;
    traverser->current = head;
    if(head==NULL){
        cout<< "There is no element to print!" << endl<<endl;
        return;
    }
    cout <<  name << " | NodeCount: " << nodeCount << endl;
    cout << "------" << endl;
}
void FAFR::add(int x){  //add to beginning
    Node *newNode = new Node;
    newNode->data = x;
    if(head == NULL){
        head=newNode;
        nodeCount++;
        return;
    }
    head->prev = newNode;
    newNode->next = head;
    head = newNode;
    nodeCount++;
}

class FALR : public AdderRemover{
public:
    FALR();
    void add(int);
    void setTraverser();
    void remove();
};
FALR::FALR(){  //constructor
    name = "FALR";
}
void FALR::remove(){  //remove last element
    Node* temp=tail;
    tail = tail->prev;
    tail->next=NULL;
    delete temp;
    nodeCount--;
}
void FALR::setTraverser(){  //set traverser to ARTF, since we'll traverse from beginning current is head
    traverser = new ARTF;
    traverser->current = head;
    if(head==NULL){
        cout<< "There is no element to print!" << endl<<endl;
        return;
    }
    cout <<  name << " | NodeCount: " << nodeCount << endl;
    cout << "------" << endl;
}
void FALR::add(int x){  //add to beginning
    Node *newNode = new Node;
    newNode->data = x;
    if(head == NULL){
        head=newNode;
        tail = newNode;//since we are adding to beginning of the list, tail will be first element which will be last element later on
        nodeCount++;
        return;
    }
    head->prev = newNode;
    newNode->next = head;
    head = newNode;
    nodeCount++;
}

class LAFR : public AdderRemover{
public:
    LAFR();
    void add(int);
    void setTraverser();
    void remove();
};
LAFR::LAFR(){ //constructor
    name = "LAFR";
}
void LAFR::remove(){  //remove first element
    Node* temp=head;
    head = head->next;
    head->prev = NULL;
    delete temp;
    nodeCount--;
}
void LAFR::setTraverser(){  //set traverser to ARTL, since we will start from last element current is tail
    traverser = new ARTL;
    traverser->current = tail;
    if(tail==NULL){
        cout<< "There is no element to print!" << endl<<endl;
        return;
    }
    cout <<  name << " | NodeCount: " << nodeCount << endl;
    cout << "------" << endl;
}
void LAFR::add(int x){  //ad to the end
    Node *newNode = new Node;
    newNode->data = x;
    if(head == NULL){
        head=newNode;
        tail = newNode;
        nodeCount++;
        return;
    }
    tail->next = newNode;  //here first element is not changing (head), last element (tail) is changing
    newNode->prev = tail;
    tail = newNode;
    nodeCount++;
}


class LALR : public AdderRemover{
public:
    LALR();
    void add(int);
    void setTraverser();
    void remove();
};
LALR::LALR(){  //constructor
    name = "LALR";
}
void LALR::remove(){ //remove last element
    Node* temp=tail;
    tail = tail->prev;
    tail->next=NULL;
    delete temp;
    nodeCount--;
}
void LALR::setTraverser(){  //set traverser to ARTL, starting from last node, current is tail
    traverser = new ARTL;
    traverser->current = tail;
    if(tail==NULL){
        cout<< "List is empty!" << endl;
        return;
    }
    cout <<  name << " | NodeCount: " << nodeCount << endl;
}
void LALR::add(int x){  // add to the end
    Node *newNode = new Node;
    newNode->data = x;
    if(head == NULL){
        head=newNode;
        tail = newNode;
        nodeCount++;
        return;
    }
    tail->next = newNode;
    newNode->prev = tail;
    tail = newNode;
    nodeCount++;
}
