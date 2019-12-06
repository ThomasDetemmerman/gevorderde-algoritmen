#ifndef INC_10_7_KNOOP_TERNAIREBOOM_H
#define INC_10_7_KNOOP_TERNAIREBOOM_H

#include "TernaireBoom.h"

class TernaireBoom;
class Knoop {

friend class TernaireBoom;

public:
    Knoop(char t): teken(t){};
    Knoop(){};
    TernaireBoom l;
    TernaireBoom r;
    TernaireBoom m;
    char teken;
};
#endif