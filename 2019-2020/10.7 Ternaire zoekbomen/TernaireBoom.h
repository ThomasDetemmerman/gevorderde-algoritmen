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

class Knoop;
typedef unique_ptr<Knoop> Knoopptr;

class TernaireBoom: public Knoopptr{
public:
    TernaireBoom();
    TernaireBoom(unique_ptr<Knoop> uniquePtr);

    void teken(const char *bestandsnaam);
    void voegToe(string data);
private:
    char afsluitteken = (char)1000;
};

TernaireBoom::TernaireBoom(unique_ptr<Knoop> uniquePtr): unique_ptr<Knoop>(move(uniquePtr)){};
TernaireBoom::TernaireBoom(){};

void TernaireBoom::voegToe(string data) {
    data = data + afsluitteken;
    Knoopptr * current = this;
    Knoopptr * prev = nullptr;
    Knoopptr * prevprev = nullptr;
    for(int i=0; i < data.size(); i++){
        if(*current == nullptr){
            //onderstaande if is waarschijnlijk niet zo mooi geprogrammeerd en crasht vermoedelijk in sommige situaties met korte woorden.
            if(prevprev != nullptr && (*prev)->teken == afsluitteken){
                (*prevprev)->r = move(make_unique<Knoop>(afsluitteken));
                 current = prev;
                teken("ternaireBoom.dot");
            }
            *current = move(make_unique<Knoop>(data[i]));
        }
        
        prevprev = prev;
        prev = current;
        if(data[i] == (*current)->teken){
            current =&((*current)->m);
        } else if (data[i] < (*current)->teken){
            current =&((*current)->l);
            i--; //if the character didnt match, we have to check the same character again. Therefore we undo the i++ is the for(;;)
        } else {
            current = &((*current)->r);
            i--; //if the character didnt match, we have to check the same character again. Therefore we undo the i++ is the for(;;)
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
    (*this)->tekenrec(uit, knoopteller);
    uit << "}";
}

#endif //INC_10_7_TERNAIRE_ZOEKBOMEN_TERNAIREBOOM_H
