#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>
#include <climits>
#include "stack-queue.h"

using namespace std;


bool isOperator(char *c){
    if(strcmp(c, "sum")==0 || strcmp(c, "sub")==0 || strcmp(c, "product")==0 || strcmp(c, "div")==0 || strcmp(c, "exp")==0
    || strcmp(c, "sqrt")==0 || strcmp(c, "max")==0 || strcmp(c, "min")==0 || strcmp(c, "abs")==0 || strcmp(c, "mod")==0
    || strcmp(c, "log")==0 || strcmp(c, "mod")==0 ){
        return true;
    }
    else return false;
}

Stack s;
Queue q;
long int inf = LONG_MAX;
long int ninf = LONG_MIN;
int flag = 0;

long int compute(char*);
char* translate(char*);

int main(int argc, char *argv[]){
    s.create(); //create queue and stack
    q.create();
    while(1){
        if (argc==2){ //file operations
            FILE *fptr;
            fptr = fopen(argv[1], "r");
            if(!fptr){
                if(!(fptr = fopen( argv[1], "r" ))){
                cerr << "Cannot open file" << endl;
                exit(1);
                }
            }
            while(!feof(fptr)){
                char* prefix = new char[100];
                char postfix[100];
                prefix = fgets(prefix, 100, fptr); //take first line
                prefix = strtok(prefix, "\n"); //delete newline character
                strcpy(postfix, translate(prefix)); //translate prefix and copy to postfix
                compute(postfix); //compute postfix
                delete [] prefix;
            }
        cout << "Answer Queue: "; //write answer queue
        char *data;
        while (!q.isempty()){
            data = q.dequeue();
            if (atol(data)==inf){
                cout << "inf";
            }else if(atol(data)==ninf){
                cout << "-inf";
            }else{
                cout << data;
            }
            if(!q.isempty())
                cout << " -> ";
        }
        getchar();
        cout << endl;
        break;
        }else {  //console operations
            cout << "Initializing the interpreter..." << endl << endl;
            char* prefix = new char[100];
            char postfix[100];
            //char* postfix = new char[100];
            cout << ">> ";
            cin.getline(prefix, 100); //take prefix
            if(strcmp(prefix, "exit") == 0 ){ //if exit: print answer queue
                cout << "Answer Queue: ";
                char *data;
                while (!q.isempty()){
                    data = q.dequeue();
                    if (atol(data)==inf){
                        cout << "inf";
                    }else if(atol(data)==ninf){
                        cout << "-inf";
                    }else{
                        cout << data;
                    }
                    if(!q.isempty())
                        cout << " -> ";
                }
                cout << endl;
                getchar();
                break;
            }
            long int ans;
            strcpy(postfix, translate(prefix)); //translate and copy to postfix
            cout << endl;
            cout << "::: Postfix String: "<< postfix << endl << endl; //print postfix
            ans = compute(postfix); //compute
            if(flag !=0){
                if(ans == inf){
                    cout << "::: Answer: inf" << endl << endl; //if answer is inf print inf (global value LONG_MAX)
                }else if(ans == ninf){
                    cout << "::: Answer: -inf" << endl << endl; //else if answer is -inf print ninf (global value LONG_MIN)
                }else{
                    cout << "::: Answer: " << ans << endl << endl; //else print answer
                }
            }
            delete [] prefix;
        }
    }

    return 0;
}
char* translate(char *prefix){
    char *postfix=new char[100]();
    char* taken;
    char* top;
    taken = strtok(prefix, " ");
        while(taken!=NULL){
            if(isOperator(taken)|| strcmp(taken, "(")==0){  //if we got operator or "(" push it to stack
                s.push(taken);
            }
            else if(strcmp(taken, ")")==0){  //when we have ')' pop until '(' and add them to postfix string
                while(!s.isEmpty()){
                    top = s.pop();
                    if(strcmp(top, "(")==0) break; //we popped '(', too and break it so no need to pop again.
                    strcat(postfix, top);
                    strcat(postfix, " ");
                }
            }
            else{
                strcat(postfix, taken); //else if operand add it to postfix string.
                strcat(postfix, " ");
            }
            taken = strtok(NULL, " "); //take next data
        }
        while(s.head != NULL){
            top = s.pop();   //after loop make empty to stack and add them to postfix string
            if(strcmp(top, "(")!=0){
                strcat(postfix, top);
                strcat(postfix, " ");
            }
        }
        return postfix;

}

