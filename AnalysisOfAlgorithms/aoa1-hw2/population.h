#include <iostream>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <limits.h>
using namespace std;

class PopManager;
char *strtok_single (char * str, char const * delims);

class Population{  //population class
    char pop[10];   //I made all variables char because defining null value to an integer gives zero, I want empty values for null.
    char min_Age[3];
    char max_Age[3];
    char gender[10];
    char zipCode[10];
    char geoID[15];
    friend PopManager;  //friend class
};

class PopManager{
    vector <Population*> v; //vector for holding datas
public:
    PopManager(){}; //default constructor
    void print(); //print function -not used in code but used for testing-
    void addPop(Population*);  //add new population
    void Quicksort(int p, int r);  //sort
    int partition(int, int);
    void readFile(int);
    void writeFile();
};

int PopManager::partition(int p, int r){ //partition function for quick-sort
    Population* ch;
    int i = p-1;
    for(int j=p; j<r; j++){
        if(atoi(v[j]->pop)<atoi(v[r]->pop)){  //I chose last element as pivot, if population is smaller than population of pivot
            i=i+1;
            ch = v[i];  //exchange  v[i] ↔ v[j]
            v[i] =  v[j];
            v[j] = ch;
        }else if( (atoi(v[j]->pop)==atoi(v[r]->pop)) && (strcmp(v[j]->geoID, v[r]->geoID)<=0) ){  //if population numbers are same, sort by geoID
            i=i+1;
            ch = v[i];  //exchange  v[i] ↔ v[j]
            v[i] =  v[j];
            v[j] = ch;
        }
    }
    ch = v[i+1];  //exchange vi+1] ↔ v[r]
    v[i+1] = v[r];
    v[r] = ch;
    return i+1;
}

void PopManager::Quicksort(int p, int r){ //quick sort algorithm
    if(p<r){
        int q = partition(p,r);
        Quicksort(p,q-1);
        Quicksort(q+1,r);
    }
}

void PopManager::print(){  //print function
    for(int i=0; i<v.size(); i++){
        cout << v[i]->pop << "," << v[i]->min_Age << "," << v[i]->max_Age << "," << v[i]->gender << "," <<
        v[i]->zipCode << "," << v[i]->geoID << endl;
    }
 }


void PopManager::addPop(Population* obj){  //adding new population
    v.push_back(obj);
}


void PopManager::readFile(int N){  //read file function
    FILE *fptr;
    fptr = fopen("population_by_zip_2010.csv", "r");
    char taken[150];
    fgets(taken, 150, fptr); //take first line, redundant
    const char* part;
    for(int i=0; i<N; i++){   //take first N elements, N from argument
        fgets(taken, 150, fptr);  //take next line
        if(feof(fptr)){
            break;
        }
        Population* population = new Population;  //read values to the population and add it to vector
        part = strtok_single(taken, ",");  //strtok_single function: user written function that considers empty inputs
        strcpy(population->pop, part);
        part = strtok_single(NULL, ",");
        strcpy(population->min_Age, part);
        part = strtok_single(NULL, ",");
        strcpy(population->max_Age, part);
        part = strtok_single(NULL, ",");
        strcpy(population->gender, part);
        part = strtok_single(NULL, ",");
        strcpy(population->zipCode, part);
        part = strtok_single(NULL, "\n");
        strcpy(population->geoID, part);
        addPop(population);
    }
    fclose(fptr);

}
void PopManager::writeFile(){
    FILE* fptr;
    fptr = fopen("deneme.csv", "w+");
    for(int i=0; i<v.size(); i++){
        fprintf(fptr, "%s,%s,%s,%s,%s,%s\n", v[i]->pop, v[i]->min_Age, v[i]->max_Age, v[i]->gender, v[i]->zipCode, v[i]->geoID);
        delete v[i]; //delete populations after writing to file
    }
    fclose(fptr);

}

// I took this function from https://stackoverflow.com/questions/8705844/need-to-know-when-no-data-appears-between-two-token-separators-using-strtok

char *strtok_single (char * str, char const * delims)  //strtok function does not consider empty data, this function has written to fix that situation
{
    static char* src = NULL;
    char* p, * ret = 0;

    if (str != NULL)
        src = str;

    if (src == NULL)
        return NULL;

    p = strpbrk (src, delims);
    if ((p = strpbrk (src, delims)) != NULL) {
        *p  = 0;
        ret = src;
        src = ++p;

    } else if (*src) {
        ret = src;
        src = NULL;
    }
    return ret;
}

