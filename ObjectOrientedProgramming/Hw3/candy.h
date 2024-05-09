/*
    Ahmet Göktuð SEVÝNÇ
    150140120
*/
#ifndef CANDY_H_INCLUDED
#define CANDY_H_INCLUDED

#include <string.h>
using namespace std;

class Candy{
    string type;
    string name;
    float weight;
    float priceperweight;

public:
    Candy(){type = "Candy";} //default constructor
    Candy(const char* , float, float); //constructor
    void print()const;
    float getWeight(){return weight;}
    float getPrice()const{return weight*priceperweight;} //price calculation
    string getType(){return type;}
    string getName(){return name;}
};

Candy::Candy(const char* obj, float w, float ppw){
    type = "Candy";
    name = obj;
    weight = w;
    priceperweight = ppw;
}

void Candy::print()const{
    cout << name << " #" << weight << " Cost: " << getPrice()  << endl;
}



ostream& operator <<(ostream& out, const Candy& inObj)  // Overloading <<
{
    cout << "**********************" << endl;
    inObj.print();
    return out;
};
#endif // CANDY_H_INCLUDED
