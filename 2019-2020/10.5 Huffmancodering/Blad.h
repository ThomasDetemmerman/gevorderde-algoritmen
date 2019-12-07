//
// Created by Thomas on 30/11/2019.
//

#ifndef INC_10_PATRICIATRIES_BLAD_H
#define INC_10_PATRICIATRIES_BLAD_H

#include <memory>
#include <string>
#include <sstream>
#include "Knoop.h"
using namespace std;

class Knoop;
typedef unique_ptr<Knoop> Knoopptr;

class Blad : public Knoop {
public:
    char sleutel;
    Blad( char sleutel_) : sleutel{sleutel_}{};
    virtual bool isBlad() { return true; }
    virtual string tekenrec(ostream &uit, int &knoopteller);
};

// teken functies
string Blad::tekenrec(ostream &uit, int &knoopteller) {
    ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';

    uit << wortelstring.str() << "[label=\"" << sleutel << "\"]";

    return wortelstring.str();
}

#endif //INC_10_PATRICIATRIES_BLAD_H
