/*
    Ahmet Göktuð SEVÝNÇ
    150140120
*/
#ifndef SHOP_H_INCLUDED
#define SHOP_H_INCLUDED

#include <iostream>
#define TAX 8

using namespace std;

template<class Type>
class Shop{
private:
    int MAX;
    int size;
    int discount;
    Type *element;
public:
    Shop(); //default constructor
    Shop(Type&); //constructor
    void setDiscount(int);
    Type* getelement()const; //since elements are private I used methods to call them
    int getSize()const;
    int getCost()const;
    int getDiscount()const;
    void add(const Type&);
    Type& operator[](int);
    ~Shop(); //destructor
};

template<class Type>
Shop<Type>::Shop(){   //default constructor
    MAX=20;
    size = 0;
    discount = 0;
    element = new Type[MAX];  //take place
}


template<class Type>
Shop<Type>::Shop(Type& obj){   //constructor with object
    MAX=20;
    element = new Type[MAX];  // take dynamic memory
    size = 0;
    discount=0;
    element[size]=obj;  //add first element
    size++;
}

template<class Type>
void Shop<Type>::setDiscount(int n){

    if(n<0 || n>30){  //if discount is out of the range throw error
        discount=0;
        throw "Discount rate is out of the range!";
    }
    discount = n;
}

template<class Type>
int Shop<Type>::getSize()const{
    return size;
}

template<class Type>
int Shop<Type>::getDiscount()const{
    return discount;
}

template<class Type>
Type* Shop<Type>::getelement()const{
    return element;
}

template<class Type>
void Shop<Type>::add(const Type& item){
    if(size>=MAX){  //if there are MAX(20) elements, I'll double the dynamic memory
        MAX = MAX*2; //multiply MAX with 2
        Type* temp = new Type[MAX]; //take a temporary memory
        for(int i=0; i<MAX/2; i++){
            temp[i] = element[i];  // take elements to temporary memory
        }
        temp[size] = item;  //add last item
        size++;
        delete[]element; //delete element
        element = temp;  //make element to temp
    }
    element[size] = item;  //add item
    size++;
}

template<class Type>
Type& Shop<Type>::operator[](int index){  //operator overloading
    if(index<0||index>size-1){  //throw message
        throw "We don't have enough ";
    }
    else return element[index];
}

template<class Type>
Shop<Type>::~Shop(){  //destructor delete element
    delete [] element;
}

template <typename Type>
ostream& operator <<(ostream& out, const Shop<Type>& inObj)  // Overloading <<
{
    float cost=0;
    int discount = inObj.getDiscount();
    cout << "**********************" << endl;
    cout << "Number of items: "  << inObj.getSize() << endl; //write total items
    for (int i=0; i<inObj.getSize(); i++){
        cout << i+1 << ": ";
        inObj.getelement()[i].print();  //call print function for ith object
        cost = cost + inObj.getelement()[i].getPrice(); //renew cost

    }
    float totalCost =  cost + (cost*TAX/100); //add tax
    cout << "**********************" << endl;
    cout << "Total cost: " << totalCost << endl; //print total cost
    if(discount!=0){ //discount is 0 as default
        cout << "Discount: %" << discount << endl;  //print discount informations
        cout << "Discount amount: " << -(totalCost*discount/100) << endl;
        cout << "Discounted amount: " << totalCost - (totalCost*discount/100) << endl;
    }
    cout << "**********************";
   return out;
};


#endif // SHOP_H_INCLUDED
