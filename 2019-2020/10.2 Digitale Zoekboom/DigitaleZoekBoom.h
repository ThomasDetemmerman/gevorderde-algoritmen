#ifndef __Zoekboom_H
#define __Zoekboom_H
#include <cstdlib>
#include <iostream>
#include <queue>
#include <memory>
#include <functional>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <string>
#include <iostream>
#include "zoekknoop.h"

using std::cerr;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::pair;
using std::string;
using std::unique_ptr;
using std::move;
using std::make_unique;

template <int T>
class zoekKnoop;

template <int T>
class DigitaleZoekboom : public unique_ptr<zoekKnoop<T>>
{
public:

    DigitaleZoekboom(){};                                    //default
    DigitaleZoekboom(unique_ptr<zoekKnoop<T>> && ptr) : unique_ptr<zoekKnoop<T>>(std::move(ptr)){}
    DigitaleZoekboom<T> & operator=(unique_ptr<zoekKnoop<T>> && ptr){
        unique_ptr<zoekKnoop<T>>::operator=(std::move(ptr));
        return *this;
    }

    /*DigitaleZoekboom(bitset<T> bitpatroon){
        unique_ptr<zoekKnoop<T>> tmp = move(std::make_unique<zoekKnoop<T>>(bitpatroon));
        *this = move(tmp);
    }*/

    void teken(const char *bestandsnaam);
    string tekenrec(ostream &uit, int &knoopteller);


    void voegToe(string woord);
    DigitaleZoekboom<T>* zoek(const string woord);
    DigitaleZoekboom<T>* zoekRec(std::bitset<T> patroon, int diepte);
};


template<int T>
void DigitaleZoekboom<T>::voegToe(string woord) {
    std::bitset<T> bitpatroon(woord);
    DigitaleZoekboom *  dest = zoek(woord);
    *dest = move(make_unique<zoekKnoop<T>>(bitset<T>(woord)));
}

template<int T>
DigitaleZoekboom<T> *DigitaleZoekboom<T>::zoek(const string woord) {
    if(*this == nullptr){
        return this;
    }
    return zoekRec( std::bitset<T>(woord), 0 );
}



template <int T>
void DigitaleZoekboom<T>::teken(const char *bestandsnaam)
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\n";
    this->tekenrec(uit, knoopteller);
    uit << "}";
}

template <int T>
string  DigitaleZoekboom<T>::tekenrec(ostream &uit, int &knoopteller)
{
    ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';
    if (!*this)
    {
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        uit << wortelstring.str() << "[label=\"" << (*this)->bitpatroon << "\"]";
        uit << ";\n";
        string linkskind = (*this)->links.tekenrec(uit, knoopteller);
        string rechtskind = (*this)->rechts.tekenrec(uit, knoopteller);
        uit << wortelstring.str() << " -> " << linkskind << ";\n";
        uit << wortelstring.str() << " -> " << rechtskind << ";\n";
    };
    return wortelstring.str();
}

template<int T>
DigitaleZoekboom<T> *DigitaleZoekboom<T>::zoekRec(std::bitset<T> patroon, int diepte) {
    if(patroon != (*this)->bitpatroon) {
        if (patroon[T - (diepte + 1)]) {
            if ((*this)->rechts == nullptr) {
                return &(*this)->rechts;
            } else {
                return (*this)->rechts.zoekRec(patroon, ++diepte);
            }
        } else {
            if ((*this)->links == nullptr) {
                return &(*this)->links;
            } else {
                return (*this)->links.zoekRec(patroon, ++diepte);
            }
        }
    }



}


#endif
