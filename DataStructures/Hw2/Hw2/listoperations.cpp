#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iomanip>

#define SNAME_LENGTH 60
#define STYLE_LENGTH 30
#define SINGER_LENGTH 50

#include "list.h"

using namespace std;
void shuffle(int*, int);

void list::create(){
    head = NULL;
    playlist *Cplaylist, *Splaylist, *Rplaylist;  //create main lists
    Splaylist = new playlist;
    Rplaylist = new playlist;
    Cplaylist = new playlist;
    Cplaylist->head = NULL;
    Rplaylist->head = NULL;
    Splaylist->head = NULL;
    strcpy(Cplaylist->name,"Chronological");
    strcpy(Splaylist->name,"Sorted");
    strcpy(Rplaylist->name,"Random");
    addplaylist(Cplaylist);
    addplaylist(Splaylist);
    addplaylist(Rplaylist);
    readfromfile(Cplaylist); //read songs to Chronological list
    for(int i=1; i<=Cplaylist->songnumber;i++){  //Add all songs to sorted list
        song_node *t;
        t = givenode(i, Cplaylist); //take songs one by one
        addSongNode(t, Splaylist);  // add them to Sorted playlist
    }
    createrandom(Rplaylist, Cplaylist);  // Create random playlist
}
song_node* list::givenode(int a, playlist *p){
    song_node *newnode,*t;
    newnode = new song_node;
    t = p->head;
    for(int i=1; i<a; i++){  //move until ath song_node
        t= t->next;
    }

    *newnode = *t;  //copy it
    return newnode;  //return
}

void list::readfromfile(playlist *p){

    filename = "songbook.txt";
    songbook = fopen(filename, "r+");
    if(!songbook){
        if(!(songbook = fopen(filename, "r+"))){
            cerr << "File could not open!" << endl;
            getchar();
            getchar();
            return;
        }
    }
    char al[120];
    fseek(songbook, 0, SEEK_SET);
    fgets(al,120,songbook); //take first line, i took it here because i'll take lines at the end of the loop
    while(!feof(songbook)){
        char *taken;
        song *newsong;
        newsong = new song;
        newsong->name = new char[SNAME_LENGTH]; //take memory for songs
        newsong->singer = new char[SINGER_LENGTH];
        newsong->style = new char[STYLE_LENGTH];
        song_node *newnode;
        newnode = new song_node;
        taken = strtok(al, "\n");
        if(strcmp(taken, "*****")==0){ //if line is *****
            fgets(al,120,songbook); // take next line
            playlist *t;
            t = new playlist;
            addplaylist(t); //create new playlist
            t->head = NULL;
            taken = strtok(al, "\n");  //take the name of playlist
            strcpy(t->name, taken);  //copy it

            while(!feof(songbook)){  //then take song under "*****" till next "*****"
                newsong = new song;
                newsong->name = new char[SNAME_LENGTH];
                newsong->singer = new char[SINGER_LENGTH];
                newsong->style = new char[STYLE_LENGTH];
                fgets(al, 120, songbook);  // take next line songs of that playlist
                if(feof(songbook)) break;
                taken = strtok(al, "\t");
                if(strcmp(taken, "*****\n")==0) break;  //if we got "*****" (next playlist) break this loop
                strcpy(newsong->name, taken);  //take songs
                taken = strtok(NULL, "\t");
                strcpy(newsong->singer, taken);
                taken = strtok(NULL, "\t");
                newsong->year = atoi(taken);
                taken = strtok(NULL, "\n");
                strcpy(newsong->style, taken);
                newnode->data = newsong;
                addSongNode(newnode, t); //add them to new
            }
            continue;  // we break the loop and currently have "*****" or end of the file, so go beginning
        }
        taken = strtok(al, "\t");  //if not "*****" take songs of main playlist (Chronological)
        strcpy(newsong->name, taken);
        taken = strtok(NULL, "\t");
        strcpy(newsong->singer, taken);
        taken = strtok(NULL, "\t");
        newsong->year = atoi(taken);
        taken = strtok(NULL, "\n");
        strcpy(newsong->style, taken);
        newnode->data = newsong;
        addSongNode(newnode, p); //add them
        fgets(al,120,songbook);  // take next line
    }
    fclose(songbook);
}
void list::createrandom(playlist *p, playlist *q){ //p = random   q = main list
    srand(time(NULL));
    int num = q->songnumber;
    song_node *s;
    s = p->head;
    if(s!=NULL){  //firstly if random playlist is not empty remove all songs
        for( int i=1; i<=num; i++){
            removesong(1, p);
        }
    }
    p->head = NULL;
    int arr[50];
    for(int i=1; i<=num; i++){  //take an array and make first num(1-2-3-....num) element equal to itself
        arr[i] = i;
    }
    shuffle(arr, num);  //shuffle this array
    for(int j=1; j<=num; j++){
        int random = arr[j];  //take randomly ordered numbers from shuffled array
        song_node *s;
        s = givenode(random, q); //take random songs from chronological list
        addSongNode(s, p); // add them to random list
    }
}

