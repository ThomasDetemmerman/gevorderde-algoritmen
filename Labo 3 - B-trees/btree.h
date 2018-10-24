#ifndef BTREE_H
#define BTREE_H

#include "schijf.h"

#include <iostream>

using std::endl;
using std::cerr;
using std::ostream;
using std::cout;

// Dit bestand bevat code en hoofdingen van ...
template<class T, class D, unsigned int m>
class Btree;
template<class T, class D, unsigned int m>
class Bknoop;

// Betekenis m: zie cursus

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

private:
    static int crashtest;
};

template<class T, class D, unsigned int m>
int Bknoop<T, D, m>::crashtest = 0;

template<class T, class D, unsigned int m>
class Btree
{
    typedef Bknoop<T, D, m> Knoop;
public:
    Btree(Schijf<Knoop>& s):schijf(s){
        wortel.k=0;
        wortel.isblad=true;
        wortelindex=schijf.schrijf(wortel);
    };
    void voegtoe(T & sleutel, D & data);
    bool zoek(T & sleutel, Bknoop<T, D, m> * zoekKnoop, blokindex & parent);
    
private:
   Schijf<Knoop>& schijf;
   Knoop wortel;
   blokindex wortelindex;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////         Btree  : functies         ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class T, class D, unsigned int m>
void Btree<T, D, m>::voegtoe(T & sleutel, D & data)
{
	cout << "Voegtoe: " << sleutel << "->" << data << endl;
	blokindex index = 0;
	Bknoop<T, D, m>* knoop = &wortel;
	if (zoek(sleutel, knoop, index))
	{
		cout << "Bestaat al!" << endl;
	}
	else
	{
		cout << "Bestaat nog niet!" << endl;
		if (knoop->k == 0){	//nog geen elementen in de tree
			knoop->sleutel[knoop->k] = sleutel;
			knoop->data[knoop->k] = data;
			schijf.herschrijf(*knoop, wortelindex);
			knoop->k++;
		}
		else if (knoop->k < m){
			knoop->sleutel[knoop->k] = sleutel;
			knoop->data[knoop->k] = data;
			schijf.herschrijf(*knoop, index);
			knoop->k++;
		}
		while (knoop->k == m){
			Bknoop<T, D, m> * nieuwe = new Bknoop<T, D, m>();
			for (int i = m / 2 + 1; i < m; i++){
				nieuwe->sleutel[(i - m / 2) - 1] = knoop->sleutel[i];
				if (!knoop->isblad){
					nieuwe->index[(i - m / 2) - 1] = knoop->index[i];
				}
				knoop->k = (m / 2);
			}
			Bknoop<T, D, m> * parent = 0;
			while (index != 0){
				this->schijf.lees(*parent, index);
				if (parent->k < m)
					break;
			}
			bool isNieuwe = false;	//nieuwe root of niet
			if (parent == 0){
				isNieuwe = true;
				parent = new Bknoop<T, D, m>();
			}
			blokindex b = this->schijf.schrijf(*nieuwe);
			parent->sleutel[parent->k] = knoop->sleutel[(m / 2)];
			parent->index[parent->k] = b;
			parent->k++;	
			if (isNieuwe){
				this->wortelindex = this->schijf.schrijf(*parent);
				this->wortel = *parent;
			}
			else{
				this->schijf.herschrijf(*parent, index);
			}
			knoop = parent;
		}
	}

}

/*
* is eerder bool exists()
* return true als het bestaat, anders false.
*/
template <class T, class D, unsigned int m>
bool Btree<T, D, m>::zoek(T & sleutel, Bknoop<T, D, m> * zoekKnoop, blokindex & parentIndex)
{
	cout << "Zoek: " << sleutel << endl;
	int i = 0;
    // i < zoekKnoop->k                  //zolang er waarden zijn in de knoop
    // zoekKnoop->sleutel[i] < sleutel   //zolang de sleutel kleiner is als wat we zoeken
	while (i < zoekKnoop->k && zoekKnoop->sleutel[i] < sleutel)
	{
		i++;
	}
    // als i niet het einde van de rij bereikt heeft (anders error omdat i niet bestaat?)
    // en de sleutel wel gevonden hebben
	if (i < m && zoekKnoop->sleutel[i] == sleutel)
	{
		return true;
	}
    // indien we in een blad zitten kunnen we niet verder doen met de recursie.
    // een oplossing was niet gevonden.
	if (zoekKnoop->isblad)
	{
		return false;
	}
    // volgende data opvragen
    // schrijf.lees zal *zoekKnoop overschrijven met zoekKnoop->index[i]
	schijf.lees(*zoekKnoop, zoekKnoop->index[i]);
    // recursief verder zoeken.
	return zoek(sleutel, zoekKnoop, zoekKnoop->index[i]);
}

#endif