#include <iostream>
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

int total_calc = 0;

struct Point{
    int x, y,z;
public:
    const Point& operator=(const Point &taken); // = operator
};

const Point& Point::operator=(const Point &taken){
    x = taken.x;
    y = taken.y;
    z = taken.z;
}

int partition(char arg, Point v[], int p, int r){ //partition function for quick-sort
    Point ch;
    int i = p-1;
    for(int j=p; j<r; j++){
        if(arg=='x'){  //sort wrt x coordinates
            if(v[j].x<v[r].x){  //I chose last element as pivot
                i=i+1;
                ch = v[i];  //exchange  v[i]  v[j]
                v[i] =  v[j];
                v[j] = ch;
            }
        }else if(arg =='y'){  //sort wrt y coordinates
            if(v[j].y<v[r].y){
                i=i+1;
                ch = v[i];  //exchange  v[i]  v[j]
                v[i] =  v[j];
                v[j] = ch;
            }
        }
    }
    ch = v[i+1];  //exchange [vi+1]  v[r]
    v[i+1] = v[r];
    v[r] = ch;
    return i+1;
}

void Quicksort(char arg, Point P[], int p, int r){ //quick sort algorithm
    if(p<r){
        int q = partition(arg, P, p, r);
        Quicksort(arg, P, p,q-1);
        Quicksort(arg, P, q+1, r);
    }
}


float Edist_threed(Point p1, Point p2){   //Euclidean distance for 3D
    return sqrt( (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) + (p1.z -p2.z)*(p1.z -p2.z));
}

float Edist_twod(Point p1, Point p2){ //Euclidean distance for 2D
    return sqrt( (p1.y - p2.y)*(p1.y - p2.y) + (p1.z - p2.z)*(p1.z - p2.z));
}


float bruteForce(Point P[], int n){   //Brute Force method to return the smallest distance between two points
    float min = FLT_MAX;
    for (int i = 0; i < n; ++i){
        for (int j = i+1; j < n; ++j){
            total_calc++;
            if (Edist_threed(P[i], P[j]) < min)
                min = Edist_threed(P[i], P[j]);
        }
    }
    return min;
}

float min(float x, float y){
    return (x < y)? x : y;
}

// Note that this method seems to be a O(n^2) method, but it's a O(n) method as the inner loop runs at most 15 times
float sClosest(Point S[], int size, float d) {
    float min = d;  // Initialize the minimum distance as d

    Quicksort('y', S, 0, size-1); //first sort coordinates wrt y axis.

    //In 3D we can think the line in 2D as a plane that is parallel to x axis. So, in S there are points
    //that their x coordinates are in distance to the plane as 0<distance<d, since we are looking for points
    //with distances less than 'd', their 2D Euclidean distance between y and z coordinates
    //should be less than  'd', so we are looking for these points;
    for (int i = 0; i < size; ++i)
        for (int j = i+1; j < size && Edist_twod(S[j], S[i]) < min; ++j){
            total_calc++;
            if (Edist_threed(S[i],S[j]) < min)
                min = Edist_threed(S[i], S[j]);
        }

    return min;
}

float closestPair_Rec(Point Px[], Point Py[], int n){  //recursive function to find the smallest distance.
    // If there are 2 or 3 points, then use brute force
    if (n <= 3)
        return bruteForce(Px, n);

    // Find the middle point
    int mid = n/2;

    Point midPoint = Px[mid]; //mid point that determines the plane separates Right and Left points
    //Construct Lx, Ly, Rx, Ry
    Point *Ry = new Point[n-mid];
    Point *Ly = new Point[mid];
    Point *Lx = new Point[mid];
    Point *Rx = new Point[n-mid];

    int ri=0;
    for (int i = 0; i < mid; i++){
       Lx[i] = Px[i];
       Ly[i] = Py[i];
    }
    for (int i = mid; i < n; i++){
       Rx[ri] = Px[i];
       Ry[ri] = Py[i];
       ri++;
    }


    float dl = closestPair_Rec(Lx, Ly, mid);
    float dr = closestPair_Rec(Rx, Ry, n-mid);

    float d = min(dl, dr);    // Find the smaller of two distances


    Point* S = new Point[n];
    int j = 0;
    for (int i = 0; i < n; i++)      // Build an array S[] that contains points closer than d
        if (abs(Px[i].x - midPoint.x) <=d)
            S[j] = Px[i], j++;

    return min(d, sClosest(S, j, d) );
}

float closestPair(Point P[], int n){

    Point* Px = new Point[n];
    Point* Py = new Point[n];
    for (int i = 0; i < n; i++){
        Px[i] = P[i];
        Py[i] = P[i];
    }

    Quicksort('x', Px, 0, n-1);   //I used Quicksort
    Quicksort('x', Py, 0, n-1);

    return closestPair_Rec(Px, Py, n);
}

int rFile(char* filename, Point*&P){
    int n;
    ifstream inFile(filename);
    int xCo,yCo,zCo;
    string first_line;
    getline(inFile, first_line);
    istringstream buffer(first_line);
    buffer>>n;
    P = new Point[n];
    int i=0;
    while(inFile >> xCo >> yCo >> zCo){
            Point newPoint;
            newPoint.x = xCo;
            newPoint.y = yCo;
            newPoint.z = zCo;
            P[i] = newPoint;
            i++;
    }
    return n;
}

int main(int argc, char* argv[]){
    char* filename = argv[1];
    Point *P;
    int n = rFile(filename, P);
    clock_t begin = clock();
    cout << "The smallest distance is " << closestPair(P, n) << endl;
    clock_t end = clock();
    double elapsed_secs = double(end-begin)/CLOCKS_PER_SEC;
    cout << "Number of total distance calculations is " << total_calc << endl << endl;
    cout << "Elapsed time: "<< elapsed_secs<< " secs"<< endl;
    return 0;
}
