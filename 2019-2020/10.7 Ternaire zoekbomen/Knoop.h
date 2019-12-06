#ifndef INC_10_7_KNOOP_TERNAIREBOOM_H
#define INC_10_7_KNOOP_TERNAIREBOOM_H

#include "TernaireBoom.h"
using namespace std;
class Knoop;
typedef std::unique_ptr<Knoop> Knoopptr;

class Knoop {

friend class TernaireBoom;

public:
    Knoop(char t);
    Knoop();

protected:
    Knoopptr l;
    Knoopptr r;
    Knoopptr m;
    char teken;

    string tekenrec(ostream &uit, int &nullteller);
};

Knoop::Knoop(char t):teken(t){};
Knoop::Knoop(){};

string Knoop::tekenrec(ostream &uit, int &nullteller)
{
    ostringstream wortelstring;
    if (!this)
    {
        wortelstring << "null" << ++nullteller;
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        wortelstring << '"' << nullteller << '"';
        uit << wortelstring.str() << "[label=\"" << teken << "\"]";
        uit << ";\n";
        string linkskind = l->tekenrec(uit, ++nullteller);
        string middenkind = m->tekenrec(uit, ++nullteller);
        string rechtskind = r->tekenrec(uit, ++nullteller);

        uit << wortelstring.str() << " -> " << rechtskind << ";\n";
        uit << wortelstring.str() << " -> " << middenkind << ";\n";
        uit << wortelstring.str() << " -> " << linkskind << ";\n";

    };
    return wortelstring.str();
}

#endif