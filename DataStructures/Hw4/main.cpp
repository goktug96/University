#include <iostream>
#include "stack.h"

using namespace std;

int main(int argc, char*argv[]){
    if(argc !=2){
        cout << "Usage from command line:" << endl;
        cout << ">>" << argv[0] << " infix_expression" << endl;
        return 0;
    }

    char *infix = argv[1];
    cout << "Infix string: " << infix << endl << endl;
    char postfix[100];
    int postfixindex = 0;
    struct Stack operatorstack;
    operatorstack.create();
    for(int i = 0; infix[i]!='\0'; i++){
        if(infix[i] == '('){
            operatorstack.push(infix[i]);
           }
        else if(infix[i]=='*'){
            operatorstack.push(infix[i]);
        }
        else if(infix[i] == '/'){
            if(!operatorstack.isEmpty()){
                while(operatorstack.getHeadChar() == '*'){
                    postfix[postfixindex] = operatorstack.pop();
                    postfixindex++;
                    if(operatorstack.isEmpty()) break;
                }
                operatorstack.push(infix[i]);
            }
        }
        else if(infix[i] == '+'){
            if(!operatorstack.isEmpty()){
                while(operatorstack.getHeadChar()== '*'||operatorstack.getHeadChar()== '/'){
                    postfix[postfixindex] = operatorstack.pop();
                    postfixindex++;
                    if(operatorstack.isEmpty()) break;
                }
                operatorstack.push(infix[i]);
            }
        }
        else if(infix[i] == '-'){
            while(operatorstack.getHeadChar()== '*'||operatorstack.getHeadChar()== '/'||operatorstack.getHeadChar()== '+'){
                if(!operatorstack.isEmpty()){
                while(operatorstack.getHeadChar()== '*'||operatorstack.getHeadChar()== '/'){
                    postfix[postfixindex] = operatorstack.pop();
                    postfixindex++;
                    if(operatorstack.isEmpty()) break;
                }
                operatorstack.push(infix[i]);
                }
            }
        }
        else if(infix[i]== ')'){
            while(operatorstack.getHeadChar() != '('){
                postfix[postfixindex] = operatorstack.pop();
                postfixindex++;
            }
            char buffer = operatorstack.pop();
        }
        else{
            postfix[postfixindex]=infix[i];
            postfixindex++;
        }
        postfix[postfixindex] = '\0';
        cout << "Postfix string: " << postfix << "\t";
        operatorstack.print();
    }

    while(!operatorstack.isEmpty()){
    postfix[postfixindex] = operatorstack.pop();
    postfixindex++;
    }

    postfix[postfixindex] = '\0';
    cout << endl << "Postfix string: " << postfix << endl;

    return 0;
}
