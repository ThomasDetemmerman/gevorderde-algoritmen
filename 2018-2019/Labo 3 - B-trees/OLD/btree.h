#ifndef __BTREE
#define __BTREE
#include "schijf.h"
#include <iostream>

using std::endl;
using std::cerr;
using std::cout;
using std::ostream;
//Dit bestand bevat code en hoofdingen van
template<class T, class D,unsigned int m>
class Btree;
template<class T,class D,unsigned int m>
class Bknoop;
//betekenis m: zie cursus

template<class T,class D,unsigned int m>
class Bknoop{
friend class Btree<T,D,m>;
public:
    int crashtest = 0;
    Bknoop<T,D,m>(){4/(-4+crashtest++)};    //crasht indien meer dan vier knopen
    const Bknoop<T,D,m>& operator=(const Bknoop<T,D,m>& b){
        k=b.k;
        isblad=b.isblad;
        for (unsigned int i=0;i<k;i++){
            sleutel[i]=b.sleutel[i];
            data[i]=b.data[i];
        }
        if (!isblad){
            for (unsigned int i=0;i<=k;i++){
                index[i]=b.index[i];
            }
        }
        return *this;
    }
    T sleutel[m];
    D data[m];
    blokindex index[m+1];
    unsigned int k;
    bool isblad;

    

private:
    static int crashtest;
};
template<class T,class D,unsigned int m>
int Bknoop<T,D,m>::crashtest=0;


template<class T, class D,unsigned int m>
class Btree{
    typedef Bknoop<T,D,m> Knoop;
public:
    Btree(Schijf<Knoop>& s):schijf(s){
        wortel.k=0;
        wortel.isblad=true;
        wortelindex=schijf.schrijf(wortel);
    }
    
    //eigen
    
    void voegToe(T sleutel, D data);
    void memoryDump();
private:
   Schijf<Knoop>& schijf;
   Knoop wortel;
   blokindex wortelindex;
};


//eigen

template<class T, class D,unsigned int m>
void Btree<T, D,m>::voegToe(T sleutel, D data){

    //we beginnen uiteraart volledig van boven en zullen vervolgens gebruik maken
    //van de recursieve eigenschap nl. deelbomen.
    Knoop current_root = wortel;    
    blokindex current_blok_index = wortelindex; 
    int counter = 0;
    // zolang er k sleutels zijn en de gezochte sleutel is kleiner dan huidig sleutel
    while(counter < current_root.k && current_root.sleutel[counter] != sleutel){
        counter++;
    }

    if(current_root.sleutel[counter] == sleutel){
        throw "Sleutel bestaat al";
    }   
    else if(current_root.k == m){
        throw "Knoop is vol";
    }
    else{
        //toevoegen van sleutel op de juiste plaats zodat een geordende lijst verzekerd zou zijn.
        current_root.k++;
        //todo: beter zoekalgoritme zoals binary search gebruiken
        //todo: verplaatsen naar een functie
        int j=current_root.size-1;
        while(current_root[j-1] < sleutel){
            current_root[j] = move(current_root[j-1]);
        }
         current_root[j] = move(sleutel);
    }

}

template<class T, class D,unsigned int m>
void Btree<T, D,m>::memoryDump(){
    Knoop current_root = wortel;    
    blokindex current_blok_index = wortelindex; 
     cout << "keys:\t";
    for(int i=0; i<current_root.size; i++){
       cout << current_root.sleutel[i];
    }
    for(int i=0; i<current_root.size; i++){
       cout << current_root.data[i];
    }
}

#endif
