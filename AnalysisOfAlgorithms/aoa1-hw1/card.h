#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include <iostream>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <limits.h>
using namespace std;

class CardManager;

class Card{
    char name[30];
    char cname[30];
    char rarity[20];
    char setName[20];
    char Type[20];
    int cost;
    friend CardManager;
};

class CardManager{
    vector <Card*> v;
public:
    CardManager(){};
    void FullSort(char*, char*, CardManager&);
    void FilterSort(char*, char*, CardManager& );
    void print();
    void addCard(Card*);
    void InsertionSort(char *type);
    void merge(char*type, int, int, int);
    void mergeSort(char*, int, int);
    void readFile(char*, CardManager&);
    void writeFile(char*);
};

void CardManager::print(){
    for(int i=0; i<v.size(); i++){
        cout << v[i]->name << " " << v[i]->cname << " " << v[i]->rarity << " " << v[i]->setName << " " <<
        v[i]->Type << " " << v[i]->cost << endl;
    }
 }

 void CardManager::FullSort(char* type, char* sorting, CardManager& deck){
    if(strcmp(sorting, "-i") == 0){
        clock_t begin = clock();
        deck.InsertionSort(type);
        clock_t end = clock();
        double elapsed_secs = double(end-begin)/CLOCKS_PER_SEC;
        cout << "Elapsed time: "<< elapsed_secs<< " secs"<< endl;
    }else if(strcmp(sorting, "-m")==0){
        clock_t begin = clock();
        deck.mergeSort(type, 1, deck.v.size());
        clock_t end = clock();
        double elapsed_secs = double(end-begin)/CLOCKS_PER_SEC;
        cout << "Elapsed time: "<< elapsed_secs<< " secs"<< endl;
    }
 }

void CardManager::FilterSort(char* type, char* sorting, CardManager& deck){
    if(strcmp(sorting, "-i") == 0){
        clock_t begin = clock();
        deck.InsertionSort(type);
        clock_t end = clock();
        double elapsed_secs = double(end-begin)/CLOCKS_PER_SEC;
        cout << "Elapsed time: "<< elapsed_secs<< " secs"<< endl;
    }else if(strcmp(sorting, "-m")==0){
        clock_t begin = clock();
        deck.mergeSort(type, 1, deck.v.size());
        clock_t end = clock();
        double elapsed_secs = double(end-begin)/CLOCKS_PER_SEC;
        cout << "Elapsed time: "<< elapsed_secs<< " secs"<< endl;
    }
}
void CardManager::addCard(Card* obj){
    v.push_back(obj);
}

void CardManager::mergeSort(char* type, int first, int last){
    if(first < last){
        int mid = floor((first+last)/2);
        mergeSort(type, first, mid);
        mergeSort(type, mid+1, last);
        merge(type, first, mid, last);
    }
}
void CardManager::merge(char* type, int first, int mid, int last){
    Card* last_card = new Card;
    strcpy(last_card->cname, "ZzzzLast");
    strcpy(last_card->name, "ZzzzzLast");
    strcpy(last_card->Type, "ZzzzzzLast");
    last_card->cost = INT_MAX;
    int first_half = mid - first + 1;
    int second_half = last - mid;
    vector<Card*>L; vector<Card*>R;
    for(int i=0; i<first_half; i++){
        L.push_back(v[first + i - 1]);
    }
    for(int j=0; j<second_half; j++){
        R.push_back(v[mid+j]);
    }
    L.push_back(last_card);
    R.push_back(last_card);
    int i=0; int j=0;
    if(strcmp(type, "-full")==0){
        for (int k=first-1; k<last; k++){
            if(strcmp(L[i]->cname, R[j]->cname)<0){
                v[k]=L[i];
                i++;
            }else if(strcmp(L[i]->cname, R[j]->cname)>0){
                v[k]=R[j];
                j++;
            }else{
                if(L[i]->cost < R[j]->cost){
                    v[k]=L[i];
                    i++;
                }else if(L[i]->cost > R[j]->cost){
                    v[k]=R[j];
                    j++;
                }else{
                    if(strcmp(L[i]->name, R[j]->name)<=0){
                        v[k]=L[i];
                        i++;
                    }else{
                        v[k]=R[j];
                        j++;
                    }
                }
            }
        }
    }else if(strcmp(type, "-filter")==0){
        for (int k=first-1; k<last; k++){
            if(strcmp(L[i]->Type, R[j]->Type)<=0){
                v[k]=L[i];
                i++;
            }else {
                v[k]=R[j];
                j++;
            }
        }
    }

    delete last_card;


}

void CardManager::InsertionSort(char* type){
    Card* sample = new Card;
    if(strcmp(type, "-full") == 0){
        for(int i=1; i<v.size(); i++){
            sample = v[i];
            int j,k,l;
            for(j=i-1; j>=0 && strcmp(sample->cname, v[j]->cname) < 0; j--){
                v[j+1] = v[j];
            }
            j++;

            for(k=j-1; k>=0 && strcmp(sample->cname, v[k]->cname)==0 && sample->cost < v[k]->cost; k--){
                v[k+1] = v[k];
            }
            k++;

            for(l=k-1; l>=0 && strcmp(sample->cname, v[l]->cname)==0 && sample->cost == v[l]->cost && strcmp(sample->name, v[l]->name) < 0; l--){
                v[l+1] = v[l];
            }
            v[l+1]=sample;
        }
    }else if(strcmp(type, "-filter")==0){
        for(int i=1; i<v.size(); i++){
            sample = v[i];
            int j;
            for(j=i-1; j>=0 && strcmp(sample->Type, v[j]->Type) < 0; j--){
                v[j+1] = v[j];
            }
            v[j+1] = sample;
        }
    }
    delete sample;
}

void CardManager::readFile(char* file_name, CardManager &deck){
    FILE *fptr;
    fptr = fopen(file_name, "r");
    char taken[150];
    const char* part;
    while(!feof(fptr)){
        fgets(taken, 150, fptr);
        if(feof(fptr)){
            break;
        }
        Card* card = new Card;
        part = strtok(taken, "\t");
        strcpy(card->name, part);
        part = strtok(NULL, "\t");
        strcpy(card->cname, part);
        part = strtok(NULL, "\t");
        strcpy(card->rarity, part);
        part = strtok(NULL, "\t");
        strcpy(card->setName, part);
        part = strtok(NULL, "\t");
        strcpy(card->Type, part);
        part = strtok(NULL, "\t");
        card->cost = atoi(part);
        deck.addCard(card);
    }
    fclose(fptr);

}
void CardManager::writeFile(char* file_name){
    FILE* fptr;
    fptr = fopen(file_name, "w+");
    for(int i=0; i<v.size(); i++){
        fprintf(fptr, "%s\t%s\t%s\t%s\t%s\t%d\n", v[i]->name, v[i]->cname, v[i]->rarity, v[i]->setName, v[i]->Type, v[i]->cost);
        delete v[i];
    }
    fclose(fptr);

}


#endif // CARD_H_INCLUDED
