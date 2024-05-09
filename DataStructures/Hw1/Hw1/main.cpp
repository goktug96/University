/*
    Ahmet Göktuð SEVÝNÇ
    150140120
    sevinca@itu.edu.tr
    14/10/2016
*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <fstream>

using namespace std;

struct patient_record {
char name[30], doctorName[30], diagnosis[30];
int patientNumber, polyclinicNumber;
char branchName[30];
}k[50];


void diz();
void bosluk_sil();
void bosluk_koy();
void olustur();
void al();
void print_menu();
void print_all();
void search_b();
void search_c();
void add();
void sil();
bool operation(char secim);
void bitir();

int counter = 0;


int main()
{
    olustur();  //once dizimizi olusturuyoruz
    bool bitir = false;
    char choice;
    while (!bitir){
        print_menu();
        cout << "Your choice: ";
        cin>>choice;
        bitir = operation(choice);
    }
    return 0;
}
void print_menu(){
    system("cls");
    cout << "Please select the operation" << endl << endl;
    cout << "P - Print all patients " << endl;
    cout << "B - Search data by branch name "<< endl;
    cout << "C - Search data by polyclinic number "<< endl;
    cout << "I - Insert new patient record " << endl;
    cout << "R - Remove patient record " << endl;
    cout << "E - Exit" << endl<<endl;

}
bool operation(char secim){
    bool son = false;
    switch (secim){
        case 'P': case 'p':
        print_all();
        getchar();
        getchar();
        break;

        case 'B': case 'b':
        search_b();
        getchar();
        break;

        case 'C': case 'c':
        search_c();
        getchar();
        getchar();
        break;

        case 'I': case 'i':
        add();
        break;

        case 'R': case 'r':
        sil();
        break;

        case 'E': case 'e':
        cout << "Do you want to exit? (Y/N) ";
        cin >> secim;
        if (secim == 'Y' || secim == 'y'){
            cout << endl;
            cout << "Ahmet Goktug SEVINC gururla sundu :)"<< endl;
            cout << "150140120" << endl;
            cout << "ILGINIZ ICIN TESEKKURLER.." << endl;
            getchar();
            getchar();
            bitir();
            son = true;
        }
        break;

        default:
        cout << "Please enter a valid choice..";
        getchar();
        getchar();
        break;
    }
    return son;
}
void olustur(){

    al(); //dosyadan verileri dizinin icine al
    diz();  // verileri sirala
    bosluk_koy(); //her bir poliklinik numarasini 10 eleman olacak sekilde bosluklarla doldur
}
void al(){
    FILE *fptr;
    fptr = fopen("database.txt", "r+");  //dosyayi okumak icin ac
    char deneme[120];
    while(!feof(fptr)){   //her bir line'yi al ve bolerek dizinin icine yerlestir..
        fgets(deneme, 120, fptr);
        if (feof(fptr)) break;
        char *item;
        item = strtok(deneme, "\t");
        strcpy(k[counter].name, item);
        item = strtok(NULL, "\t");
        strcpy(k[counter].doctorName, item);
        item = strtok(NULL, "\t");
        strcpy(k[counter].diagnosis, item);
        item = strtok(NULL, "\t");
        k[counter].patientNumber = atoi(item);
        item = strtok(NULL, "\t");
        k[counter].polyclinicNumber = atoi(item);
        item = strtok(NULL, "\n");
        strcpy(k[counter].branchName, item);
        counter++;
    }
    fclose(fptr);  //dosya kapat
}
void diz(){
    patient_record degis;
    for (int i=0 ; i<counter ; i++){
        for(int j=i+1; j<counter; j++){
            if (strcmp(k[i].branchName , k[j].branchName)>0){  //isme gore diz
                degis = k[i];
                k[i] = k[j];
                k[j] = degis;
            } //eger isimler ayni ise numaraya gore diz
            if ((strcmp(k[i].branchName , k[j].branchName)==0) && k[i].polyclinicNumber > k[j].polyclinicNumber){
                degis = k[i];
                k[i] = k[j];
                k[j] = degis;
            }
        }
    }
}
void bosluk_koy(){
    /*
    -once kac cesit poliklinik numarasi oldugunu bul.
    -daha sonra her bir poliklinik numarasindan kac tane oldugunu bul
    -diziyi ilk poliklinik numarasi kac ise ondan sonra 10 - poliklinik numarasi sayisi kadar kaydir
    -daha sonra ilk poliklinik numarasindan sonraki elemanlari tam 10 olacak sekilde bosa esitle
    */
    patient_record bos={};
    int pol_sayisi=0;

    int freq[100]={};
    for(int i=0; i<counter;i++){  //her bir polikligin frekansini buluyoruz
        freq[k[i].polyclinicNumber]++;
    }
    for (int i=0;i<20;i++){
        if(freq[i]!=0){
            pol_sayisi++; //pol_sayisi kadar farklı poliklinik var
        }
    }

    int c=0;
    int x;
    patient_record t1;
    for (int i=0; i<pol_sayisi;i++){ //her bir ayri eleman 11. 21. 31... siralarda olacak donguden sonra
            t1.polyclinicNumber = k[i*10].polyclinicNumber;  //bu yuzden k[i*10]. elemani aliyoruz her dongu basinda
            c=0;
        for(int j=10*i; j<10+10*i; j++){
            if(t1.polyclinicNumber == k[j].polyclinicNumber){
                c++;  //o poliklinik numarasindan kac tane var.
            }
        }
        x = 10-c;
        for (int n=counter-1; n>=c+i*10; n--){
            k[n+x] = k[n];  //diziyi 10-c kadar c. elemandan sonra kaydir
        }
        for (int m=c+i*10; m<i*10+10; m++){
            k[m] = bos;  //10a tamamlayana kadar bos ekle...
            counter++;  // kayit sayisini artir..
        }
    }
}
void bosluk_sil(){  //eleman ekledikten sonra veya dosyaya kaydederken veya branşa gore arama yaparken aradaki bos kayitlari siler
    patient_record bos = {};
    patient_record temp1[50];
    int counter1 = 0;

    for (int i=0;i<counter; i++){ //gecici dosyayi bosalt
        temp1[i]=bos;
    }

    for (int i=0; i<counter; i++ ){
        if(strcmp(k[i].name,bos.name)!=0){
            temp1[counter1]=k[i];
            counter1++;  //bos olmayan her bir eleman icin counter1i. elemani tempe yaz ve counter1'i artir
        }
    }
    for (int i=0;i<counter; i++){
        k[i]=bos;  //ana diziyi bosalt
    }
    for (int i=0; i<counter1; i++){
        k[i]=temp1[i];  //counter1 kadar bos olmayan eleman var bunlari tekrar k'ya at
    }
    counter = counter1;
}

