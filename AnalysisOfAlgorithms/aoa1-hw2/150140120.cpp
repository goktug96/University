#include <iostream>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include "population.h"

using namespace std;

int main(int argc, char* argv[])
{
    int N = atoi(argv[1]); //take argument
    PopManager pop1;
    pop1.readFile(N);  //read records
    clock_t begin = clock();      //set clock
    pop1.Quicksort(0, N-1);  //sort
    clock_t end = clock();   //end clock
    double elapsed_secs = double(end-begin)/CLOCKS_PER_SEC;
    cout << "Elapsed time: "<< elapsed_secs<< " secs"<< endl;  //elapsed time
    pop1.writeFile();  //write to file

    return 0;
}
