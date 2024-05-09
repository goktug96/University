#ifndef NODE
#define NODE

class Node{
    friend class ARTF;
    friend class ARTL;
    friend class FAFR;
    friend class FALR;
    friend class LAFR;
    friend class LALR;
    friend class AdderRemover;
    Node *next;
    Node *prev;
public:
    int data;
    Node(){next = NULL; prev = NULL;} //constructor, set next and prev to NULL
};
#endif // NODE
