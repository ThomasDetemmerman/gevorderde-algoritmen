//
// Created by Thomas on 01/11/2019.
//

#ifndef INC_1_2_SPLAY_TREE_TOPDOWN_SPLAYTREE_H
#define INC_1_2_SPLAY_TREE_TOPDOWN_SPLAYTREE_H

#include "Knoop.h"
#include <memory>
#include <fstream>
#include <sstream>

using namespace std;

template <class Sleutel, class Data>
class Knoop;

template <class Sleutel, class Data>
class SplayTree: public unique_ptr<Knoop<Sleutel,Data>> {
public:
    void voegToe(Sleutel s, Data d);
    SplayTree<Sleutel, Data>*  zoek(Sleutel s, Data d);
    SplayTree & operator=(unique_ptr<Knoop<Sleutel,Data>> && ptr){
        unique_ptr<Knoop<Sleutel,Data>>::operator=(move(ptr));
        return *this;
    }
    void teken(const char *bestandsnaam);
    void schrijf(ostream &os) const;

private:
    void splay(Sleutel s);
    SplayTree<Sleutel, Data>* vindExtrema(bool min);
    void roteer(bool naarRechts);
    void merge(SplayTree<Sleutel, Data> && L, SplayTree<Sleutel, Data> && R);
    string tekenrec(ostream &uit, int &knoopteller);

};

template<class Sleutel, class Data>
void SplayTree<Sleutel, Data>::voegToe(Sleutel s, Data d) {
    if(!*this){
        *this = move(make_unique<Knoop<Sleutel,Data>>(s,d));
    } else {
        splay(s);
        SplayTree<Sleutel, Data> links, rechts;
        if((*this)->sleutel > s){
            links = move((*this)->links);
            rechts = move(*this);

        } else {
            rechts = move((*this)->rechts);
            links = move(*this);

        }
        *this = move(make_unique<Knoop<Sleutel,Data>>(s,d));
        (*this)->links = move(links);
        (*this)->rechts = move(rechts);
    }


}

template<class Sleutel, class Data>
SplayTree<Sleutel, Data>* SplayTree<Sleutel, Data>::zoek(Sleutel s, Data d) {

    return this;
}

template<class Sleutel, class Data>
void SplayTree<Sleutel, Data>::splay(Sleutel zoeksleutel) {
    SplayTree<Sleutel, Data> L;
    SplayTree<Sleutel, Data> R;

    if((*this)->sleutel == zoeksleutel){
        return;
    }

    // zig
    bool pIsLeft = ((*this)->sleutel > zoeksleutel);
    SplayTree<Sleutel, Data>   *p = &((*this)->geefKind(pIsLeft));
    if(*p == nullptr){
        return;
    }

    bool cIsLeft = ((*p)->sleutel > zoeksleutel);
    SplayTree<Sleutel, Data>  *c = &((*p)->geefKind(cIsLeft));
    SplayTree<Sleutel, Data> * dest;
    if(*c == nullptr){
        //zig
        //indien de knoop p links staat moeten we in L het maximum vinden

        if(pIsLeft){
            dest = L.vindExtrema(false);
        } else {
            dest = R.vindExtrema(true);
        }
        *dest = move(*this);
        *this = move(*p);

    } else {
        if(cIsLeft == pIsLeft){
            // zig zig
            roteer(cIsLeft);
            if(pIsLeft){
                dest = L.vindExtrema(false);
            } else {
                dest = R.vindExtrema(true);
            }
            *dest = move(*this);
            *this = move(*c);

        } else {
            //zig zag
            if(pIsLeft){
                dest = L.vindExtrema(false);
            } else {
                dest = R.vindExtrema(true);
            }
            *dest = move(*this);
            if(cIsLeft){
                dest = L.vindExtrema(false);
            } else {
                dest = R.vindExtrema(true);
            }
            *dest = move(*p);
            *this = move(*c);
        }

    }

    /// merge


    dest = L.vindExtrema(false);
    *dest = move((*this)->geefKind(true));
    dest = R.vindExtrema(false);
    *dest = move((*this)->geefKind(true));

    dest = (*this).vindExtrema(true);
    *dest = move(R);

    dest = (*this).vindExtrema(false);
    *dest = move(L);

}

template<class Sleutel, class Data>
SplayTree<Sleutel, Data> *SplayTree<Sleutel, Data>::vindExtrema(bool min) {
    SplayTree<Sleutel, Data>* extrema = this;

        while((*extrema) != nullptr){
            extrema = &((*extrema)->geefKind(min));
        }
    return extrema;
}


template<class Sleutel, class Data>
void SplayTree<Sleutel, Data>::roteer(bool naarRechts) {
    //je kan niet roteren op een lege (deel)boom
    // geen assert op this. Indien deze knoop niet bestaat kun je de functie  niet oproepen. Is *this beter?
    //assert(this) is useless, want this zal altijd wel naar iets wijzen.
    assert(*this);

    // koppel I los van P
    SplayTree<Sleutel, Data> pointerToI = move((*this)->geefKind(naarRechts));

    //a rotate function requires a child.
    // assert(pointerToI);
    if (!pointerToI) {
        return;
    }
    // rechter kind van I wordt linkerkind van P
    (*this)->geefKind(naarRechts) = move(pointerToI->geefKind(
            !naarRechts)); // to do: kan dit een segmetation fault opleveren? kan ik een lege pointer moven? antwoord: dit is ok.

    // P wordt nu linkerkind van I
    pointerToI->geefKind(!naarRechts) = move(*this);

    // zet I als nieuwe parent
    *this = move(pointerToI);
}

/*****************************************************************************

    Tools: teken & schrijf

*****************************************************************************/

template <class Sleutel, class Data>
void SplayTree<Sleutel, Data>::schrijf(ostream &os) const
{
    inorder([&os](const Knoop<Sleutel, Data> &knoop) {
        os << "(" << knoop.sleutel << " -> " << knoop.data << ")";
        os << "\n  Linkerkind: ";
        if (knoop.links)
            os << knoop.links->sleutel;
        else
            os << "-----";
        os << "\n  Rechterkind: ";
        if (knoop.rechts)
            os << knoop.rechts->sleutel;
        else
            os << "-----";
        os << "\n";
    });
}

template <class Sleutel, class Data>
void SplayTree<Sleutel, Data>::teken(const char *bestandsnaam)
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\n";
    this->tekenrec(uit, knoopteller);
    uit << "}";
}

template <class Sleutel, class Data>
string SplayTree<Sleutel, Data>::tekenrec(ostream &uit, int &knoopteller)
{
    ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';
    if (!*this)
    {
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        uit << wortelstring.str() << "[label=\"" << (*this)->sleutel << ":" << (*this)->data << "\"]";
        uit << ";\n";
        string linkskind = (*this)->links.tekenrec(uit, knoopteller);
        string rechtskind = (*this)->rechts.tekenrec(uit, knoopteller);
        uit << wortelstring.str() << " -> " << linkskind << ";\n";
        uit << wortelstring.str() << " -> " << rechtskind << ";\n";
    };
    return wortelstring.str();
}


#endif //INC_1_2_SPLAY_TREE_TOPDOWN_SPLAYTREE_H
