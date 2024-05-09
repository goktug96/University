/*
    Ahmet Göktuð SEVÝNÇ
    150140120
*/
#ifndef COOKIE_H_INCLUDED
#define COOKIE_H_INCLUDED

#include <string.h>
#include <stdlib.h>
using namespace std;

class Cookie{
    string type;
    string name;
    int number;
    float priceperdozen;

public:
    Cookie(){type = "Cookies";} //default constructor
    Cookie(const char* , int, float); //constructor
    void print()const;
    int getNumber(){return number;}
    int getPrice()const{return number*(priceperdozen/12);} //price calculation
    string getType(){return type;}
    string getName(){return name;}
};

Cookie::Cookie(const char* obj, int n, float ppd){
    type = "Cookies";
    name = obj;
    number = n;
    priceperdozen = ppd;
}

void Cookie::print()const{
    cout << name << " #" << number << " Cost: " << getPrice()  << endl;
}


ostream& operator <<(ostream& out, const Cookie& inObj)  // Overloading <<
{
    cout << "**********************" << endl;
    inObj.print();
    return out;
};

#endif // COOKIE_H_INCLUDED
