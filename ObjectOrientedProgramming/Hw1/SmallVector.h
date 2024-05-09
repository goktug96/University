#ifndef SMALLVECTOR_H_INCLUDED
#define SMALLVECTOR_H_INCLUDED

class SmallVector{
    private:
        int size, dynamicSize;
        int StaticBuffer[32];
        int *DynamicBuffer;
        int *temp;
        void ExpandCapacity(const int);
        const int ShrinkCapacity();
    public:
        SmallVector();  //default constructor
        SmallVector(int ar[], int); //constructor
        ~SmallVector(); //destructor
        int& operator[](int index);  //subscript operator for non-constant
        const int& operator[] (const int) const;  //subscript operator for constant
        SmallVector operator+(const SmallVector&) const;  // + operator
        SmallVector operator*(const int) const; // * operator
        const SmallVector& operator=(const SmallVector &taken); // = operator
        const int getSize() const;  //take size of SmallVector
        void pushback(const int); // push back
        void pushback(const int ar[], const int); //push back array
        const int popback(); // pop back
        SmallVector reverse(); //reverse smallvector
};

SmallVector::SmallVector(){
    DynamicBuffer = NULL;  //default constructor, make sizes 0 and DynamicBuffer NULL;
    size = 0;
    dynamicSize = 0;
}
SmallVector::SmallVector(int ar[], int length){  //constructor with array and it's size
    DynamicBuffer = NULL;        //first make sizes 0 and DynamicBuffer NULL;
    size = 0;
    dynamicSize = 0;
    if(length<=32){     //if length of array is less than 32 we'll just use StaticBuffer
        for(int i=0; i<length; i++){
            StaticBuffer[i]=ar[i];  //add elements of array to StaticBuffer and increase size;
            size++;
        }
    }else{                             // if size bigger than 32 we'll save first 32 elements to StaticBuffer and rest of them
        for(int i=0; i<32; i++){       // to Dynamic buffer
            StaticBuffer[i] = ar[i];   //add first 32 elements and increase size;
            size++;
        }
        ExpandCapacity(length-32);  //Now we need a length-32 space for DynamicBuffer, call ExpandCapacity function;
        for(int j=0; j<length-32; j++){  //Now we have enough space in DynamicBuffer, copy +32 elements of array to DynamicBuffer
            DynamicBuffer[j] = ar[32+j];
            dynamicSize++;  // and increase DynamicSize
        }
    }
}

SmallVector::~SmallVector(){ //Destructor
    if(dynamicSize>1)
        delete [] DynamicBuffer; //Delete memory allocated for DynamicBuffer
    else
        delete DynamicBuffer;
}

void SmallVector::ExpandCapacity(const int add){
    if(DynamicBuffer==NULL){   //if DynamicBuffer is empty
        DynamicBuffer = new int[add];  //take memory for add times integers
        size = size + add;  //increase size of SmallVector
                            /* I did not use capacity since size is enough. Capacity is 32 for StaticBuffer and it is expands
                            with DynamicBuffer just like size. Or I understood something wrong. But my code works.
                            */
    }
    else{   // if DynamicBuffer is not empty
        temp = new int[dynamicSize+add]; //we'll create new space that enough for dynamicsize that used before and new needed size
        for(int i=0; i<dynamicSize; i++){  //copy old members of DynamicBuffer to temporary memory
            temp[i] = DynamicBuffer[i];
        }
        delete [] DynamicBuffer;  //now delete old DynamicBuffer
        DynamicBuffer = temp; // and make DynamicBuffer to new allocated temporary memory.
        size = size + add;  //and increase size of SmallVector, by the way I increase dynamicsize in functions not here.
    }
}

