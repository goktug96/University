/*
    Ahmet Göktuð SEVÝNÇ
    150140120
*/
#ifndef ICECREAM_H_INCLUDED
#define ICECREAM_H_INCLUDED

#include <string.h>
#include <stdlib.h>
using namespace std;

class Icecream{
    string type;
    string name;
    float litre;
    float priceperlitre;

public:
    Icecream(){type = "Ice-Cream";}  //default constructor
    Icecream(const char* , float, float);  //constructor
    void print()const;
    float getLitre(){return litre;} //return private elements
    float getPrice()const{return litre*priceperlitre;} //price calculation
    string getType(){return type;}
    string getName(){return name;}
};

Icecream::Icecream(const char* obj, float l, float ppl){
    type = "Ice-Cream";
    name = obj;
    litre = l;
    priceperlitre = ppl;
}

void Icecream::print()const{
    cout << name << " #" << litre << " Cost: " << getPrice()  << endl;
}


ostream& operator <<(ostream& out, const Icecream& inObj)  // Overloading <<
{
    cout << "**********************" << endl;
    inObj.print();
    return out;
};



#endif // ICECREAM_H_INCLUDED