void print_all(){  //pointer arithmetic
    cout << left << setw(20) << "Name" << setw(20)<< "Doctor Name" << setw(25) << "Diagnosis" << setw(15) << "Patient No" << setw(15) << "Polyclinic" << setw(10) << "Branch"<<endl<<endl;
    for(int i=0; i<counter; i++){
        cout << left << setw(20) << (k+i)->name << setw(20)<< (k+i)->doctorName << setw(25) << (k+i)->diagnosis << setw(15) << (k+i)->patientNumber << setw(15) << (k+i)->polyclinicNumber << setw(10) << (k+i)->branchName<<endl;
    }
}

void search_b(){
    char al[20];
    cout << "Which branch do you looking for? : ";
    cin.ignore(1000, '\n');
    cin.getline(al, 30);
    int flag = 0;
    //alttaki dongu basligi yazdırma boyle yapiyorum cunku boyle yapmazsam kullanici gecersiz bir deger girdiginde de yazdiriyor
    for(int i=0; i<counter; i++){
        if (strcmp(al, k[i].branchName)==0){
            cout << left << setw(20) << "Name" << setw(20)<< "Doctor Name" << setw(25) << "Diagnosis" << setw(15) << "Patient No" << setw(15) << "Polyclinic" << setw(20) << "Branch"<<endl<<endl;
            break;
        }
    }
    for (int i=0; i<counter; i++){
        if(strcmp(al, k[i].branchName)==0){
        flag = 1;
        cout << left << setw(20) << (k+i)->name << setw(20)<< (k+i)->doctorName << setw(25) << (k+i)->diagnosis << setw(15) << (k+i)->patientNumber << setw(15) << (k+i)->polyclinicNumber << setw(20) << (k+i)->branchName<<endl;
        }
    }
    if(flag == 0){
        cout << "This branch could not found." << endl;
    }
}