SmallVector SmallVector::reverse(){
    SmallVector reversed;
    if(size>32){ //if size of SmallVector is bigger than 32
        for(int i=0; i<dynamicSize; i++){  //DynamicBuffer elements will become our first elements
            reversed.StaticBuffer[i]=DynamicBuffer[dynamicSize-i-1];  //
        }
        for(int i=dynamicSize; i<32; i++){ //from dynamicSize'th element to 32th element take last elements of StaticBuffer
            reversed.StaticBuffer[i]=StaticBuffer[size-i-1];  //size -1 because if we have 35 elements 35th element is S.Buffer[34]
        }
        reversed.ExpandCapacity(dynamicSize); //we need DynamicBuffer as much as our SmallVector's DynamicBuffer
        for(int i=0; i<dynamicSize; i++){ //fill DynamicBuffer with first elements of StaticBuffer
            reversed.DynamicBuffer[i]=StaticBuffer[dynamicSize-i-1];
        }
    }else{  //if size less than 32 we'll just reverse StaticBuffer
        for(int i=0; i<size; i++){
            reversed.StaticBuffer[i]=StaticBuffer[size-1-i];
        }
    }
    reversed.size=size; //size and dynamicSize is not changed.
    reversed.dynamicSize = dynamicSize;
    return reversed; //return reversed SmallVector
}
void SmallVector::pushback(int value){
    if(size<32){  //if size less than 32 we'll add new element to end of the StaticBuffer
        StaticBuffer[size] = value;
        size++; //increase size
        return;
    } //else we'll expand capacity of DynamicBuffer by 1
    ExpandCapacity(1);
    DynamicBuffer[dynamicSize] = value; //and add the value end of the DynamicBuffer
    dynamicSize++; //increase DynamicSize
}

void SmallVector::pushback(const int ar[], const int length){
    if (size+length<32){  //if size + length is less than 32 we'll add elements to StaticBuffer
        for(int i=0; i<length; i++){
            StaticBuffer[size]=ar[i];
            size++; //increase size
        }
        return;
    }
    ExpandCapacity(length); //else expand DynamicBuffer by length
    for(int i=0; i<length; i++){
        DynamicBuffer[dynamicSize+i] = ar[i]; //add elements of array to DynamicBuffer
    }
    dynamicSize = dynamicSize + length; //increase dynamicSize
}

const int SmallVector::popback(){
    if(size<=32){  //if size is less or equal to 32 return last element of array and decrease size;
        size--;
        return StaticBuffer[size];
    }else{//else call ShrinkCapacity function that returns last element of DynamicBuffer and decreases size of DynamicBuffer by 1
        return ShrinkCapacity();
    }
}

const int SmallVector::ShrinkCapacity(){  // Shrink Capacity
    int last;
    last = DynamicBuffer[dynamicSize-1];  //take last element to be returned
    temp = new int[dynamicSize-1];  // create new memory for dynamicSize-1 integers
    for(int i=0; i<dynamicSize-1; i++){  //copy elements to new memory
        temp[i]=DynamicBuffer[i];
    }
    delete [] DynamicBuffer; //delete old DynamicBuffer
    DynamicBuffer = temp;  //and make it new memory
    dynamicSize--;  //increase dynamicSize and size
    size--;
    return last;  //return deleted number
}

const int SmallVector::getSize() const{
    return size; //return size;
}

