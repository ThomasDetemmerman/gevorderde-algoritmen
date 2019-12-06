//
// Created by Thomas on 06/12/2019.
//

#ifndef INC_10_7_TERNAIRE_ZOEKBOMEN_TERNAIREBOOM_H
#define INC_10_7_TERNAIRE_ZOEKBOMEN_TERNAIREBOOM_H
#include <string>
#include <fstream>
#include <sstream>
#include "Knoop.h"

using std::unique_ptr;
using std::string;
using std::ostream;
using std::make_unique;
using std::ofstream;
using std::ostringstream;




class TernaireBoom: public unique_ptr<Knoop>{

public:
    TernaireBoom();
    TernaireBoom(unique_ptr<Knoop> uniquePtr);

    void teken(const char *bestandsnaam);
    void voegToe(string data);
private:
    string tekenrec(ostream &uit, int &nullteller);
};

TernaireBoom::TernaireBoom(unique_ptr<Knoop> uniquePtr): unique_ptr<Knoop>(move(uniquePtr)){};
TernaireBoom::TernaireBoom(){};

void TernaireBoom::voegToe(string data) {
    data = data + "$";
    TernaireBoom * current = this;
    for(int i=0; i < data.size(); i++){
        if(*current == nullptr){
            *current = move(make_unique<Knoop>(data[i]));

        } else {
            if(data[i] == (*current)->teken){
                current = static_cast<TernaireBoom *>(&((*current)->m));
            } else if (data[i] < (*current)->teken){
                current = static_cast<TernaireBoom *>(&((*current)->l));
                i--; //if the character didnt match, we have to check the same character agian. Therefore we undo the i++ is the for(;;)
            } else {
                current = static_cast<TernaireBoom *>(&((*current)->r));
                i--;
            }
        }

    }

}
/////////////////// /////////////////// ///////////////////
///////////////////        teken        ///////////////////
/////////////////// /////////////////// ///////////////////


void TernaireBoom::teken(const char *bestandsnaam)
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\n";
    this->tekenrec(uit, knoopteller);
    uit << "}";
}


string TernaireBoom::tekenrec(ostream &uit, int &nullteller)
{
    ostringstream wortelstring;
    if (!*this)
    {
        wortelstring << "null" << ++nullteller;
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        wortelstring << '"' << (*this)->teken << '"';
        uit << wortelstring.str() << "[label=\"" << (*this)->teken << "\"]";
        uit << ";\n";
        string linkskind = (*this)->l.tekenrec(uit, nullteller);
        string rechtskind = (*this)->r.tekenrec(uit, nullteller);
        string middenkind = (*this)->m.tekenrec(uit, nullteller);
        uit << wortelstring.str() << " -> " << linkskind << ";\n";
        uit << wortelstring.str() << " -> " << middenkind << ";\n";
        uit << wortelstring.str() << " -> " << rechtskind << ";\n";
    };
    return wortelstring.str();
}




#endif //INC_10_7_TERNAIRE_ZOEKBOMEN_TERNAIREBOOM_H