void shuffle(int *arr, int n)  //shuffle array
{
    int i;
    for (i = 1; i <= n ; i++)
    {
        int j = rand() % n + 1; //j is random number between 1-n
        int t = arr[j]; //t equals this random number since arr listed from 1-n
        arr[j] = arr[i]; // make jth element equal to ith element
        arr[i] = t; // make ith element random number
    }
}

song_node* list::printsongs(playlist*p, int* a){
    song_node *q;
    q = p->head;
    if(*a==-1){ //if a=-1 print all songs of the playlist
        for(int i=0; i<p->songnumber; i++){
            cout<< left <<  i+1 <<setw(5) << ". " << setw(20) << q->data->singer << setw(42) << q->data->name << setw(20) << q->data->year << setw(10) << q->data->style<< endl;
            q = q->next;
        }
        return NULL;
    }

    if(*a>p->songnumber){ //else print ath song
        while(*a<=p->songnumber){
            *a= *a-p->songnumber;
        }
    }
    int i;
    for(i=1; i<*a; i++){
        q = q->next;
    }
    cout<< left  << i << setw(5) << ". " << setw(20) << q->data->singer << setw(42) << q->data->name << setw(20) << q->data->year << setw(10) << q->data->style << endl;
    return q;
}

void list::addplaylist(playlist *p){

    if(head==NULL){ //if there is no playlist
        head = p;
        p->next = p;
        p->prev = p;
        playlist_count++;
        return;
    }
    playlist *q;
    q=head;
    while(q->next != head){ //else add to the end
        q = q->next;
    }
    p->next = head;
    q->next = p;
    p->prev = q;
    head->prev = p;
    playlist_count++;

}
void list::addSongNode(song_node *s, playlist *p){
    song_node *newnode;
    song *newsong;
    newnode = new song_node;
    *newnode = *s;

    if(p->head==NULL){ //if there is no song on this playlist
        p->head = newnode;
        newnode->next = newnode;
        newnode->prev = newnode;
        p->songnumber++;
        return;
    }
    song_node*q;
    q=p->head;

    if(strcmp(p->name, "Chronological")==0){ //if playlist is chronological, sort by year
        if(p->head->data->year > newnode->data->year){ //add to the beginning
            p->head->prev->next = newnode;
            newnode->prev = p->head->prev;
            newnode->next = p->head;
            p->head->prev = newnode;
            p->head = newnode;
            p->songnumber++;
            return;
        }

        while(q->next != p->head && q->next->data->year < newnode->data->year ){
            q = q->next;
        }
        if(q->next == p->head){  //add to the end
            q->next = newnode;
            newnode->prev = q;
            newnode->next = p->head;
            p->head->prev = newnode;
            p->songnumber++;
        }else{
            newnode->next = q->next;  //add to the middle
            q->next->prev = newnode;
            q->next = newnode;
            newnode->prev = q;
            p->songnumber++;
        }
    }

    else if(strcmp(p->name, "Sorted")==0){ //if playlist is sorted, sort firs by singer then song
        if(strcmp(p->head->data->singer, newnode->data->singer)>0){ //add to the beginning
            p->head->prev->next = newnode;
            newnode->prev = p->head->prev;
            newnode->next = p->head;
            p->head->prev = newnode;
            p->head = newnode;
            p->songnumber++;
            return;
        }

        while(q->next != p->head){
            if(q->data->singer[0] < newnode->data->singer[0]){
                q = q->next;
            }else break;
        }

        if(q->next != p->head && q->next->data->singer[0] == newnode->data->singer[0]){
            while(q->next != p->head && strcmp(q->next->data->name, newnode->data->name)<0 ){
                q = q->next;
            }

        }
        if(q->next == p->head){  //add to the end
            q->next = newnode;
            newnode->prev = q;
            newnode->next = p->head;
            p->head->prev = newnode;
            p->songnumber++;
            return;
        }else{
            newnode->next = q->next;  //add to the middle
            q->next->prev = newnode;
            q->next = newnode;
            newnode->prev = q;
            p->songnumber++;
            return;
        }
    }

    else{  //add to the end for other playlists
        q->prev->next = newnode;
        newnode->prev = q->prev;
        newnode->next = p->head;
        p->head->prev = newnode;
        p->songnumber++;
    }
}

