/*Ahmet Göktuð SEVÝNÇ
  150140120
  sevinca@itu.edu.tr
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include  <time.h>
#include <string.h>

#include "list.h"

using namespace std;

typedef list datastructure;
datastructure defter;

int x = -1;
int y= 1;

void play();
void navigate(int, playlist*);
void addoperator(int, playlist*);
void printlist();
void createlist();
void printmenu();
void addsong();
void removesong();
void deletelist();
void writetofile();
void exit();
bool operate(char);

int main(){

    defter.create();
    bool operation = false;
    char choice;
    while(!operation){
        printmenu();
        cin >> choice;
        operation = operate(choice);
    }

    return 0;
}

void printmenu(){
    system("cls");
    cout << endl << endl;
    cout << "SONGBOX APPLICATION" << endl<<endl;;
    cout << "Choose and operation " << endl<<endl;
    cout << "P: Play" << endl;
    cout << "L: Show all playlists" << endl;
    cout << "A: Add songs to a playlist" << endl;
    cout << "R: Remove songs from a playlist" << endl;
    cout << "C: Create a new playlist" << endl;
    cout << "D: Delete a playlist " << endl;
    cout << "W: Write to a file" << endl;
    cout << "E: Exit" << endl;
}
bool operate(char choice){
    bool bitir = false;
    switch(choice){
        case 'P': case 'p':
        play();
        break;

        case 'L' : case 'l' :
        cout << endl;
        printlist();
        break;

        case 'C' : case 'c' :
        createlist();
        break;

        case 'A' : case 'a' :
        addsong();
        break;

        case 'R' : case 'r' :
        removesong();
        break;

        case 'D' : case 'd' :
        deletelist();
        break;

        case 'W' : case 'w' :
        writetofile();
        break;

        case 'E' : case 'e' :
        exit();
        bitir = true;
        break;

        default:
        cout << "Please enter a valid choice.." << endl;
        getchar();
        getchar();
    }
    return bitir;
}

void createlist(){
    playlist *newplaylist;
    newplaylist = new playlist;
    newplaylist->head = NULL;
    int option;
    char c[30];
    playlist *t;
    t = defter.head;
    cout << endl;
    cout << "1: Specific style" << endl;
    cout << "2: Specific singer" << endl;
    cout << "3: Combine playlists" << endl;
    cout << "4: Combine songs" << endl << endl;
    cout << "Option: ";
    cin >> option;
    cout << endl;
    if(option != 1 && option != 2 && option != 3 && option != 4){
        cout << "Please choose a valid choice!" << endl;
        getchar();
        getchar();
        return;
    }
    if(option == 1){
        cout << "In which style do you want to create a playlist (in capital letters please ex: SLOW) : " << endl;
        cin.ignore(1000, '\n');
        cin.getline(c, 30);
        strcpy(newplaylist->name, c); // name of new playlist will be type of song ex.SLOW
        song_node *s;
        int j=1;
        for(int i=1;i<t->songnumber;i++){ // scan all songs of chronological playlist
            if(strcmp(defter.givenode(i, t)->data->style, c) == 0){  // if type match
                defter.addSongNode(defter.givenode(i, t), newplaylist); // add song to new playlist
                j++; //control variable
            }
        }

        if(j==1){ //if j did not change it means there is no song in that style
            cout << endl;
            cout << "There is not any song in this style." << endl;
            getchar();
            return;
        }
        defter.addplaylist(newplaylist);  //add new playlist
        cout << endl;
        cout << c << " playlist has been created and songs have been added." << endl;
        getchar();
    }
    else if(option==2){
        cout << "Which singer's playlist do you want to create (capital letters please ex: THE BEATLES) :  " << endl;
        cin.ignore(1000, '\n');
        cin.getline(c, 30);
        strcpy(newplaylist->name, c);
        song_node *s;
        int j=1;
        for(int i=1;i<t->songnumber;i++){
            if(strcmp(defter.givenode(i, t)->data->singer, c) == 0){  // if singer name matches
                defter.addSongNode(defter.givenode(i, t), newplaylist);  // add to new playlist
                j++;
            }
        }

        if(j==1){  // if j = 1 there is no songs of this singer
            cout << endl;
            cout << "You don't have any songs of this singer." << endl;
            getchar();
            return;
        }
        defter.addplaylist(newplaylist);  //add playlist
        cout << endl;
        cout << c << " playlist has been created and songs have been added." << endl;
        getchar();
    }
    else if(option == 3){
        int a,b;
        defter.printplaylist(&x); // x is global variable -1 this function prints all lists
        cout << "Which playlists do you want to combine: ";
        cin >> a >> b;
        if(a<0 || a>defter.playlist_count || b<0 || b>defter.playlist_count){ //check valid option
            cout << "Please enter valid choices." << endl;
            getchar();
            getchar();
            return;
        }
        playlist *m,*n;
        m = defter.printplaylist(&a); // print playlist function returns playlist*, so m and n are chosen playlists
        n = defter.printplaylist(&b);
        a = m->songnumber;  //song numbers
        b = n->songnumber;
        cout << endl;
        cout << "Name for new playlist:  ";  // take name for playlist
        cin.ignore(1000, '\n');
        cin.getline(c, 30);
        strcpy(newplaylist->name, c);
        defter.addplaylist(newplaylist);
        for(int i=1; i<=a; i++){
            defter.addSongNode(defter.givenode(i, m),newplaylist);  // first add songs of first chosen playlist to new playlist
        }
        for(int i=a+1; i<=a+b; i++){
            defter.addSongNode(defter.givenode(i, n),newplaylist);  // then add other playlist's songs
        }
        cout << "Playlist had been created" << endl;
        getchar();
        getchar();

    }else if(option == 4){
        cout << "Enter the name of new playlist:  ";
        int a;
        cin.ignore(1000, '\n');
        cin.getline(c,30);
        strcpy(newplaylist->name, c);
        defter.addplaylist(newplaylist);
        addoperator(a, newplaylist); // send playlist to addoperator function
        cout << "Playlist has been created." << endl;
        getchar();
        getchar();
    }
}

void addsong(){
    system("cls");
    playlist *t, *p;
    p = defter.head->next; // p sorted playlist
    defter.printplaylist(&x);  // print all playlists
    int a;
    cout << "Which playlist do you want to add song: ";
    cin >> a;
    if (a==1 || a==2 || a==3){ //can not add songs to chronological, random, sorted
        cout << endl;
        cout << "This is main playlist you can not add songs to it." << endl;
        getchar();
        getchar();
    }else if(a<0 || a>defter.playlist_count){  //check valid option
        cout << "Please choose a valid option."<<endl;
        getchar();
        getchar();
    }else{
        t = defter.printplaylist(&a);  //take choice of user to t playlist
        cout << endl << endl;
        addoperator(a, t); //send it to addoperator function
    }
}

void removesong(){
    playlist *t;
    cout << endl;
    defter.printplaylist(&x);  //list all playlists
    cout << "Which playlist do you want to remove song: ";
    int a;
    cin >> a;
    if(a<0 || a>defter.playlist_count){  //check validity
        cout << "Please enter a valid choice." << endl;
        getchar();
        getchar();
        return;
    }
    if(a == 1 || a == 2 || a == 3){  //can not remove songs from main playlists
        cout << "This playlist is main playlist. Songs from this playlist can not be deleted." << endl;
        getchar();
        getchar();
        return;
    }
    system("cls");
    t=defter.printplaylist(&a);  //t is choice
    cout << endl<< endl;
    defter.printsongs(t,&x); //print songs of t
    cout << endl;
    cout << "Enter the number of song that you want to delete: ";
    cin >> a;
    if (a<=0 || a>t->songnumber){  //check validity
        cout << "Please enter a valid number!" << endl;
        getchar();
        getchar();
    }else{
        defter.removesong(a, t);  //remove song
        cout << "Song has been deleted.." << endl;
        getchar();
        getchar();
    }
}

void deletelist(){
    playlist *t;
    cout << endl;
    defter.printplaylist(&x);
    cout << "Which playlist do you want to delete: ";
    int a;
    cin >> a;
    if(a<0 || a>defter.playlist_count){ //check validity
        cout << "Please enter a valid choice." << endl;
        getchar();
        getchar();
        return;
    }
    char c;
    if (a==1 || a==2 || a==3){  //main playlists can not be deleted
        cout << "This is main playlist it can not be deleted." << endl;
        getchar();
        getchar();
    }else{
        cout << "Do you want to delete this playlist? (Y/N)" << endl;
        cin >> c;
        if(c=='Y' || c=='y'){
            defter.removeplaylist(a);  // delete playlist
            cout << endl;
            cout << "Playlist has been deleted." << endl;
            getchar();
            getchar();
        }
    }
}

void play(){
    playlist *t;
    system("cls");
    cout << endl;
    cout << "1: Play a playlist from beginning " << endl;
    cout << "2: Play a playlist from a specific song " << endl;
    cout << "3: Play a single song " << endl << endl;
    cout << "Option: ";
    int a;
    cin >> a;
    cout << endl;
    if(a==1){
        defter.printplaylist(&x); //print all songs
        cout << "Which playlist do you want to play: ";
        cin >> a;
        if(a<0 || a>defter.playlist_count){  // check validity
            cout << "Please enter a valid choice." << endl;
            getchar();
            getchar();
            return;
        }
        t = defter.printplaylist(&a);  //t is chosen
        if(strcmp(t->name, "Random")==0){  //if random playlist is chosen as for shuffle it or not
            cout << endl << endl;
            cout << "Do you want to shuffle this list? (Y/N) ";
            char c;
            cin >> c;
            if(c=='Y' || c=='y'){
                defter.createrandom(t,defter.head);  // if answer is yes send random playlist and chronological playlist to func.
            }
        }
        int b=1;
        navigate(b,t); //call navigate function
    }
    else if(a==2){
        defter.printplaylist(&x);
        cout << "Which playlist do you want to play: ";
        cin >> a;
        if(a<0 || a>defter.playlist_count){
            cout << "Please enter a valid choice." << endl;
            getchar();
            getchar();
            return;
        }
        system("cls");
        cout << endl;
        t = defter.printplaylist(&a);
        if(strcmp(t->name, "Random")==0){  // ask for shuffle random list
            cout << endl << endl;
            cout << "Do you want to shuffle this list? (Y/N) ";
            char c;
            cin >> c;
            if(c=='Y' || c=='y'){
                defter.createrandom(t,defter.head);
            }
        }
        cout << endl<< endl;
        for(int i=1; i<=t->songnumber; i++){
            defter.printsongs(t, &i);
        }
        cout << endl;
        cout << "Which song do you want to start playing: ";
        cin >> a;
        navigate(a, t);
    }else if(a==3){
        system("cls");
        t = defter.head->next;
        cout << endl<< endl;
        for(int i=1; i<=t->songnumber; i++){
            defter.printsongs(t, &i);
        }
        cout << endl;
        cout << "Which song do you want to play: ";
        cin >> a;
        cout << endl;
        cout << "playing..." << endl;
        defter.printsongs(t, &a);
        getchar();
        getchar();
    }else{
        cout << "Please choose a valid option." << endl;
        getchar();
        getchar();
    }
}

void printlist(){
    playlist *t,*q;
    q = defter.head;
    int a=1;
    char c;
    int take;
    while(a!=-2){
        system("cls");
        if(a>defter.playlist_count){  // I explained this in navigate function please check it if you want..
            while(a>=defter.playlist_count){
                a = a-defter.playlist_count;
            }
        }else if(a<=0){
            a = a + defter.playlist_count;
        }
        cout << endl;
        defter.printplaylist(&x);
        t=defter.printplaylist(&a);
        cout << endl;
        cout << "N(NEXT) / P(PREVIOUS) / S(SELECT) / E(EXIT)"  << endl;
        cin >> c;
        if(c=='n' || c=='N'){
            a++;
        }
        else if(c=='p' || c=='P'){
            a--;
        }
        else if (c=='e' || c=='E'){
            a = -2;
        }
        else if(c == 's' || c== 'S'){
            system("cls");
            t=defter.printplaylist(&a);
            if(strcmp(t->name, "Random")==0){  //ask for shuffle
                cout << endl << endl;
                cout << "Do you want to shuffle this list? (Y/N) ";
                cin >> c;
                if(c=='Y' || c=='y'){
                    defter.createrandom(t,q);
                }
            }
            cout << endl<<endl;
            cout << "D: Delete the music list." << endl;
            cout << "A: Add song to playlist." << endl;
            cout << "R: Remove song from playlist." << endl;
            cout << "P: Play songs of this music list." << endl;
            cin >> c;
            if(c=='D' || c=='d'){
                if (a==1 || a==2 || a==3){  //cam't delete main lists
                    cout << "This is main playlist it can not be deleted." << endl;
                    getchar();
                    getchar();
                }else{
                    cout << "Do you want to delete this playlist? (Y/N)" << endl;
                    cin >> c;
                    if(c=='Y' || c=='y'){
                        defter.removeplaylist(a);
                        cout << "Playlist has been deleted." << endl;
                        getchar();
                        getchar();
                    }
                }
            }else if(c=='P' || c=='p'){
                int b = 1;
                navigate(b, t);

            }else if (c=='R' || c=='r'){
                if (a==1 || a==2 || a==3){  //can't remove songs of main lists
                    cout << "This is main playlist. It's songs can not be deleted." << endl;
                    getchar();
                    getchar();
                }else{
                    defter.printsongs(t,&x);
                    cout << "Enter the number of song that you want to delete: ";
                    cin >> take;
                    if (take<=0 || take>t->songnumber){
                        cout << "Please enter a valid number!" << endl;
                        getchar();
                        getchar();
                    }
                    else{
                    defter.removesong(take, t);
                    cout << "Song has been deleted.." << endl;
                    getchar();
                    getchar();
                    }
                }
            }else if(c=='A' || c =='a'){
                if (a==1 || a==2 || a==3){  //can't add songs to main lists
                    cout << "This is main playlist you can not add songs to it." << endl;
                    getchar();
                    getchar();
                }else{
                    song_node *s;
                    defter.printsongs(q, &x);
                    cout << "Enter the number of song that you want to add: ";
                    cin >> take;
                    if (take<=0 || take>t->songnumber){
                        cout << "Please enter a valid number!" << endl;
                        getchar();
                        getchar();
                    }else{
                        s=defter.printsongs(q, &take);
                        defter.addSongNode(s, t);
                        cout << "Song has been added. " << endl;
                        getchar();
                        getchar();
                    }
                }
            }else{
                cout << "Enter a valid choice!" << endl;
                getchar();
                getchar();
            }
        }else{
            cout << "You have entered an invalid choice!" << endl;
            getchar();
            getchar();
        }
    }
}
void addoperator(int a, playlist*p){
    playlist *t;
    t = defter.head->next;  //t sorted playlist
    while(a!=-1){
        system("cls");
        defter.printsongs(t, &x); //print all songs of sorted playlist, x is global -1
        cout << endl;
        cout << "Which song do you want to add: ";
        cin >> a;
        if(a<0 || a>t->songnumber){  //check validity
            cout << "Please enter a valid choice." << endl;
            getchar();
            getchar();
            return;
        }
        defter.addSongNode(defter.givenode(a,t), p);  //take node from givenode function and send it addnode functuon as a variable
        cout << "Song had been added do you want to add more songs? (Y/N) ";
        char x;
        cin >> x;
        cout << endl;
        if(x=='y' || x=='Y') continue;
        else if(x=='n' || x=='N') a=-1;
        else{
            cout << "You have entered an invalid choice." << endl;
            getchar();
        }
    }
}

void navigate(int b, playlist*t){
    cout << endl << endl;
    char c;
    while(b!=-2){
        if(t->songnumber == 0){  // if playlist is empty
            cout << "There is no song in this playlist." << endl;
            getchar();
            getchar();
            return;
        }
        if(b>t->songnumber){ // I control playlist with integer b, if b is bigger than songnumber of playlist
            while(b>=t->songnumber){  // Until b will be smaller than it
                b = b-t->songnumber;  // for example if there is 5 songs and b is 6, b = 6-5 = 1 , so b will show first song
            }
        }else if(b==0){
            b = b + t->songnumber;  // 0 + 5 = 5, so it shows last playlist
        }
        cout << endl;
        cout << "playing ...." << endl;
        defter.printsongs(t, &b);
        cout << "N(NEXT) / P(PREVIOUS) / E(EXIT)"  << endl<< endl;
        getchar();
        c = getchar();
        if(c=='n' || c=='N'){
            b++;
        }
        else if(c=='p' || c=='P'){
            b--;
        }
        else if(c=='E' || c == 'e'){
            b=-2;
        }else{
            cout << "Please enter a valid choice." << endl;
            getchar();
            getchar();
            return;
        }
    }
}

void writetofile(){
    defter.writetofile();
}

void exit(){
    defter.writetofile();
    defter.exit();
}