void search_c(){
    int al;
    int flag = 0;
    cout << "Please enter polyclinic number: ";
    cin >> al;
    cout << endl;
    //alttaki dongu basligi yazdırma boyle yapiyorum cunku boyle yapmazsam kullanici gecersiz bir deger girdiginde de yazdiriyor
    for(int i=0; i<counter; i++){
        if (al == k[i].polyclinicNumber){
            cout << left << setw(20) << "Name" << setw(20)<< "Doctor Name" << setw(25) << "Diagnosis" << setw(15) << "Patient No" << setw(15) << "Polyclinic" << setw(20) << "Branch"<<endl<<endl;
            break;
        }
    }
    for(int i=0; i<counter; i++){
        if (al == k[i].polyclinicNumber){
            flag = 1;
            cout << left << setw(20) << (k+i)->name << setw(20)<< (k+i)->doctorName << setw(25) << (k+i)->diagnosis << setw(15) << (k+i)->patientNumber << setw(15) << (k+i)->polyclinicNumber << setw(20) << (k+i)->branchName<<endl;
        }
    }
    if (flag == 0){
        cout << "Polyclinic number could not found." << endl;
    }
}

void add(){
    /* bu fonksiyon baya uzun oldu burada hepsini aciklamaya calisacagim:
        oncelikle her bir polikligimde 10'ar hasta oldugu icin ve once brans ismi, sonra numaraya gore
        dizili oldugu icin bu poliklinik numaralarina 10'un katlari seklinde erisebilirim.
        eger i'inci ve i+10'uncu elemanin brans isimleri ayni ise demek ki bu bransta iki poliklinik var.
        eger degilse o bransta sadece bir poliklinik var.
        hic bulamadiysa oyle bir brans kayitlarda yok demektir.
        poliklinik numarasini aldıktan sonra orada kaç hasta oldugunu bulacagim eger 10 ise dolu diye uyari verecegim
        yoksa hastayi ekleyecegim
    */
    int flag=1;
    char al[30];
    patient_record t;
    int hasta_sayisi = 0;
    cout << "Please enter the branch name: ";
    cin >> al;
    int i;
    for(i=0;i<counter;i++){
        if (strcmp(k[i].branchName,al)==0 && strcmp(k[i+10].branchName,al)==0){
            flag=0;
            cout << "There are two polyclinics for this branch." << endl;
            cout << "You can add patient only in "<< k[i].polyclinicNumber << "th or "<< k[i+10].polyclinicNumber << "th polyclinics." << endl;
            for (;;){
                cout << "Polyclinic number: ";
                cin >> al;
                if (atoi(al) != k[i].polyclinicNumber && atoi(al) != k[i+10].polyclinicNumber){
                    cout << "Please choose a valid polyclinic number." << endl;
                }else if (atoi(al) == k[i].polyclinicNumber){
                    for (int j=0;j<counter;j++){
                        if (k[j].polyclinicNumber == atoi(al)){
                            hasta_sayisi++;
                            }
                        }
                    if (hasta_sayisi == 10){
                        cout << "This polyclinic is full. Try again later please." << endl;
                        getchar();
                        getchar();
                        break;
                    }else cout << "There are " << hasta_sayisi << " patient(s) in this polyclinic you can add " << 10-hasta_sayisi << " more." << endl;
                    cout << "Name: ";
                    cin.ignore(1000, '\n');
                    cin.getline(al,30);
                    strcpy(t.name,al);
                    cout << "Doctor name: ";
                    cin.getline(al,30);
                    strcpy(t.doctorName,al);
                    cout << "Diagnosis: ";
                    cin.getline(al,30);
                    strcpy(t.diagnosis,al);
                    for (;;){
                        cout << "Patient number: ";
                        cin >> al;
                        if(strlen(al)!= 4) {
                            cout << "Please enter 4 digit number!" << endl;
                            continue;
                        }else{
                        t.patientNumber = atoi(al);
                        break;
                        }
                    }
                    strcpy(t.branchName,k[i].branchName);
                    t.polyclinicNumber = k[i].polyclinicNumber;
                    k[i+hasta_sayisi] = t;
                    cout << "Record added.";
                    getchar();
                    getchar();
                    break;
                }else if (atoi(al) == k[i+10].polyclinicNumber){
                    for (int j=0;j<counter;j++){
                        if (k[j].polyclinicNumber == atoi(al)){
                            hasta_sayisi++;
                            }
                        }
                    if (hasta_sayisi == 10){
                        cout << "This polyclinic is full. Try again later please." << endl;
                        getchar();
                        getchar();
                        break;
                    }else cout << "There are " << hasta_sayisi << " patient(s) in this polyclinic you can add " << 10-hasta_sayisi << " more." << endl;
                    cout << "Name: ";
                    cin.ignore(1000, '\n');
                    cin.getline(al,30);
                    strcpy(t.name,al);
                    cout << "Doctor name: ";
                    cin.getline(al,30);
                    strcpy(t.doctorName,al);
                    cout << "Diagnosis: ";
                    cin.getline(al,30);
                    strcpy(t.diagnosis,al);
                    for (;;){
                        cout << "Patient number: ";
                        cin >> al;
                        if(strlen(al)!= 4) {
                            cout << "Please enter 4 digit number!" << endl;
                            continue;
                        }else{
                        t.patientNumber = atoi(al);
                        break;
                        }
                    }
                    strcpy(t.branchName,k[i+10].branchName);
                    t.polyclinicNumber = k[i+10].polyclinicNumber;
                    k[hasta_sayisi+i+10] = t;
                    cout << "Record added.";
                    getchar();
                    getchar();
                    break;
                }

            }
            break;

        }else if(strcmp(k[i].branchName,al)==0){
            flag = 0;
            cout << "There is one polyclinic for this branch." << endl;
            cout << "You can add patient in "<< k[i].polyclinicNumber << "th polyclinic or "<< "you can create one more polyclinic." << endl;
            cout << k[i].polyclinicNumber << "th polyclinic or new polyclinic? (" << k[i].polyclinicNumber << "/" << "N)?";
            cin >> al;
            if (atoi(al)==k[i].polyclinicNumber){
                    for (int j=0;j<counter;j++){
                        if (k[j].polyclinicNumber == atoi(al)){
                            hasta_sayisi++;
                            }
                        }
                    if (hasta_sayisi == 10){
                        cout << "This polyclinic is full. Try again later please." << endl;
                        getchar();
                        getchar();
                        break;
                    }else cout << "There are " << hasta_sayisi << " patient(s) in this polyclinic you can add " << 10-hasta_sayisi << " more." << endl;
                    cout << "Name: ";
                    cin.ignore(1000, '\n');
                    cin.getline(al,30);
                    strcpy(t.name,al);
                    cout << "Doctor name: ";
                    cin.getline(al,30);
                    strcpy(t.doctorName,al);
                    cout << "Diagnosis: ";
                    cin.getline(al,30);
                    strcpy(t.diagnosis,al);
                    for (;;){
                        cout << "Patient number: ";
                        cin >> al;
                        if(strlen(al)!= 4) {
                            cout << "Please enter 4 digit number!" << endl;
                            continue;
                        }else{
                        t.patientNumber = atoi(al);
                        break;
                        }
                    }
                    strcpy(t.branchName,k[i].branchName);
                    t.polyclinicNumber = k[i].polyclinicNumber;
                    k[hasta_sayisi+i] = t;
                    cout << "Record added.";
                    getchar();
                    getchar();
                    break;
            }else {
                int flag=0;
                for(;;){
                cout << "Please enter the polyclinic number that you want to create: ";
                cin >> al;
                t.polyclinicNumber = atoi(al);
                for(int i=0;i<counter;i++){
                    if(t.polyclinicNumber == k[i].polyclinicNumber){
                        flag++;
                    }
                }
                if(flag!=0){
                    flag=0;
                    cout << "This polyclinic number is being used for another polyclinic. Please choose a another number." << endl;
                }
                else break;
                }

                cout << "Name: ";
                cin.ignore(1000, '\n');
                cin.getline(al,30);
                strcpy(t.name,al);
                cout << "Doctor name: ";
                cin.getline(al,30);
                strcpy(t.doctorName,al);
                cout << "Diagnosis: ";
                cin.getline(al,30);
                strcpy(t.diagnosis,al);
                for (;;){
                    cout << "Patient number: ";
                    cin >> al;
                    if(strlen(al)!= 4) {
                        cout << "Please enter 4 digit number!" << endl;
                        continue;
                    }else{
                    t.patientNumber = atoi(al);
                    break;
                    }
                }
                strcpy(t.branchName, k[i].branchName);
                k[counter] = t;
                counter++;
                bosluk_sil();
                diz();
                bosluk_koy();
                cout << "Record added.";
                getchar();
                getchar();
            }
            break;
        }
    }
    if (flag == 1){
        flag = 0;
        char take;
        cout << "You are creating new branch." << endl;
        cout << "Do you want to continue (Y/N): ";
        cin >> take;
        if(take == 'n' || take == 'N') return;
        strcpy(t.branchName, al);
        for(;;){
            cout << "Please enter the polyclinic number that you want to create: ";
            cin >> al;
            t.polyclinicNumber = atoi(al);
            for(int i=0;i<counter;i++){
                if(t.polyclinicNumber == k[i].polyclinicNumber){
                    flag++;
                }
            }
            if(flag!=0){
                flag=0;
                cout << "This polyclinic number is being used for another polyclinic. Please choose a another number." << endl;
            }
            else break;
            }

            cout << "Name: ";
            cin.ignore(1000, '\n');
            cin.getline(al,30);
            strcpy(t.name,al);
            cout << "Doctor name: ";
            cin.getline(al,30);
            strcpy(t.doctorName,al);
            cout << "Diagnosis: ";
            cin.getline(al,30);
            strcpy(t.diagnosis,al);
            for (;;){
                cout << "Patient number: ";
                cin >> al;
                if(strlen(al)!= 4) {
                    cout << "Please enter 4 digit number!" << endl;
                    continue;
                }else{
                t.patientNumber = atoi(al);
                break;
                }
            }
            k[counter] = t;
            counter++;
            bosluk_sil();
            diz();
            bosluk_koy();
            cout << "Record added.";
            getchar();
            getchar();
    }
}

void sil(){
    patient_record bos = {};
    char al[10];
    int flag=1;
    cout << "Please enter the patient number that you want to delete: ";
    cin >> al;
    int num = atoi(al);
    for(int i=0; i<counter; i++){
        if(k[i].patientNumber == num){
            flag=0;
            k[i] = bos;
            bosluk_sil();  //kayiti sildikten sonra bos kayitlari siliyoruz cunku aradaki bir kayit silinmis olabilir arada
            bosluk_koy();  // bosluk kalmasin daha sonra bos kayitlari tekrar ekliyoruz
            cout << "Record has been deleted.";
            getchar();
            getchar();
            break;
        }
    }
    if (flag==1){
        cout << "No matching record.";
        getchar();
        getchar();
    }
}
void bitir(){  //dosyayi yazmak icin ac, bosluklari sil, diziyi dosyaya yaz
    FILE *fptr;
    fptr = fopen("database.txt", "w");
    bosluk_sil();
    for(int i=0; i<counter; i++){
        fprintf(fptr, "%s\t%s\t%s\t%d\t%d\t%s\n", k[i].name, k[i].doctorName, k[i].diagnosis, k[i].patientNumber, k[i].polyclinicNumber, k[i].branchName);
    }
    fclose(fptr);
}

