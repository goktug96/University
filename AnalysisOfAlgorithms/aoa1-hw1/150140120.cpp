#include <iostream>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include "card.h"
using namespace std;


int main(int argc, char* argv[])
{
    CardManager deck;
    if(strcmp(argv[1],"-full")==0){
        deck.readFile(argv[3], deck);
        deck.FullSort(argv[1], argv[2], deck);
        deck.writeFile(argv[4]);

    }else if(strcmp(argv[1],"-filter")==0){
        deck.readFile(argv[3], deck);
        deck.FilterSort(argv[1], argv[2], deck);
        deck.writeFile(argv[4]);
    }
    return 0;
}
