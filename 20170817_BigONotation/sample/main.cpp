#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

class BigONotation {
    private:
        // private field
        int *array;  // pointer untuk menyimpan array
        int arrayLen=0; // banyaknya elemen yang disimpan dalam array

        // variabel helper untuk mengukur waktu eksekusi
        clock_t timeStart;
        clock_t timeEnd;

        // private method
        void swapIndex(int, int); //untuk menukar nilai dari dua posisi elemen

    public:
        // constructor & destructor
        BigONotation(int);
        ~BigONotation();

        // helper public methods
        int size(); // mengembalikan jumlah elemen dalam array
        void printArray(); // output semua elemen array

        // main methods
        void setValueAtIndex(int, int);
        bool linearSearch(int);
        void bubbleSort();
        bool binarySearch(int);
        void stdSort();
};

int main(){
    srand(time(NULL));

    // coding session #1
    BigONotation *d1 = new BigONotation(10000);
    BigONotation *d2 = new BigONotation(1000000);

    d1->linearSearch(5000);
    d2->linearSearch(5000);

    d1->setValueAtIndex(5000, rand()%d1->size());
    d1->setValueAtIndex(5000, rand()%d1->size());
    d1->setValueAtIndex(5000, rand()%d1->size());

    d2->setValueAtIndex(5000, rand()%d2->size());
    d2->setValueAtIndex(5000, rand()%d2->size());
    d2->setValueAtIndex(5000, rand()%d2->size());

    d1->linearSearch(5000);
    d2->linearSearch(5000);

    d1->bubbleSort();

    d1->binarySearch(5000);
    d1->binarySearch(100);

    d2->stdSort();
    //d3->printArray();


    delete d1;
    delete d2;
    // delete d3;
}

BigONotation::BigONotation(int num){
    array = new int[num];

    for(int i=0;i<num;i++){
        array[arrayLen++] = rand();
    }
}

BigONotation::~BigONotation(){
    delete array;
}

int BigONotation::size(){
    return arrayLen;
}

void BigONotation::swapIndex(int a, int b){
    array[a] = array[a]^array[b];
    array[b] = array[a]^array[b];
    array[a] = array[a]^array[b];
}

void BigONotation::printArray(){
    for(int i=0;i<arrayLen;i++)
        cout << array[i] << " ";
    cout << endl;
}

void BigONotation::setValueAtIndex(int val, int pos){
    array[pos] = val;
}

bool BigONotation::linearSearch(int x){
    bool found = false;

    timeStart = clock();
    for(int i=0; i< arrayLen; i++){
        if(x == array[i]){
            cout << "[Linear search] Found at index: " << i << endl;
            found = true;
            break;
        }
    }
    timeEnd = clock();

    if(!found){
        cout << "[Linear search] Not found in array" << endl;
    }

    cout << "[Linear search] Time elapsed: " << (double)(timeEnd - timeStart)*1000/CLOCKS_PER_SEC << "ms"  << endl;

    return found;
}

void BigONotation::bubbleSort(){
    timeStart = clock();

    for(int i=0; i<arrayLen;i++){
        for(int j=0;j<arrayLen;j++){
            if(array[j]>array[j+1])
                swapIndex(j, j+1);
        }
    }
    timeEnd = clock();

    cout << "[Bubble sort] Time elapsed: " << (double)(timeEnd - timeStart)*1000/CLOCKS_PER_SEC  << "ms" << endl;

}

bool BigONotation::binarySearch(int x){
    int start = 0, end = arrayLen-1;
    int searchIteration = 0;
    bool found = false;

    timeStart = clock();
    while(end > start+1){
        ++searchIteration;
        int mid = (start + end)/2;
        if (array[mid] < x){
            start = mid;
        }
        else if(array[mid] > x){
            end = mid;
        }
        else{
            cout << "[Binary search] Found at iteration " << searchIteration << endl;
            found = true;
            break;
        }
    }
    if(start != end){
        if(array[start] == x){
            cout << "[Binary search] Found at iteration " << searchIteration << endl;
            found = true;
        }
    }

    if(!found){
        cout << "[Binary search] Not found in array after iteration " << searchIteration << endl;
    }
    timeEnd = clock();

    cout << "[Binary search] Time elapsed: " << (double)(timeEnd - timeStart)/CLOCKS_PER_SEC << "ms" << endl;

    return found;
}

void BigONotation::stdSort(){
    timeStart = clock();

    sort(array, array+arrayLen);

    timeEnd = clock();

    cout << "[Std sort] Time elapsed: " << (double)(timeEnd - timeStart)/CLOCKS_PER_SEC << "ms" << endl;
}
