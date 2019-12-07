//
// Created by Thomas on 30/11/2019.
//

#ifndef INC_10_PATRICIATRIES_KNOOP_H
#define INC_10_PATRICIATRIES_KNOOP_H

#include <ostream>

using namespace std;

class Knoop {
public:
    virtual bool isBlad() = 0;
    virtual string tekenrec(ostream &uit, int &knoopteller) = 0;
};


#endif //INC_10_PATRICIATRIES_KNOOP_H
