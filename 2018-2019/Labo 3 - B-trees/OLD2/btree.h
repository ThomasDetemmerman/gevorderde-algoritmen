#ifndef BTREE_H
#define BTREE_H

#include "schijf.h"
#include <string>
#include <iostream>
#include <stack>
#include <map>
#include <sstream>
#include <stack>
#include <queue>

using std::map;
using std::stack;
using std::string;
using std::endl;
using std::cerr;
using std::ostream;
using std::cout;
using std::flush;
using std::ifstream;

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
    //code uit apart bestand op intranet (btree-teken.h)
    string dotstring() const;
    //eigen
    void voegToe(T sleutel, D data);

private:
    static int crashtest;
     void schrijf(ostream & out) const; 

friend ostream& operator<<(ostream & out, const Bknoop<T, D, m> & b){
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
    //code uit apart bestand op intranet (btree-teken.h)
    void teken(const char * bestandsnaam) const;

    //eigen
    void voegToe(T, D);
    void memoryDump();
    Knoop zoek2(T);
    void splits(Knoop& , stack<blokindex>&);
    

private:
   Schijf<Knoop>& schijf;
   Knoop wortel;
   blokindex wortelindex;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////         BTREE : functies         ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
//eigen
template<class T, class D,unsigned int m>
void Btree<T, D, m>::voegToe(T sleutel, D data){
    //we beginnen uiteraart volledig van boven en zullen vervolgens gebruik maken
    //van de recursieve eigenschap nl. deelbomen.
    
    //voorlopig enkel root
    wortel.voegToe(sleutel,data);
}*/


template<class T, class D,unsigned int m>
void Btree<T, D,m>::memoryDump(){
    Knoop current_root = wortel;    
    blokindex current_blok_index = wortelindex; 
    cout << "dumping " << current_root.k << " keys" << endl;
    cout << "keys: ";
    cout << current_root;
    cout << endl << endl;
}
/*
template<class T, class D,unsigned int m>
Btree<T, D, m>::Knoop Btree<T, D,m>::zoek2(T sleutel){
     Bknoop<T, D, m> current = wortel;    
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
*/

template<class T, class D, unsigned int m>
void Btree<T, D, m>::voegToe(T sleutel, D data) {
	// Zoeken naar knoop waar de sleutel onder kan
	Knoop huidig = wortel;
	blokindex huidigeIndex = wortelindex;
	stack<blokindex> parents; // top element is de huidige locatie op schijf
	parents.push(huidigeIndex);
	std::cout << "---ADDING " << sleutel << ", " << data << "!" << std::endl;
	while (!huidig.isblad) {
		int i = huidig.k-1;
		std::cout << "i was " << i << ", with " << huidig.k << " keys in it ";
		
		while (i >= 0 && huidig.sleutel[i] > sleutel) {
			i--;
		}
		i++;
		
		int tmp;
		std::cout << "Conditon: " << sleutel << " > " << huidig.sleutel[i] << std::endl;
		std::cout << " and is now " << i << std::endl;
		std::cin >> tmp;
		huidigeIndex = huidig.index[i];
		parents.push(huidigeIndex);
		schijf.lees(huidig, huidigeIndex);
	}
	
	std::cout << "adding.1" << std::endl;
	
	huidig.voegToe(sleutel, data);
	
	std::cout << "adding.2" << std::endl;
	// Commit changes
	schijf.herschrijf(huidig, huidigeIndex);
	std::cout << "adding.3" << std::endl;
	
	// Update root in mem
	if (huidigeIndex == wortelindex) {
		wortel = huidig;
	}
	
	std::cout << "adding.4 - splits" << std::endl;

	// Controle vol zitten
	/*while (huidig.k == m) {
		std::cout << "adding.4.1" << std::endl;
		//std::cout << "TO DO: splits(huid,parents)";
        splits(huidig, parents);
	}*/
	std::cout << "done adding!" << std::endl;
}

template<class T, class D, unsigned int m>
void Btree<T, D, m>::splits(Knoop& huidig, stack<blokindex>& parents) {
    int midden = (m / 2) + (m % 2);
	std::cout << "SPLITTSS!!!" << std::endl;
	// Knoop splitsen
	Knoop broer;
	for (int start = midden + 1; start < m; start++) {
		broer.sleutel[start - midden - 1] = huidig.sleutel[start];
		broer.data[start - midden - 1] = huidig.data[start];
	}
	broer.k = midden - 1;
	huidig.k = midden - 1;
	broer.isBlad = huidig.isBlad;
	std::cout << "Is blad: " << broer.isBlad << std::endl;
	
	T sleutel = huidig.sleutel[midden];
	D data = huidig.data[midden];
	
	blokindex lChild = parents.top(); parents.pop();
	blokindex rChild = schijf.schrijf(broer);
	
	// Updaten van de originele huidige node
	schijf.herschrijf(huidig, lChild);
	
	if (parents.empty()) {
		huidig = *(new Knoop()); // Hergebruik huidige als nieuwe root
		huidig.isBlad = false;
		huidig.voegToe(sleutel, data);
		huidig.addChilds(sleutel, lChild, rChild);
		wortelindex = schijf.schrijf(huidig);
		wortel = huidig;
	} else {
		std::cout << "NOT Its a new root!" << std::endl;
		blokindex huidigeIndex = parents.top(); parents.pop();
		schijf.lees(huidig, huidigeIndex);
		huidig.addKeyValue(sleutel, data);
		huidig.addChilds(sleutel, lChild, rChild);
		schijf.herschrijf(huidig, huidigeIndex);
		if (huidigeIndex == wortelindex) {
			wortel = huidig;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////         BKnoop : functies         ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//called by Btree which has his own voegtoe function. After finding the correct position in the btree,
// this function adds the value at the right possition in de bknoop
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
void Bknoop<T, D, m>::schrijf(ostream & out) const{
    for(int i=0; i<k; i++){
       cout << this->sleutel[i] << "\t";
    }
    cout << endl;
    for(int i=0; i<k; i++){
       cout << this->data[i] << "\t";
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////         teken functies           ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class D,unsigned int m>
string Bknoop<T,D,m>::dotstring() const{
    std::ostringstream os;
//noot: voor debugdoelen is het vaak handig de indexen van de kinderen uit te schrijven
//vervang dan volgende lijn door
//        if (!isBlad){
    if (false){
        os<<"niet-blad: ";
    os<<" ("<<kind[0]<<")";
    for (int i=1; i<=aantalSleutels; i++ ){
        os<<sleutel[i]<<"("<<kind[i]<<") ";
    }
    }
    else{
    os<<"  ";
    for (int i=1; i<=aantalSleutels; i++ ){
        os<<sleutel[i]<<"  ";
    }
    }
    return os.str();
}

template<class T, class D, unsigned int m>
void Btree<T, D, m>::teken(const char * bestandsnaam) const{
        std::ofstream uit(bestandsnaam);
    //    ostream& uit=cerr;
        assert(uit);
        uit<<"digraph {\n";
    //    uit<<wortel.aantalSleutels<<"\n";
    //    uit<<wortel.isBlad<<"\n";
        //noot: beetje ineffici"ent: knopen worden twee keer ingelezen
        std::queue<std::pair<blokindex,string>> BEq;//queue voor breedte eerst;
        if (!wortel.isBlad)
            BEq.emplace(wortelindex,wortel.dotstring());
        uit<<"\""<<wortel.dotstring()<<"\" [shape=rectangle] [label=\""<<wortel.dotstring()<<"\"];\n";
        while (!BEq.empty()){
            Knoop ouder;
            schijf.lees(ouder,BEq.front().first);
            string& ouderstring=BEq.front().second;
            for (int i=0; i<=ouder.aantalSleutels; i++ ){
                Knoop kind;
                schijf.lees(kind,ouder.kind[i]);
                string kindstring=kind.dotstring();
                uit<<"\""<<kindstring<<"\" [shape=rectangle] [label=\""<<kindstring<<"\"];\n";
                uit<<"\""<<ouderstring<<"\" -> \""<<kindstring<<"\";\n";
                if (!kind.isBlad){
                    BEq.emplace(ouder.kind[i],kindstring);
                };
            }
            BEq.pop();
        };

        uit<<"}\n";
    }
#endif