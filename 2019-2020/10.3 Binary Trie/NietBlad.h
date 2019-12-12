//
// Created by Thomas on 12/12/2019.
//

#ifndef INC_10_3_BINARY_TRIE_NIETBLAD_H
#define INC_10_3_BINARY_TRIE_NIETBLAD_H

#include <memory>
#include <sstream>
#include <iostream>
#include "Knoop.h"
#include "Blad.h"

using namespace std;

class Knoop;
typedef unique_ptr<Knoop> Knoopptr;

class NietBlad : public Knoop {
private:
    void testChar(char i);
public:
    Knoopptr links;
    Knoopptr rechts;

    NietBlad(){};
    virtual bool isBlad() { return false; }
    virtual string tekenrec(ostream &uit, int &knoopteller, int diepte);

    Knoopptr &geefKind(bool linkerkind);

    void plaatsKind(bool linkerkind, Knoopptr &&kind);
};

string NietBlad::tekenrec(ostream &uit, int &knoopteller, int diepte) {
    ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';

    // HARDCODED 100 HIER: verwijderen achteraf
    uit << wortelstring.str() << "[label=\"" << "." << "\"]";
    uit << ";\n";
    vector<Knoopptr*> kind = {&links, &rechts};
    for (int i = 0; i < 2; i++) {
        if (*kind[i]) {
            string res = (*kind[i])->tekenrec(uit, knoopteller, diepte + 1);
            uit << wortelstring.str() << " -> " << res << ";\n";
        } else {
            ostringstream wortelstring2;
            wortelstring2 << '"' << ++knoopteller << '"';

            uit << wortelstring2.str() << " [shape=point];\n";
            uit << wortelstring.str() << " -> " << wortelstring2.str() << ";\n";
        }
    }

    return wortelstring.str();
}

void NietBlad::plaatsKind(bool linkerkind, Knoopptr&& kind){
    if (linkerkind)
        links = move(kind);
    else
        rechts = move(kind);
}
Knoopptr &NietBlad::geefKind(bool linkerkind)
{
    if (linkerkind)
        return links;
    else
        return rechts;
};


#endif //INC_10_3_BINARY_TRIE_NIETBLAD_H
