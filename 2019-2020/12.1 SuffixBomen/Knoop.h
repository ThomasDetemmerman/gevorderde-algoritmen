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
public:
    Knoop *staart;
    int beginIndex;
    int eindIndex;
    int startIndexOfEntireSuffix;
    vector<unique_ptr<Knoop>> kinderen;
    Knoop(int suffixBeginIndex, int suffixEindIndex, int startIndexOfEntireSuffix,  Knoop *staartpointer);
    Knoop *voegToe(int prefixLenghte, int suffi, int startIndexOfEntireSuffix, Knoop *);
    bool isBlad();
};

Knoop::Knoop(int suffixBeginIndex, int suffixEindIndex, int startIndexOfEntireSuffix_, Knoop *staartpointer) : beginIndex{suffixBeginIndex},
                                                                                eindIndex{suffixEindIndex},
                                                                                startIndexOfEntireSuffix{startIndexOfEntireSuffix_},
                                                                                staart{staartpointer} {}

Knoop *Knoop::voegToe(int prefixLenghte, int suffixOfChild, int startIndexOfEntireSuffix, Knoop *sp) {
    if (prefixLenghte > 0) {
        kinderen.push_back(move(make_unique<Knoop>(beginIndex + prefixLenghte, eindIndex,startIndexOfEntireSuffix, this)));
    }
    kinderen.push_back(move(make_unique<Knoop>(suffixOfChild + prefixLenghte, eindIndex, startIndexOfEntireSuffix, sp)));
    this->eindIndex = beginIndex + prefixLenghte;
    return &(*(kinderen.back()));
}

bool Knoop::isBlad(){
    return kinderen.size() <= 0;

}
#endif //B_SUFFIXBOMEN_KNOOP_H
