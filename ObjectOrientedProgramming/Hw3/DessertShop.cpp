/*
    Ahmet Göktuğ SEVİNÇ
    150140120
*/
#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include "shop.h"
#include "candy.h"
#include "cookie.h"
#include "icecream.h"

using namespace std;


int main() {

    Cookie cookie1("Chocolate Chip Cookies",10, 180); //(name, pieces, priceperdozen)
    Cookie cookie2("Cake Mix Cookies", 16, 210);
    Shop<Cookie> cookieShop(cookie1);
    cookieShop.add(cookie2);
    cout<<cookieShop<<endl;
    //Catch an exception here while trying to access the element at(2)

    try{
        cout << cookieShop[2] << endl;
    }catch(const char* err){
        cout << err << cookieShop.getelement()->getType() << endl;
    }

    Icecream icecream1("Chocolate ice cream",1.5, 170); //(name, litre, priceperlitre)
    Shop<Icecream> icecreamShop(icecream1);
    cout<<icecreamShop<<endl;
    //Catch an exception here while trying to set the discount to(50)

    try{
        icecreamShop.setDiscount(50);
    }catch(const char* err){
        cout << err << endl;
    }
    Candy candy2("Gummi bears",12,89); //(name, weight, priceperkg)
    Candy candy3("Hanukkah gelt",8,110);
    Shop<Candy> candyShop(candy2);
    candyShop.add(candy3);
    candyShop.setDiscount(15);
    cout<<candyShop<<endl;


    //  *****************FILE OPERATIONS**********************

    FILE* stock = fopen("stock.txt", "r");
    char taken[100];
    const char* part;
    char name[50];
    int count,type; //for cookies
    float c; //for icecreams and candies
    float priceper;
    fgets(taken, 100, stock); //take first line, redundant
    Shop<Candy> candyShop2;  //new shops for stock
    Shop<Cookie> cookieShop2;
    Shop<Icecream> icecreamShop2;

    while(!feof(stock)){

        fgets(taken, 100, stock); //take second line of stock file
        part = strtok(taken, "\t"); // split taken
        strcpy(name, part); //first part is name
        part = strtok(NULL, "\t"); //second part is type
        type = atoi(part);
        if(type==1){ //type 1 is cookie
            part = strtok(NULL, "\t"); //third part is count
            count = atoi(part);
            part = strtok(NULL, "\n"); //last part is price
            priceper = atof(part);
            const Cookie cookie(name, count, priceper); //create cookie for these properties
            cookieShop2.add(cookie); //add it to shop
        }
        else if(type==2){ //same operations for ice-cream and candy
            part = strtok(NULL, "\t");
            c = atof(part);
            part = strtok(NULL, "\n");
            priceper = atof(part);
            const Candy candy(name, c, priceper);
            candyShop2.add(candy);
        }
        else if(type==3){
            part = strtok(NULL, "\t");
            c = atof(part);
            part = strtok(NULL, "\n");
            priceper = atof(part);
            const Icecream ice(name, c, priceper);
            icecreamShop2.add(ice);
        }
    }

    //now we have shops that storing stock elements
    FILE* order = fopen("order.txt", "r");
    FILE* checkout = fopen("checkout.txt", "w+");
    int numofitem = 0;
    while(!feof(order)){ //first reading of order file for writing errors to checkout file
        //in example checkout file you have uploaded, first errors than infos was written so I read order file twice
        fgets(taken, 100, order);
        if(feof(order)) break;
        part = strtok(taken, "\t");
        strcpy(name, part);
        part = strtok(NULL, "\t");
        type = atoi(part);
        if(type == 1){ //if type is 1 look for cookies
            for(int i=0; i<cookieShop2.getSize(); i++){
                if(strcmp(name, cookieShop2.getelement()[i].getName().c_str())==0){ //c_str function translate string to char*
                    //if we have matching names look stock and order numbers
                    part = strtok(NULL, "\n");
                    count = atoi(part);
                    numofitem++;
                    if(count>cookieShop2.getelement()[i].getNumber()){ //if stock is not enough print error
                        fprintf(checkout, "!!!We dont have %d (%s)s\n", count, name);
                    }
                }
            }
        }else if(type == 2){ //same operations for candy and ice-cream
            for(int i=0; i<candyShop2.getSize(); i++){
                if(strcmp(name, candyShop2.getelement()[i].getName().c_str())==0){
                    part = strtok(NULL, "\n");
                    c = atof(part);
                    numofitem++;
                    if(c>candyShop2.getelement()[i].getWeight()){
                        fprintf(checkout, "!!!We dont have %.0f (%s)s\n", c, name);
                    }
                }
            }
        }else if(type==3){
            for(int i=0; i<icecreamShop2.getSize(); i++){
                if(strcmp(name, icecreamShop2.getelement()[i].getName().c_str())==0){
                    part = strtok(NULL, "\n");
                    c = atof(part);
                    numofitem++;
                    if(c>icecreamShop2.getelement()[i].getLitre()){
                        fprintf(checkout, "!!!We dont have %.0f (%s)s\n", c, name);
                    }
                }
            }
        }
    }

    fprintf(checkout, "------------------------------------------\n");
    fprintf(checkout, "Number of items: %d\n", numofitem);
    float totalCost=0;
    fseek(order, 0, SEEK_SET); //turn head of order file;
    while(!feof(order)){ //second read of order file to print infos
        fgets(taken, 100, order);
        if(feof(order)) break;
        part = strtok(taken, "\t");
        strcpy(name, part);
        part = strtok(NULL, "\t");
        type = atoi(part);
        float cost;

        if(type == 1){
            for(int i=0; i<cookieShop2.getSize(); i++){
                if(strcmp(name, cookieShop2.getelement()[i].getName().c_str())==0){
                    part = strtok(NULL, "\n");
                    count = atoi(part);
                    if(count>cookieShop2.getelement()[i].getNumber()){ //if orders is higher than stock we'll use stock infos
                        //basically cost = element[i].price; (since they are private I called methods)
                        cost = cookieShop2.getelement()[i].getPrice(); //Typeshop2's are keeping stock infos
                        totalCost = totalCost + cost;
                        count = cookieShop2.getelement()[i].getNumber();
                    }else{ //if we have enough in stock;
                        //calculating price:  (getPrice()function for stock)*count from order/count in stock
                        //if we have 10 in stock and order is 6:    cost=element[i].price*(6/10)
                        cost = (cookieShop2.getelement()[i].getPrice())*count/cookieShop2.getelement()[i].getNumber();
                        totalCost = totalCost + cost;
                    }
                    fprintf(checkout, "%d: %s #%d Cost: %.2f\n", type, name, count, cost);
                }
            }
        }else if(type == 2){ //same operations
            for(int i=0; i<candyShop2.getSize(); i++){
                if(strcmp(name, candyShop2.getelement()[i].getName().c_str())==0){
                    part = strtok(NULL, "\n");
                    c = atof(part);
                    if(c>candyShop2.getelement()[i].getWeight()){
                        cost = candyShop2.getelement()[i].getPrice();
                        totalCost = totalCost + cost;
                        c = candyShop2.getelement()[i].getWeight();

                    }else{
                        cost = (candyShop2.getelement()[i].getPrice())*c/candyShop2.getelement()[i].getWeight();
                        totalCost = totalCost + cost;
                    }
                    fprintf(checkout, "%d: %s #%.0f Cost: %.2f\n", type, name, c, cost);
                }
            }
        }else if(type==3){
            for(int i=0; i<icecreamShop2.getSize(); i++){
                if(strcmp(name, icecreamShop2.getelement()[i].getName().c_str())==0){
                    part = strtok(NULL, "\n");
                    c = atof(part);
                    if(c>icecreamShop2.getelement()[i].getLitre()){
                        cost = icecreamShop2.getelement()[i].getPrice();
                        totalCost = totalCost + cost;
                        c = icecreamShop2.getelement()[i].getLitre();

                    }else{
                        cost = (icecreamShop2.getelement()[i].getPrice())*c/icecreamShop2.getelement()[i].getLitre();
                        totalCost = totalCost + cost;
                    }
                    fprintf(checkout, "%d: %s #%.0f Cost: %.2f\n", type, name, c, cost);
                }
            }
        }
    }
    totalCost = totalCost + totalCost*TAX/100;  //print other infos
    int discountForFile = 10; //file discount
    fprintf(checkout, "******************************\n");
    fprintf(checkout, "Total cost: %.2f\n", totalCost);
    fprintf(checkout, "Discount: %% %d\n", discountForFile);
    float discountAmount = totalCost*discountForFile/100;
    fprintf(checkout, "Discount amount: -%.2f\n", discountAmount);
    fprintf(checkout, "Discounted cost: %.2f\n", totalCost-discountAmount);
    fclose(stock);
    fclose(order);
    fclose(checkout);
    return 0;
}
