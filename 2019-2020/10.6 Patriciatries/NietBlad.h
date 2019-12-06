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
    Knoopptr kind[26];
    int testIndex;
    NietBlad(int verschilIndex): testIndex{verschilIndex}{};

    virtual bool isBlad() { return false; }
    virtual string tekenrec(ostream &uit, int &knoopteller, int diepte);


    Knoopptr *geefKind(char i);

    void voegToe(Knoopptr &kptr, char i);

    bool geeftKind(char i);

    Blad *geefWillekeurigKind();

    void voegNietBladToe(Knoopptr &kptr, int i);
};

string NietBlad::tekenrec(ostream &uit, int &knoopteller, int diepte) {
    ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';

    // HARDCODED 100 HIER: verwijderen achteraf
    uit << wortelstring.str() << "[label=\"" << testIndex << "\"]";
    uit << ";\n";

    for (int i = 0; i < 26; i++) {
        if (this->kind[i]) {
            string res = this->kind[i]->tekenrec(uit, knoopteller, diepte + 1);
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

Knoopptr *NietBlad::geefKind(char i) {
    testChar(i);
    return &(kind[i-97]);
}

void NietBlad::voegToe(Knoopptr &kptr, char i) {
    testChar(i);
    kind[i-97] = move(kptr);
}

void NietBlad::voegNietBladToe(Knoopptr &kptr, int i) {
    kind[i] = move(kptr);
}

bool NietBlad::geeftKind(char i){
    testChar(i);
    Knoopptr *kindknoop = &(kind[i-97]);
    bool result = (*kindknoop != NULL);
    return result;
}

void NietBlad::testChar(char i){
    if(!(i >= 97 && i <= 123)){
        std::cout << "char " << i << " with integer value" << (int)i <<" is an illegal character. Only alfabetic values are allowed" << std::endl;
    }
    assert(i >= 97 && i <= 123);
}

Blad *NietBlad::geefWillekeurigKind() {
    bool found = false;
    int i=0;
    while((kind[i]) == NULL && i < 26){
        i++;
    }
    if(i == 26){
        std::cout << "geen willekeurig kind is beschikbaar";
        assert(false);
    }
    return static_cast<Blad *>(&(*kind[i]));
}

#endif //INC_10_PATRICIATRIES_NIETBLAD_H
