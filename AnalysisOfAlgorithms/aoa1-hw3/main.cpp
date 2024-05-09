#include <iostream>
#include <iomanip>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>


using namespace std;

class BookManager;

class Book{
        int pageNo;
        int lineNo;
        int indexNo;
        char c[5];
        friend BookManager;
};

class BookManager{
    private:
        Book** Hashtable;
        unsigned long int key;
        int collisions;  //number of collisions
        vector <Book*> v;  //vector for list operations

    public:
        BookManager();
        unsigned long int create_key(int, int, int);
        void lookupDict();
        void lookupList();
        void readForDict();
        void readForList();
        void writeFile();
        void insertList(Book*);
        void insertDict(Book*, int);
        int h1();  //hash function
        int h2( int); //probe function
        void finish();
        int getCol(){return collisions;}
};

// I took this function from https://stackoverflow.com/questions/8705844/need-to-know-when-no-data-appears-between-two-token-separators-using-strtok
char *strtok_single (char * str, char const * delims) ;  //declaration

BookManager::BookManager(){
    Hashtable = new Book*[131070];
    for(int i=0; i<131070; i++){  // initially make all cells of hash table NULL
        Hashtable[i] = NULL;
    }
    collisions = 0;  //initialize collisions
}

void BookManager::readForDict(){ //read file for dictionary operations, I read files 2 times for measuring runtime separately
    FILE *fptr;
    fptr = fopen("ds-set-input.txt", "r");
    char taken[150];
    const char* part;
    int n=0;  //n is defined to specify first 1000, 10000 and 100000 input data
   while(!feof(fptr)){
        fgets(taken, 150, fptr);  //get first line
        if(feof(fptr)){
            break;
        }
        Book* book = new Book;
        part = strtok_single(taken, "\t");  //parse line
        book->pageNo = atoi(part);
        part = strtok_single(NULL, "\t");
        book->lineNo = atoi(part) ;
        part = strtok_single(NULL, "\t");
        book->indexNo = atoi(part) ;
        part = strtok_single(NULL, "\n");
        strcpy(book->c, part);
        insertDict(book,n);  //insert book character to dictionary
        n++;  //increase n
    }
    fclose(fptr);
}

void BookManager::insertDict(Book* obj, int n){

        create_key(obj->pageNo, obj->lineNo, obj->indexNo);  //firstly create key from book's page no, line no, index no
        if(Hashtable[h1()] == NULL){ //if row is available, put object into row
            Hashtable[key] = obj;
        }else{   //if there is object in that row, call probing function until finding empty row
            collisions++;
            int i=1; //number for probing function
            while(Hashtable[h2(i)]!=NULL){
                collisions++; //increase number of collisions
                i++;
            }
            Hashtable[key] = obj;
        }
        if(n ==1000)  //write collision numbers
         cout << "Average number of collisions (first 1,000)         |   " << (double) collisions/1000 << endl;
        else if(n==10000)
        cout << "Average number of collisions (first 10,000)        |   " << (double)collisions/10000<< endl;
        else if(n==100000)
        cout << "Average number of collisions (first 100,000)       |   "<<(double)collisions/100000 << endl;
}
unsigned long int BookManager::create_key(int p, int l, int i){
        int digit1 = 0;
        int digit2 = 0;
        int x;
        x = i;
        while(x>0){  //find how many digits input no has and put it into digit1
            x = x/10;
            digit1 ++;
        }
        x=l;
        while(x>0){  //find how many digits line no has and put it into digit2
            x = x/10;
            digit2 ++;
        }
        for(int k=0; k<digit1; k++){ //multiply line no by 10, digit1 times
            l = l*10;
        }
        for(int k=0; k<digit1 + digit2; k++){ //multiply page  no by 10, digit2 times
            p = p*10;
        }
        key = p+l+i;  //add them all to create our key
        return key;  //return key
}

int BookManager::h1(){  //h function
    double A = (sqrt(5)-1)/2;
    key = floor(131071* fmod(key*A, 1));
    return key;
}

int BookManager::h2(int i){  //probe function
    key = (key + 7*i + 3*i*i)%131071;
    return key;
}

void BookManager::readForList(){  //read input file for list operations
    FILE *fptr;
    fptr = fopen("ds-set-input.txt", "r");
    char taken[150];
    const char* part;
    int n=0;
   while(!feof(fptr)){
        fgets(taken, 150, fptr);
        if(feof(fptr)){
            break;
        }
        Book* book = new Book;
        part = strtok_single(taken, "\t");
        book->pageNo = atoi(part);
        part = strtok_single(NULL, "\t");
        book->lineNo = atoi(part) ;
        part = strtok_single(NULL, "\t");
        book->indexNo = atoi(part) ;
        part = strtok_single(NULL, "\n");
        strcpy(book->c, part);
        insertList(book);  //insert book character to vector
    }
    fclose(fptr);
}

