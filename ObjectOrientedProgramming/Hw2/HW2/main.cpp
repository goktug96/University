#include <iostream>
#include <string.h>
#include "AdderRemover.h"

using namespace std;

int main()
{
    AdderRemover *myList[4];
    myList[0] = new FAFR;
    myList[1] = new LALR;
    myList[2] = new FALR;
    myList[3] = new LAFR;
    for (int i=0; i<4; i++){
        for (int j=0; j<5; j++){
            myList[i]->add(j);
        }
    }
    for(int i=0; i<4; i++){
        myList[i]->remove();
        myList[i]->display();
        myList[i]->removeAll();
        myList[i]->display();
    }
    cout << "Test Traverser: " << endl;
    for(int i=0; i<4; i++){
        for (int j=0; j<5; j++){
            myList[i]->add(j);
        }
        myList[i]->remove();
    }
    for (int i=0; i<4; i++){
        myList[i]->setTraverser();
        myList[i]->traverse();
    }
    return 0;
}
