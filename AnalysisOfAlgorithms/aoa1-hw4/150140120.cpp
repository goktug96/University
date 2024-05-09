/*Ahmet Göktug SEVINC
    150140120
*/

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <iomanip>

using namespace std;

struct Node{
    char name[20];
    char gender;
    int age;
    char color;
    Node *left, *right, *parent;
    int numF;
    int numM;
    Node()
    {
       left =NULL;
       right = NULL;
       parent = NULL;
       color = 'r';
    }
};

class RBTree{

public:
    Node *root;
    void rotateLeft(Node *);
    void rotateRight(Node *);
    void makeRBT(Node *);
    RBTree() { root = NULL; }
    void insert(Node*);
    void readFile(char*);
};

void RBTree::readFile(char* filename){
    FILE *fptr;
    fptr = fopen(filename, "r");
    char taken[150];
    const char* part;
    //for(int i=0; i<9; i++){
    while(!feof(fptr)){
        fgets(taken, 150, fptr);  //get first line
        if(feof(fptr)){
            break;
        }
        Node* nd = new Node;
        part = strtok(taken, "\t");  //parse line
        strcpy(nd->name, part);
        part = strtok(NULL, "\t");
        nd->gender = part[0];
         if(nd->gender =='F'){  //if female increase numF of node
            nd->numF = 1;
            nd->numM = 0;
       }else{
            nd->numM = 1;  //if male increase numM of node
            nd->numF = 0;
       }
        part = strtok(NULL, "\t");
        nd->age = atoi(part) ;
        insert(nd);  //insert book character to dictionary
    }
}

Node* treeIns(Node* r, Node* n){
    if(r == NULL){
        return  n;
    }else{

        if(n->gender =='F'){
            r->numF++;  //if the node we will insert is female increase every nodes' numF that it pass.
        }else{
            r->numM++;
        }
        if(strcmp(n->name, r->name)<0){
            r->left = treeIns(r->left, n); //if smaller look left side
            r->left->parent = r;  //after return set r->left' parent is r
        }else{
            r->right = treeIns(r->right, n);
            r->right->parent = r;
        }
    }
    return r;
}

void RBTree::insert(Node* n){
    root = treeIns(root, n);  //insert to a tree
    makeRBT(n);  //make that tree RBT
}

void RBTree::rotateLeft( Node* n){  //from the lecture slides (rotate 7 to left)
    Node* right_node = n->right;   // 10 is right_node
    Node* parent_node  = n->parent;
    int rightMale = right_node->numM;
    int rightFemale = right_node->numF;
    int Male = n->numM;
    int Female = n->numF;
    int lMale = 0;
    int lFemale = 0;
    if(right_node->left !=NULL){  // 8 = not null
        lMale = right_node->left->numM; //keep number of male and female in the right node's left tree we will attach this to node
        lFemale = right_node->left->numF;
        right_node->left->parent = n;  // 8's parent is 7 now
        n->right=right_node->left;  //7's right child is 8 now
    }else{
        n->right = NULL;   //if 8 would be null 7's right child was going to be null
    }
    if(n->parent == NULL){   //if 7's parent is null it means 7 was root
            right_node->numF = Female;  //we will change node with it's parent so number of male and female will change
            right_node->numM = Male;
            right_node->parent = NULL;  //now 10 will be root so 10's parent is null
            root = right_node; //10 is root
    }else{
        if(parent_node->right == n){  //if n is attached to it's parent from right
            right_node->numF = Female;  ////we will change node with it's parent so number of male and female will change
            right_node->numM = Male;
            right_node->parent = parent_node;
            parent_node->right = right_node;
        }else{  //if n is attached to it's parent from left
            right_node->numF = Female;
            right_node->numM = Male;
            right_node->parent = parent_node;
            parent_node->left = right_node;
        }
    }
    n->numM = Male-rightMale+lMale; //new nodes male number will be it's initial male number - it's right tree's male number + it's right->left tree's male number
    n->numF = Female-rightFemale+lFemale;
    right_node->left = n;   //7 will be right child of 10
    n->parent = right_node;  //7's parent is 10
}

void RBTree::rotateRight( Node* n){    //based on lecture slides (rotate 18 right)
    Node*  left_node = n->left;  //10 is left child
    Node* parent_node  = n->parent; //7 is parent
    int leftMale = left_node->numM;
    int leftFemale = left_node->numF;
    int Male = n->numM;
    int Female = n->numF;
    int rMale = 0;
    int rFemale = 0;
    if(left_node->right !=NULL){ //11 is not null
        rMale = left_node->right->numM;
        rFemale = left_node->right->numF;
        left_node->right->parent = n;    //11's parent is 18 now
        n->left=left_node->right; //18's left child is 11 now
    }else{
        n->left = NULL;
    }
    if(n->parent == NULL){
            left_node->numF = Female;
            left_node->numM = Male;
            left_node->parent = NULL;
            root = left_node;
    }else{
        if(parent_node->right == n){  //18 is attached to 7 from right
            left_node->numF = Female;
            left_node->numM = Male;
            left_node->parent = parent_node;  //10's parent is 7 now
            parent_node->right = left_node;  // 7's right child will be 10 now
        }else{
            left_node->parent = parent_node;
            parent_node->left = left_node;
        }
    }
    n->numM = Male-leftMale+rMale;
    n->numF = Female-leftFemale+rFemale;
    left_node->right = n;  //10's right child will be 18
    n->parent = left_node;  //18's parent is 10 now
}

