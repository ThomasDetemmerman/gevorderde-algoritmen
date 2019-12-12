//
// Created by Thomas on 12/12/2019.
//

#ifndef INC_10_3_BINARY_TRIE_BINARYTRIE_H
#define INC_10_3_BINARY_TRIE_BINARYTRIE_H

#include <string>
#include "varbitpatroon.h"
#include "Blad.h"
#include "NietBlad.h"

using std::string;
using std::unique_ptr;
using std::move;

class Knoop;
typedef unique_ptr<Knoop> Knoopptr;

class BinaryTrie : public Knoopptr {
public:
    BinaryTrie();

    BinaryTrie(Knoopptr&& ptr);

    void voegToe(string woord);

    void teken(const char *bestandsnaam);
};
BinaryTrie::BinaryTrie(){};
BinaryTrie::BinaryTrie(Knoopptr&& ptr): unique_ptr<Knoop>(move(ptr)) {};

void BinaryTrie::voegToe(string woord) {
    Knoopptr nieuwBlad = make_unique<Blad>(woord);

    if(!*this){
        *this = move(nieuwBlad); //requires constructor: BinaryTrie(Knoopptr ptr);
        return;
    }

    Varbitpatroon vbp(woord);
    Knoopptr * current = this;
    int diepte = 0;


    //we zitten in de laatste NietBlad. Twee opties zijn mogelijk
    // a) toevoegen in een lege positie
    // b) er is  een blad en die moet nu zakken tot we aan de bit zitten die verschilt tov toe te voegen bitpatroon
    Knoopptr *dest = this;
    if(!(*this)->isBlad()){
        //indien de volgende bestaat en het is geen blad, dan gaan we er naar toe
        while(static_cast<NietBlad*>((*current).get())->geefKind(vbp.geefbit(diepte)) && !static_cast<NietBlad*>((*current).get())->geefKind(vbp.geefbit(diepte))->isBlad()){
            current = &(static_cast<NietBlad*>((*current).get())->geefKind(vbp.geefbit(diepte)));
            diepte++;
        }
        dest =  &(static_cast<NietBlad*>((*current).get())->geefKind(vbp.geefbit(diepte)));
    }


    if(*dest == nullptr){
        *dest = move(nieuwBlad);
    } else {
        //ons oude blad gaan we moven naar een tijdelijke locatie om straks op zijn nieuwe positie toe te voegen.
        Knoopptr oudBlad = move(*dest);
        Blad * oudBladptr = static_cast<Blad*>(oudBlad.get());
        Knoopptr* prev = nullptr;
        while(oudBladptr->bit.geefbit(diepte) == vbp.geefbit(diepte)){
            prev = dest;
            *dest = move(make_unique<NietBlad>());
            dest =  &(static_cast<NietBlad*>(dest->get())->geefKind(vbp.geefbit(diepte)));
            diepte++;
        }

        static_cast<NietBlad*>(prev->get())->plaatsKind(oudBladptr->bit.geefbit(diepte),move(oudBlad));
        static_cast<NietBlad*>(prev->get())->plaatsKind(vbp.geefbit(diepte),move(nieuwBlad));

    }

}

void BinaryTrie::teken(const char *bestandsnaam) {
    std::ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\n";
    (*this)->tekenrec(uit, knoopteller, 0);
    uit << "}";
}


#endif //INC_10_3_BINARY_TRIE_BINARYTRIE_H
