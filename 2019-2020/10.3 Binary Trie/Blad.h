//
// Created by Thomas on 12/12/2019.
//

#ifndef INC_10_3_BINARY_TRIE_BLAD_H
#define INC_10_3_BINARY_TRIE_BLAD_H


#include <memory>
#include <string>
#include <sstream>
#include "Knoop.h"
#include "varbitpatroon.h"

using namespace std;

class Knoop;
typedef unique_ptr<Knoop> Knoopptr;

class Blad : public Knoop {
public:
    Varbitpatroon bit;
    string data;
    Blad( string data_) : data{data_}, bit(move(Varbitpatroon(data_))){};

    virtual bool isBlad() { return true; }


    virtual string tekenrec(ostream &uit, int &knoopteller, int diepte);
};



// teken functies
string Blad::tekenrec(ostream &uit, int &knoopteller, int diepte) {
    ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';
    uit << wortelstring.str() << "[label=\"" << data << "(" << bit << ")" << "\"]";
    return wortelstring.str();
}


#endif //INC_10_3_BINARY_TRIE_BLAD_H