void RBTree::makeRBT(Node* n){
    if(root == n){
        n->color = 'b';
        return;
    }
    while( n->parent !=NULL && n->parent->color =='r'){
        Node* Gparent_n = n->parent->parent;
        Node*uncle;
        if(Gparent_n->left == n->parent){ //if parent is left child of grandparent
            uncle = Gparent_n->right; //uncle is right child
            if(uncle !=NULL && uncle->color =='r'){ //if uncle is red --> recolor              **CASE 1**
                n->parent->color = 'b';   //parent and uncle black
                uncle->color = 'b';
                Gparent_n->color = 'r';  //grandparent red
                n = Gparent_n;  //keep going from grandparent node
            }else{  //if there is no uncle it means null which is black already                      **CASE 2**
                if(n->parent->right == n){  //if n is right child            ** parent is left child of g.parent and n is right child of parent
                    n = n->parent;    //left rotate parent and make it case 3
                    rotateLeft(n);
                }
                n->parent->color = 'b';  //  change colors of parent and g.parent                   **CASE 3 **
                Gparent_n->color='r';
                rotateRight(Gparent_n);  //right rotate grandparent
            }
        }else{  //if parent is right child of grandparent
            uncle = Gparent_n->left;  //uncle is left child
            if(uncle!=NULL && uncle->color == 'r'){
                n->parent->color = 'b';                                                     //                               **CASE 1**
                uncle->color = 'b';                                                             //                                  recolor
                Gparent_n->color = 'r';
                n = Gparent_n;
            }else{                                                                                        //                              **CASE 2**
                if(n->parent->left == n){                                                 //                              rotate parent and make it case 3
                    n = n->parent;
                    rotateRight(n);
                }
                n->parent->color = 'b';                                                     //                             **CASE 3**
                Gparent_n->color='r';                                                      //                              change color of g.parent and parent and rotate g.parent
                rotateLeft(Gparent_n);
            }
        }
    }
     root->color = 'b';
}

void printInorder(Node* node, int level){  //I could not print box character they are not working on windows and ssh.
    level++;  //increase level of nodes
     if (node == NULL)
          return;
     printInorder(node->left, level);
     for(int i=0; i<level; i++){  //put space enough for ever level
        cout << "\t";
     }
     if(node->parent !=NULL && node == node->parent->right){
        cout << "|___";
     }else if(node->parent!=NULL && node == node->parent->left){
        cout << "|***";
     }
     cout << "(" << node->color << ") " << node->name << " " << node->gender << " " << node->age << endl;
     printInorder(node->right, level);
     level--; //after printing left and right nodes decrease level of nodes
}

/*Hocam nTh female'yi neye göre arayacagımızı belirtmemissiniz hangi siradaki kacinci kadini ariyoruz? Bilmedigim icin ben önce soldaki kadinlari aradim sonra sagdaki en son kökteki.*/
Node* nThFemale(Node *n, int x){
    int lF=0;
    int rF=0;
    int F = n->numF;
    if(n->left){
        lF = n->left->numF;
    }
    if(n->right){
       rF = n->right->numF;
    }
    if (F == x && n->gender == 'F'){
        return n;
    }else if(lF>=x){
        return nThFemale(n->left, x); //if there are more female in the left side
    }
    else{
        return nThFemale(n->right, x-lF); //since every node contains leftFemale+rightFemale+Itself we will search x-leftFemale in the right side.
    }
}
Node* nThMale(Node *n, int x){
    int lM=0;
    int rM=0;
    int M = n->numM;
    if(n->left){
        lM = n->left->numM;
    }
    if(n->right){
       rM = n->right->numM;
    }
    if (M == x && n->gender == 'M'){
        return n;
    }else if(lM>=x){
        return nThMale(n->left, x);
    }
    else{
        return nThMale(n->right, x-lM);
    }
}

int main(int argc, char* argv[])
{
    RBTree tree;
    tree.readFile(argv[1]);
    cout <<endl;
    printInorder(tree.root, -1);
    Node *Female1 = nThFemale(tree.root, 1);
    Node *Male1 = nThMale(tree.root, 1);
    cout << endl << "1st Female: " << Female1->name << "\t 1st Male: " << Male1->name << endl;

    Node *Male2 = nThMale(tree.root, 2);
    Node *Female2 = nThFemale(tree.root, 2);
    cout << endl << "2nd Female: " << Female2->name << "\t 2nd Male: "<< Male2->name<< endl;

    Node *Male3 = nThMale(tree.root,3);
    Node *Female3 = nThFemale(tree.root, 3);
    cout << endl << "3rd Female: " << Female3->name << "\t 3rd Male: "<< Male3->name <<endl;

    Node *Male4 = nThMale(tree.root, 4);
    Node *Female4 = nThFemale(tree.root, 4);
    cout << endl << "4th Female: " << Female4->name << "\t 4th Male: "<< Male4->name << endl;

    Node *Female5 = nThFemale(tree.root, 5);
    cout << endl << "5th Female: " << Female5->name << endl;

  return 0;
}
