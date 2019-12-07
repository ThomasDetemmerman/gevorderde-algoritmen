//
// Created by Thomas on 30/11/2019.
//

#ifndef INC_10_PATRICIATRIES_NIETBLAD_H
#define INC_10_PATRICIATRIES_NIETBLAD_H


#include <memory>
#include <sstream>
#include <iostream>
#include "Knoop.h"

using namespace std;

class Knoop;
typedef unique_ptr<Knoop> Knoopptr;

class NietBlad : public Knoop {
private:
    void testChar(char i);
public:
    Knoopptr l, r;
    NietBlad(){};

    virtual bool isBlad() { return false; }
    virtual string tekenrec(ostream &uit, int &knoopteller);

};

string NietBlad::tekenrec(ostream &uit, int &nullteller) {
    ostringstream wortelstring;
    if (!this)
    {
        wortelstring << "null" << ++nullteller;
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        wortelstring << '"' << nullteller << '"';
        uit << wortelstring.str() << "[label=\"" << "." << "\"]";
        uit << ";\n";
        string linkskind = l->tekenrec(uit, ++nullteller);
        string rechtskind = r->tekenrec(uit, ++nullteller);

        uit << wortelstring.str() << " -> " << rechtskind << ";\n";
        uit << wortelstring.str() << " -> " << linkskind << ";\n";

    };
    return wortelstring.str();
}



#endif //INC_10_PATRICIATRIES_NIETBLAD_H
