#ifndef BTREE_H
#define BTREE_H

#include "schijf.h"

#include <iostream>

using std::endl;
using std::cerr;
using std::ostream;
using std::cout;
using std::flush;

// Dit bestand bevat code en hoofdingen van ...
template<class T, class D, unsigned int m>
class Btree;
template<class T, class D, unsigned int m>
class Bknoop;

// Betekenis m: zie cursus


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T, class D, unsigned int m>
class Bknoop
{
    friend class Btree<T, D, m>;
public:
    Bknoop<T, D, m>()
    {
	// temp is nodig om te verhinderen dat de compiler dit negeert
        int temp = (4 / (-4 + crashtest));
        crashtest++;
    };

    const Bknoop<T, D, m>& operator=(const Bknoop<T, D, m>& b)
    {
        k = b.k;
        isblad = b.isblad;

        for (unsigned int i=0;i<k;i++)
        {
            sleutel[i] = b.sleutel[i];
            data[i] = b.data[i];
        }

        if (!isblad)
        {
            for (unsigned int i=0; i<=k; i++)
            {
                index[i] = b.index[i];
            }
        }

        return *this;
    }

    T sleutel[m];
    D data[m];
    blokindex index[m + 1];
    unsigned int k;
    bool isblad;
    //eigen
    void voegToe(T sleutel, D data);

private:
    static int crashtest;
     void schrijf(ostream & out) const; 

friend ostream& operator<<(ostream & out, const Bknoop<T, D, m>; & b){
    b.schrijf(out);
    return out;
    }       
};

template<class T, class D, unsigned int m>
int Bknoop<T, D, m>::crashtest = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////         BTREE           /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<class T, class D, unsigned int m>
class Btree
{
    typedef Bknoop<T, D, m> Knoop;
public:
    Btree(Schijf<Knoop>& s):schijf(s){
        wortel.k=0;
        wortel.isblad=true;
        wortelindex=schijf.schrijf(wortel);
    }
    //eigen
    
    void voegToe(T, D);
    void memoryDump();
    knoop zoek(T);

private:
   Schijf<Knoop>& schijf;
   Knoop wortel;
   blokindex wortelindex;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////         BTREE : functies         ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//eigen
template<class T, class D,unsigned int m>
void Btree<T, D, m>::voegToe(T sleutel, D data){
    //we beginnen uiteraart volledig van boven en zullen vervolgens gebruik maken
    //van de recursieve eigenschap nl. deelbomen.
    
    //voorlopig enkel root
    wortel.voegToe(sleutel,data);
}


template<class T, class D,unsigned int m>
void Btree<T, D,m>::memoryDump(){
    Knoop current_root = wortel;    
    blokindex current_blok_index = wortelindex; 
    cout << "dumping " << current_root.k << " keys" << endl;
    cout << "keys: ";
    cout << current_root;
    cout << endl << endl;
}

template<class T, class D,unsigned int m>
Knoop Btree<T, D,m>::zoek(T sleutel){
     Knoop current = wortel;    
     bool done=false;
     while(!done){
         int i=0;
         while (i < current.k && sleutel > current.sleutels[i]) {
			i++;
		}
        
        if(current.sleutel[i]==sleutel){
            done=true;
            
        }else if(current.isblad){
            done=true;
        }else{
            //current is een uitvoerparameter. De oude knoop wordt overschreven door een nieuwe die van onze gesimuleerde disk gehaald word.
           schijf.lees(current, current.index[i+1]);

        }
     }
     return current;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////         BKnoop : functies         ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<class T, class D,unsigned int m>
void Bknoop<T, D, m>::voegToe(T sl, D data){
        int counter = 0;
    // zolang er k sleutels zijn en de gezochte sleutel is kleiner dan huidig sleutel
    while(counter < k && sleutel[counter] != sl){
        counter++;
        cout << counter;
    }
    if(sleutel[counter] == sl){
        throw "Sleutel bestaat al";
    }   
    else if(k == m){
        throw "Knoop is vol";
    }
    else{
        //toevoegen van sleutel op de juiste plaats zodat een geordende lijst verzekerd zou zijn.
        k++;
        //todo: beter zoekalgoritme zoals binary search gebruiken
        //todo: verplaatsen naar een functie
        int j=m-1;  //beginnen vanachteren
        /*while(sleutel[j-1] < sleutel){
            sleutel[j] = move(sleutel[j-1]);
        }*/
        //to do: move big obj
         this->sleutel[k] = sl;
         this->data[k] = std::move(data);
         cout << "added " << sl << " -> " << data << endl;
    }

}

 template<class T, class D,unsigned int m>
void Bknoop<T, D, m>::void schrijf(ostream & out) const{
    for(int i=0; i<k; i++){
       cout << current_root.sleutel[i] << "\t";
    }
    cout << endl;
    for(int i=0; i<k; i++){
       cout << current_root.data[i] << "\t";
    }
}

#endif