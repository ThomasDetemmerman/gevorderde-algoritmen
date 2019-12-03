//
// Created by Thomas on 03/12/2019.
//

#ifndef B_SUFFIXBOMEN_KNOOP_H
#define B_SUFFIXBOMEN_KNOOP_H

#include <vector>
#include <string>
#include <sstream>

using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::ostream;

class Knoop {
private:
    bool isFirstChild();

public:


    Knoop *staart;

    int beginIndex;
    int eindIndex;
    vector<unique_ptr<Knoop>> kinderen;

    Knoop(int suffixBeginIndex, int suffixEindIndex, Knoop *staartpointer);


    Knoop *voegToe(int prefixLenghte, int suffi, Knoop *);
};

Knoop::Knoop(int suffixBeginIndex, int suffixEindIndex, Knoop *staartpointer) : beginIndex{suffixBeginIndex},
                                                                                eindIndex{suffixEindIndex},
                                                                                staart{staartpointer} {
}

Knoop *Knoop::voegToe(int prefixLenghte, int suffixOfChild, Knoop *sp) {
    if (prefixLenghte > 0) {
        kinderen.push_back(move(make_unique<Knoop>(beginIndex + prefixLenghte, eindIndex, this)));
    }
    kinderen.push_back(move(make_unique<Knoop>(suffixOfChild + prefixLenghte, eindIndex, sp)));
    this->eindIndex = beginIndex + prefixLenghte;
    return &(*(kinderen.back()));
}

bool Knoop::isFirstChild() {
    bool isFirstChild = true;
    int i = 0;
    while (isFirstChild && i < kinderen.size()) {
        if (kinderen[i] != nullptr) {
            isFirstChild = false;
        }
    }
    return isFirstChild;
}


#endif //B_SUFFIXBOMEN_KNOOP_H