int& SmallVector::operator[](int index){
    if(index>=size){            //if index is out of the bounds and positive return last element
        if(size<32){ //if size is less than 32 return last element of StaticBuffer else return last element of DynamicBuffer
            return StaticBuffer[size-1];
        }else{
            return DynamicBuffer[dynamicSize-1];
        }
    }else if(index<=-size-1){  //if index is out of the bounds and negative return first element
        return StaticBuffer[0];
    }

    if(index < 0 ){        //if index less than zero transform it to needed form
        index = index + size;  //for example if index = -1 and size = 35 index will be 34 so it'll point last element [34]
    }
    if(index<32){
        return StaticBuffer[index];
    }else{
        return DynamicBuffer[index-32];
    }
}
const int& SmallVector::operator[](int index) const{  //same operations for constant
    if(index>=size){
        if(size<32){
            return StaticBuffer[size-1];
        }else{
            return DynamicBuffer[dynamicSize-1];
        }
    }else if(index<=-size-1){
        return StaticBuffer[0];
    }
    if(index < 0 ){
        index = index + size;
    }
    if(index<32){
        return StaticBuffer[index];
    }else{
        return DynamicBuffer[index-32];
    }
}
SmallVector SmallVector::operator+(const SmallVector& toAdd) const{
    int a=this->getSize();   //take sizes of vectors to be added
    int b=toAdd.getSize();
    SmallVector newVector;  //new vector
    if(a+b<32){  //if sum of sizes is less than 32 we'll add all elements to StaticBuffer of newVector
        for(int i=0; i<a; i++){  //add first SmallVector
            newVector.StaticBuffer[i] = this->StaticBuffer[i];
            newVector.size++; //increase size
        }
        for(int i=0; i<b; i++){ //add second SmallVector
            newVector.StaticBuffer[a+i]=toAdd[i];
            newVector.size++; //increase size
        }
    }
    else if(a>32){ //if a is bigger than 32 and b is less than 32
        for(int i=0; i<32; i++){  //first fill StaticBuffer with first SmallVector's StaticBuffer
            newVector.StaticBuffer[i] = this->StaticBuffer[i];
            newVector.size++; //increase size
        }
        newVector.ExpandCapacity(dynamicSize); //expand capacity of newVector's DynamicBuffer as much as dynamicSize of first vec.
        for(int i=0; i<dynamicSize; i++){ //add dynamic elements of vector to newVector
            newVector.DynamicBuffer[newVector.dynamicSize+i] = this->DynamicBuffer[i];
        }
        newVector.dynamicSize = newVector.dynamicSize + dynamicSize; //increase dynamicSize of newVector

        newVector.ExpandCapacity(b); //now we need to expand DynamicBuffer of newVector as much as added vector's size;
        if(b<32){  //if size of toAdd vector is less than 32 we need to add StaticBuffer elements of it to DynamicBuffer of newV.
            for(int i=0; i<b; i++){
                newVector.DynamicBuffer[newVector.dynamicSize+i] = toAdd.StaticBuffer[i];
            }
            newVector.dynamicSize = newVector.dynamicSize + b; //and increase the dynamicSize of newV. by size of toAdd vec.
        }else if(b>32){  //else if size of toAdd is bigger than 32
            for(int i=0; i<32; i++){  //first add 32 Static elements of toAdd to newVector's dynamicbuffer
                newVector.DynamicBuffer[newVector.dynamicSize+i] = toAdd.StaticBuffer[i];
            }
            newVector.dynamicSize = newVector.dynamicSize + 32; //increase dynamicSize of newVec. by 32
            for(int i=0; i<toAdd.dynamicSize; i++){ //now add Dynamic elements of toAdd to newVector's dynamic elements
                newVector.DynamicBuffer[newVector.dynamicSize+i] = toAdd.DynamicBuffer[i];
            }
            newVector.dynamicSize = newVector.dynamicSize + toAdd.dynamicSize; //increase dynamicSize of newVector;
        }
    }else if(a<32){
        for(int i=0; i<a; i++){  //add Static elements of Smallvector to new vector
            newVector.StaticBuffer[i] = this->StaticBuffer[i];
            newVector.size++;
        }     //fill rest of the Static elements with toAdd's Static elements
        for(int i=0; i<32-a; i++){
            newVector.StaticBuffer[a+i] = toAdd.StaticBuffer[i];
            newVector.size++;
        }

        newVector.ExpandCapacity(a+b-32); //expand DynamicBuffer of newVector
        if(b<32){
            for(int i=0; i<a+b-32; i++){
                newVector.DynamicBuffer[newVector.dynamicSize+i] = toAdd.StaticBuffer[32-a+i];
            }
            newVector.dynamicSize = newVector.dynamicSize+a+b-32;
        }else if(b>32){
            for(int i=0; i<a+b-32; i++){ //add rest of the static elements of toAdd vector
                newVector.DynamicBuffer[newVector.dynamicSize+i] = toAdd.StaticBuffer[32-a+i];
            }
            newVector.dynamicSize = newVector.dynamicSize+a+b-toAdd.dynamicSize-32;
            for(int i=0; i<toAdd.dynamicSize; i++){  //add dynamic elements of toAdd vector
                newVector.DynamicBuffer[newVector.dynamicSize+i] = toAdd.DynamicBuffer[i];
            }
            newVector.dynamicSize = newVector.dynamicSize + toAdd.dynamicSize;
        }
    }
    return newVector;
}
const SmallVector& SmallVector::operator=(const SmallVector &taken){
    if(size != taken.size){  //if sizes are not equal
        size = taken.size;  //the needed size
        delete [] DynamicBuffer; //delete this->dynamic buffer
        if(size>32){   //if size bigger than 32 take space for dynamicbuffer
            DynamicBuffer = new int[taken.dynamicSize];
        }
    }
    for(int i=0; i<32; i++){  //copy Static elements
        StaticBuffer[i] = taken.StaticBuffer[i];
    }
    if(size>32){  //if size bigger than 32 copy dynamic elements
        for(int j=0; j<size-32; j++){
            DynamicBuffer[j]=taken.DynamicBuffer[j];
        }
    }
    return *this; //return *this for chain operations
}

