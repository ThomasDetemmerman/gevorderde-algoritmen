//
// Created by Thomas on 03/12/2019.
//

#ifndef B_SUFFIXBOMEN_SUFFIXBOOM_H
#define B_SUFFIXBOMEN_SUFFIXBOOM_H

#include <string>
#include <sstream>
#include <fstream>
#include "Knoop.h"

using namespace std;

class suffixboom {
private:
    string data;
    unique_ptr<Knoop> root;
    string tekenrec(ostream &uit, int &knoopteller, Knoop* knoop);
    string getSuffix(Knoop *knoop);
public:
    suffixboom(string data_);
    void teken(const char *bestandsnaam);
    Knoop * voegToe(int suffix, Knoop* prev);
    int getLongestCommonPrefixLenght(int start1, int start2);


};

suffixboom::suffixboom(string data_) : data(data_+"$"), root(make_unique<Knoop>(data.size(),data.size() , nullptr)){
    Knoop* prev = nullptr;
    for(int i = data.size()-1; i >= 0; i--){
        prev = voegToe(i, prev);
        teken("output.dot");
    }
}

Knoop* suffixboom::voegToe(int suffix,  Knoop* prev) {
    std::cout << "adding: " << data.substr(suffix, data.length())<< std::endl;
    if(prev == nullptr){
        std::cout << "first value, adding " << data.substr(suffix) << " to root " << std::endl;
       return this->root->voegToe(0, suffix, this->root.get());
    }
    Knoop * current = prev->staart;
    while(current->beginIndex > 0 && data.substr(current->beginIndex, 1) != data.substr(suffix, 1) && current->staart){
        std::cout << "terug gaan via staart ptr want : " << data.substr(current->beginIndex, 1) << " != " << data.substr(suffix, 1) << std::endl;
        current = current->staart;
    }

    int prefixLengte = getLongestCommonPrefixLenght(suffix, current->beginIndex);
    std::cout << ((current->beginIndex == data.size()) ? "root " : "knoop met " + getSuffix(current)) << " krijgt kind met suffix " << data.substr(suffix, data.size()) << std::endl;
    return current->voegToe(prefixLengte, suffix, prev);



}

int suffixboom::getLongestCommonPrefixLenght(int start1, int start2){
    int counter = 0;
    while(start1 < data.size() && start2 < data.size() && data[start1] == data[start2] ){
        start1++;
        start2++;
        counter++;
    }
    return counter;
}

string suffixboom::getSuffix(Knoop* knoop) {
    return data.substr(knoop->beginIndex, knoop->eindIndex-knoop->beginIndex);
}

/////////////////////////////////////////////////////////////////////////////////////////////
 //                               teken
/////////////////////////////////////////////////////////////////////////////////////////////

void suffixboom::teken(const char *bestandsnaam)
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\n";
    tekenrec(uit, knoopteller, this->root.get());
    uit << "}";
}

string suffixboom::tekenrec(ostream &uit, int &knoopteller, Knoop *knoop) {

    std::ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';

    if(knoop->beginIndex < 0){
        uit << wortelstring.str() << "[label=\"" << "."  << "\"]";
    } else {
        uit << wortelstring.str() << "[label=\"" << getSuffix(knoop) << "\"]";
    }

    uit << ";\n";
    for(int i=0; i <  knoop->kinderen.size(); i++){
        if( knoop->kinderen[i] != nullptr){
            Knoop * kind = knoop->kinderen[i].get();
            string linkskind = tekenrec(uit, knoopteller, kind);
            uit << wortelstring.str() << " -> " << linkskind << ";\n";
        }
    }
    return wortelstring.str();
}



#endif //B_SUFFIXBOMEN_SUFFIXBOOM_H