long int compute(char* pfx){
    char *taken;
    char *top;
    char *postfix = new char[100];
    strcpy(postfix, pfx);
    taken = strtok(postfix, " ");
    long int ans;
    long int i;
    long int j;
    while(taken != NULL){
        flag = 0;
        ans = 0;
        if(!isOperator(taken) && strcmp(taken, "ans")!=0 && strcmp(taken, "inf")!=0 && strcmp(taken, "-inf")!=0){
            while(!isOperator(taken)){ //check if taken is operant or not, if we have inf, -inf or ans we should break.
                if((strcmp(taken, "ans")==0) || (strcmp(taken, "inf")==0) || (strcmp(taken, "-inf")==0)) break;
                s.push(taken); // push operand to stack
                taken = strtok(NULL, " ");
            }
        }
        else if(strcmp(taken, "sum")==0){
            flag = 1;
            int counter = 0;
            taken = strtok(NULL, " ");
            while(!s.isEmpty()){
                top =  s.pop();
                i=atol(top);
                counter++;
                if(i==inf){ //inf + x = inf
                    ans = inf;
                    while(!s.isEmpty()){ //if i = inf make scan the stack unless there is -inf, ans = inf
                        top =  s.pop();
                        j=atol(top);
                        if(j==ninf){ //inf + -inf error.
                            counter++;
                            flag=0;
                            break;
                        }
                        counter ++;
                    }
                }
                else if(i==ninf){ //same for -inf...
                    ans = ninf;
                    while(!s.isEmpty()){
                        top =  s.pop();
                        j=atol(top);
                        if(j==inf){
                            counter++;
                            flag=0;
                            break;
                        }
                        counter ++;
                    }
                }
                else{
                    ans = ans + i;
                }
            }
            if (counter<2){ // such as sum 2, not enough operand, Error
                flag = 0;
                break;
            }
        }
        else if(strcmp(taken, "sub")==0){
            flag = 1;
            taken = strtok(NULL, " ");
            top = s.pop();
            i=atol(top);
            if(s.isEmpty()){ //we took first data if there is no more data then there are not enough operands, Error.
                flag = 0;
                break;
            }
            top = s.pop();
            j=atol(top);
            if(i==inf && j==inf){ //inf-inf, undetermined
                flag=0;
                break;
            }
            if((i==inf && j!=inf) || (j==ninf && i!=inf)){ //x-inf or -inf-x = -inf
                ans = ninf;
                break;
            }else if((j==inf && i!=inf) || (j!=inf && i==ninf)){ //inf-x or x-(-inf) = inf
                ans = inf;
                break;
            }
            ans = j-i;
        }
        else if(strcmp(taken, "product")==0){
            flag = 1;
            ans = 1;
            int counter = 0;
            taken = strtok(NULL, " ");
            while(!s.isEmpty()){
                top =  s.pop();
                counter++;
                i=atol(top);
                if(i==inf){ //if i == inf, check if ans has changed before, (if postfix: -2 inf, then first answer will be -2)
                    if(ans<0){ //if ans<0  and i = inf then answer will be -inf
                        ans = ninf;
                    }else{ // if ans >0 and i = inf then answer will be inf
                        ans = inf;
                    }
                    while(!s.isEmpty()){ //if answer=inf, scan the stack now
                        top=s.pop();
                        counter++;
                        i=atol(top);
                        if(ans == inf && i<0 && i!=ninf){ // if there is data in stack which is <0 and ans=inf, ans should be -inf
                            ans = ninf;
                        }else if(ans == ninf && i<0 && i!=ninf){ //if there is data in stack which is <0 and ans=-inf, ans should be inf
                            ans = inf;
                        }
                    }
                    break; //break the loop we made empty the stack already.
                }else if(i==ninf){  // same things for -inf..
                    if(ans<0){
                        ans = inf;
                    }else{
                        ans = ninf;
                    }
                    while(!s.isEmpty()){
                        top=s.pop();
                        counter++;
                        i=atol(top);
                        if(ans == inf && i<0 && i!=ninf){
                            ans = ninf;
                        }else if(ans == ninf && i<0 && i!=ninf){
                            ans = inf;
                        }
                    }
                    break;
                }
                ans = ans * i;
                counter++;
            }
            if (counter<2){
                cout << "Not enough operand -";
                flag = 0;
                break;
            }
        }
        else if(strcmp(taken, "div")==0){
            flag = 1;
            taken = strtok(NULL, " ");
            top = s.pop();
            i=atol(top);
            if (i==0){ //division to 0 error.
                flag = 0;
                break;
            }
            if(s.isEmpty()){ //check if not enough operand
                flag = 0;
                break;
            }
            top = s.pop();
            j=atol(top);
            if((i==inf && j == inf) || (i==inf && j == ninf) || (i==ninf && j == ninf) || (i==ninf && j==inf)){
                // inf/inf , inf/-inf, -inf/-inf, -inf/ inf undetermined.
                flag = 0;
                break;
            }else if((j==inf && i>0) || (j==ninf && i<0) ){
                ans = inf;
                break;
            }else if ((j==ninf && i>0) || (j==inf && i<0)){
                ans = ninf;
                break;
            }else{
                ans = floor(j/i);
            }
        }
        else if(strcmp(taken, "exp")==0){
            flag = 1;
            taken = strtok(NULL, " ");
            top = s.pop();
            i=atol(top);
            if(s.isEmpty()){ //unary operations
                ans = floor(exp(i));
            }else{ //binary operations
                top = s.pop();
                j=atol(top);
                if((i==0 && j == 0) || (i==inf && j == 0) || (j==1 && i==inf)){
                    // 0^0, 0^inf, 1^inf undetermined
                    flag = 0;
                    break;
                }
                ans = floor(pow(j, i));

            }
        }
        else if(strcmp(taken, "log")==0){
            flag = 1;
            taken = strtok(NULL, " ");
            top = s.pop();
            i=atol(top);
            if(i<0){
                flag = 0; //log(-x), Error
                break;
            }
            if(s.isEmpty()){  //unary operations
                ans = floor(log(i));
            }else{  //binary operations

                top = s.pop();
                j=atol(top);
                if(j<0){
                    flag = 0;
                    break;
                }
                ans = floor(log(i)/log(j));
            }
        }
        else if(strcmp(taken, "mod")==0){
            flag = 1;
            taken = strtok(NULL, " ");
            top = s.pop();
            i=atol(top);
            if(s.isEmpty()){
                ans = i % 10; //unary
            }else{

                top = s.pop();  //binary
                j=atol(top);
                ans = i%j;
            }
        }
        else if(strcmp(taken, "abs")==0){
            flag = 1;
            taken = strtok(NULL, " ");
            if(s.isEmpty()) break;
            top = s.pop();
            i=atol(top);
            ans = abs(i);
        }
        else if(strcmp(taken, "sqrt")==0){
            flag = 1;
            taken = strtok(NULL, " ");
            if(s.isEmpty()) break;
            top = s.pop();
            i=atol(top);
            if (i<0){ // sqrt -4, Error
                flag = 0;
                break;
            }
            ans = sqrt(i);
        }
        else if (strcmp(taken, "min")==0){
            flag = 1;
            taken = strtok(NULL, " ");
            ans = inf; //make answer the inf, if there are values less than inf we'll change it
            while(!s.isEmpty()){
                top = s.pop();
                i=atol(top); //store taken values in a array, actually  int would be enough, I am not sure why I did this
                if ( i < ans){
                    ans = i;
                }
            }
        }
        else if (strcmp(taken, "max")==0){ //same as min
            flag = 1;
            taken = strtok(NULL, " ");
            ans = ninf;
            while(!s.isEmpty()){
                top =  s.pop();
                i=atol(top);
                if ( i > ans){
                    ans = i;
                }
            }
        }
        else if(strcmp(taken, "ans")==0){
            top = q.backnode();  //if we got "ans" we 'll take last element of the queue
            strcpy(taken, top); // and make it taken;
        }
        else if(strcmp(taken, "inf")==0){ //if we got taken;
            taken = strtok(NULL, " ");
            char buffer[30];
            sprintf(buffer, "%ld", inf); //translate inf (which is a global value = LONG_MAX) to char* to push it to stack.
            s.push(buffer); //translate inf (which is a global value = LONG_MAX) to char* to push it to stack.
        }
        else if(strcmp(taken, "-inf")==0){ //same as inf
            taken = strtok(NULL, " ");
            char buffer[30];
            sprintf(buffer, "%ld", ninf);
            s.push(buffer);
        }
        if(flag!=0){  //if an operation is made I change flag value to one, (flag is 0 at the beginning)
            char buffer[30]; //so, we should push the answer of the operation to stack, translate it to char*
            sprintf(buffer, "%ld", ans);
            s.push(buffer);
        }
    }
    if(flag == 0){  //if there is error or no operation has done flag is 0;
        cout << "::: Error." << endl << endl;
    }
    char buffer[30];
    sprintf(buffer, "%ld", ans);
    q.enqueue(buffer);
    s.pop(); //last answer is in the stack, pop it and make stack empty.
    delete[]postfix;
    return ans;
}