playlist* list::printplaylist(int* a){
    playlist *q;
    q=head;
    if(*a==-1){
        for(int i=0; i<playlist_count; i++){
            cout << i+1 << ": " <<  q->name << endl;
            q = q->next;
        }
        cout << endl;
        return NULL;
    }
    int i;

    for(i=1; i<*a; i++){
        q=q->next;
    }
    cout << endl;
    cout << i << ": " <<  q->name;


    return q;

}

void list::removeplaylist(int a){
    playlist *q;
    q=head;
    song_node *t, *s;
    if(a==1){
        q->prev->next = q->next;
        q->next->prev = q->prev;
        head = q->next;
        t=q->head;
        for(int i=0; i<q->songnumber; i++){
            s=t;
            t=t->next;
            delete s;
        }
        delete [] q->name;
        delete q;
        playlist_count--;
        return;
    }

    for(int i=1; i<a; i++){
        q = q->next;
    }
    q->prev->next = q->next;
    q->next->prev = q->prev;
    t=q->head;
        for(int i=0; i<q->songnumber; i++){
            s=t;
            t=t->next;
            delete s;
        }
        delete [] q->name;
        delete q;
        playlist_count--;
}
void list::removesong(int a, playlist *p){
    song_node *t;
    t= p->head;
    if(a == 1){
        t->prev->next = t->next;
        t->next->prev = t->prev;
        p->head = t->next;
        delete t;
        p->songnumber--;
        return;
    }
    for(int i=1; i<a; i++){
        t = t->next;
    }
    t->prev->next = t->next;
    t->next->prev = t->prev;
    delete t;
    p->songnumber--;
}
void list::writetofile(){
    FILE *fptr;
    filename = "songbook.txt";
    fptr = fopen(filename, "w"); //delete contents of file and open for writing
    playlist *p;
    p = head;
    while(p->next != head){
        if(strcmp(p->name, "Sorted")!=0 && strcmp(p->name, "Random")!=0){
            song_node* sn;
            sn = p->head;
            while(sn->next!=p->head){
                song *s;
                s = sn->data;
                fprintf(fptr, "%s\t%s\t%d\t%s\n",s->name, s->singer, s->year, s->style);
                sn = sn->next;
            }
            song *s;
            s = sn->data;
            fprintf(fptr, "%s\t%s\t%d\t%s\n",s->name, s->singer, s->year, s->style);
        }
        p=p->next;
        if(strcmp(p->name, "Sorted")!=0 && strcmp(p->name, "Random")!=0){
            fprintf(fptr, "*****\n");
            fprintf(fptr, "%s\n", p->name);
        }
    }
    if(strcmp(p->name, "Sorted")!=0 && strcmp(p->name, "Random")!=0){
        song_node* sn;
        sn = p->head;
        while(sn->next!=p->head){
            song *s;
            s = sn->data;
            fprintf(fptr, "%s\t%s\t%d\t%s\n",s->name, s->singer, s->year, s->style);
            sn = sn->next;
        }
        song *s;
        s = sn->data;
        fprintf(fptr, "%s\t%s\t%d\t%s\n",s->name, s->singer, s->year, s->style);
    }

    fclose(fptr);
}

void list::exit(){
    playlist *t,*p;
    t = head;
    song_node *s;
    for(int i=1; i<=t->songnumber; i++){
        s = t->head;
        t->head = t->head->next;
        delete [] s->data->name;
        delete [] s->data->singer;
        delete [] s->data->style;
        delete s;
    }
    for(int i=1; i<=playlist_count; i++){
        t = head;
        p = t;
        t = t->next;
        delete [] p->name;
        delete p;
    }
}