SmallVector SmallVector::operator*(const int mult) const{
    SmallVector multed;
    if(size*mult<32){  //if our vector's size after multiplication less than 32
        for(int i=0; i<mult; i++){ // I used nested for loops
            for(int j=0; j<size; j++){  // How to explain nested for loops :D I can't
                multed.StaticBuffer[i*size+j]=StaticBuffer[j]; //This second loop will executed mult times.
                multed.size++; //Every element of StaticBuffer will be repeated mult times;
            }
        }
    }else if(size<32){ //if size is less than 32
        int x=32/size; //for example if our size=7, x=4 in this case which means we can repeat our vec. 4 times in StaticBuffer
        for(int i=0; i<x; i++){  //Repeat vector in StaticBuffer x times
            for(int j=0; j<size; j++){
                multed.StaticBuffer[i*size+j]=StaticBuffer[j];
                multed.size++; //increase size of multedvector
            }
        }
        for(int j=0; j<32%size; j++){ //now we need to add 7*4=28, 32-28=4 elements to StaticBuffer, which is 32%7=4;
            multed.StaticBuffer[size*x+j]=StaticBuffer[j];
            multed.size++; //increase size
        }
        multed.ExpandCapacity(size*mult-32); //rest of the elements will be added to DynamicBuffer, expand DynamicBuffer
        for(int i=0; i<size*mult-32; i++){ // Rest of the elements: 7*6(consider mult=6)-32=10 elements to DynamicBuffer
        /*We added 4 elements to StaticBuffer already so we need to continue with 5th element which is StaticBuffer[4] and
        rest of the elements will be [5],[6],[0],[1],...... so we need modular arithmetic in 7 base*/
            multed.DynamicBuffer[i] = StaticBuffer[(32%size+i)%size]; //(32%7+i)%7 = [4],[5],....
            multed.dynamicSize++; //increase dynamicSize;
        }
    }else if(size>32){ //if size is greater than 32;
        for(int i=0; i<32; i++){  //first copy static elements to multe.Static elements
            multed.StaticBuffer[i] = StaticBuffer[i];
            multed.size++; //increase size
        }
        multed.ExpandCapacity(mult*size-32); //expand capacity
        for(int j=0; j<dynamicSize; j++){  //also add dynamic elements to DynamicBuffer of multed
                multed.DynamicBuffer[j] = DynamicBuffer[j];
                multed.dynamicSize++; //increase dynamicSize of multed
        }
        //now we have multed=vector, so we need to repeat this vector mult-1 times since we already have 1times
        for(int i=0; i<mult-1; i++){
            for(int k=0; k<32; k++){ //add Statick elements to dynamic elements of multed
                multed.DynamicBuffer[multed.dynamicSize] = StaticBuffer[k];
                multed.dynamicSize++;
            }
            for(int l=0; l<size-32; l++){ //add dynamic elements to dynamic elements of multed
                multed.DynamicBuffer[multed.dynamicSize] = DynamicBuffer[l];
                multed.dynamicSize++;
            }
        }
    }
    return multed;
}


#endif // SMALLVECTOR_H_INCLUDED
