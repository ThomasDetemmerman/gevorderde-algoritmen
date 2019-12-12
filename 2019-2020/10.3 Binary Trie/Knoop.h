//
// Created by Thomas on 12/12/2019.
//

#ifndef INC_10_3_BINARY_TRIE_KNOOP_H
#define INC_10_3_BINARY_TRIE_KNOOP_H


#include <ostream>

using namespace std;

class Knoop {
public:
    virtual bool isBlad() = 0;
    virtual string tekenrec(ostream &uit, int &diepte, int i) = 0;
};
#endif //INC_10_3_BINARY_TRIE_KNOOP_H
