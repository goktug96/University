#include "Node.h"

class ARTraverser{
public:
    ARTraverser(){};
    Node* current;
    virtual bool hasNode()=0;
    virtual Node* next()=0;
};

class ARTF: public ARTraverser{
public:
    ARTF();
    bool hasNode();
    Node* next();
};
ARTF::ARTF(){  //constructor
    current = NULL;
}
bool ARTF::hasNode(){ //if current node is not empty return true
    if(current!=NULL){
        return true;
    }else return false;
}
Node* ARTF::next(){ //since this traversing strategy starts form from first node to last node we will move next from current
    Node *temp;
    temp = current;
    if(hasNode()){
        current = current->next;
    }
    return temp;  //return current element before moving
}


class ARTL: public ARTraverser{
    public:
    ARTL();
    bool hasNode();
    Node* next();
};
ARTL::ARTL(){ //constructor
    current = NULL;
}
bool ARTL::hasNode(){  //same func.
    if(current!=NULL){
        return true;
    }else return false;
}
Node* ARTL::next(){  //this strategy moves from last to first, so move from current to prev. nodes
    Node *temp;
    temp = current;
    if(hasNode()){
        current = current->prev;
    }
    return temp;
}