void BookManager::insertList(Book* obj){
        v.push_back(obj);  //insert
}


void BookManager::lookupDict(){  //lookup function for dictionary
    FILE *input;
    FILE *output;
    input = fopen("ds-set-lookup.txt", "r");
    output = fopen("ds-set-output-dict.txt", "w+");
    char taken[150];
    const char* part;
    while(!feof(input)){  //read file and get book character objects
        fgets(taken, 150, input);
        if(feof(input)){
            break;
        }
        Book* book = new Book;
        part = strtok_single(taken, "\t");
        book->pageNo = atoi(part);
        part = strtok_single(NULL, "\t");
        book->lineNo = atoi(part) ;
        part = strtok_single(NULL, "\n");
        book->indexNo = atoi(part) ;

        create_key(book->pageNo, book->lineNo, book->indexNo);  //get key
        h1(); //find place in hashtable

        int count=1;

        //until finding correct object use h2 (probing) function
        while((Hashtable[key]->pageNo != book->pageNo) || (Hashtable[key]->lineNo != book->lineNo) || (Hashtable[key]->indexNo != book->indexNo)){
            h2(count);
            count++;
        }

        fprintf(output, "%d\t%d\t%d\t%s\n", Hashtable[key]->pageNo, Hashtable[key]->lineNo, Hashtable[key]->indexNo, Hashtable[key]->c); //write to file
   }
   fclose(output);
   fclose(input);
}

void BookManager::lookupList(){  //look up for list
    FILE *input;
    FILE *output;
    input = fopen("ds-set-lookup.txt", "r");
    output = fopen("ds-set-output-list.txt", "w+");
    char taken[150];
    const char* part;
    while(!feof(input)){
        fgets(taken, 150, input);
        if(feof(input)){
            break;
        }
        Book* book = new Book;
        part = strtok_single(taken, "\t");
        book->pageNo = atoi(part);
        part = strtok_single(NULL, "\t");
        book->lineNo = atoi(part) ;
        part = strtok_single(NULL, "\n");
        book->indexNo = atoi(part) ;


        for(int i=0; i<131071; i++){  //search all list to find correct object and write it to file
            if((v[i]->pageNo == book->pageNo) && (v[i]->lineNo == book->lineNo) && (v[i]->indexNo == book->indexNo)){
                fprintf(output, "%d\t%d\t%d\t%s\n", v[i]->pageNo, v[i]->lineNo, v[i]->indexNo, v[i]->c);
                break;
            }
        }
    }
    fclose(output);
    fclose(input);
}

void BookManager::finish(){  //free allocated memory
    for(int i=0; i<13700; i++){
        delete Hashtable[i];
        delete v[i];
    }
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

int main()
{
    BookManager b;
    cout << "*****DICTIONARY*****"<<endl << endl;

    clock_t begin = clock();      //set clock
    b.readForDict();
    clock_t end = clock();   //end clock
    double elapsed_secs = double(end-begin)/CLOCKS_PER_SEC;
    cout << "Average number of collisions (overall)             |   "<<   (float)b.getCol()/137001 << endl << endl;
    cout << "Insertion finished after  " << elapsed_secs << "seconds" << endl << endl;

    clock_t begin2 = clock();      //set clock
    b.lookupDict();
    clock_t end2 = clock();   //end clock
    double elapsed_secs2 = double(end2-begin2)/CLOCKS_PER_SEC;
    cout << "Lookup finished after  " << elapsed_secs2 << "seconds" << endl << endl;

    cout << "*****LIST*****" << endl<< endl;

    clock_t begin3 = clock();      //set clock
    b.readForList();
    clock_t end3 = clock();   //end clock
    double elapsed_secs3 = double(end3-begin3)/CLOCKS_PER_SEC;
    cout << "Insertion finished after  " << elapsed_secs3 << "seconds" << endl << endl;

    clock_t begin4 = clock();      //set clock
    b.lookupList();
    clock_t end4 = clock();   //end clock
    double elapsed_secs4 = double(end4-begin4)/CLOCKS_PER_SEC;
    cout << "Lookup finished after  " << elapsed_secs4 << "seconds" << endl << endl;

    b.finish();

    return 0;
}
