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
    string geefSubstring(Knoop *knoop);
    int getLongestCommonPrefixLenght(int start1, int start2);
    bool isPrefix(string a, string b);


public:
    suffixboom(string data_);
    void teken(const char *bestandsnaam);
    Knoop * voegToe(int suffix, Knoop* prev);
    vector<int> zoek(const string  &zoekterm);
    void diepteEerstZoeken(unique_ptr<Knoop> *pKnoop,vector<int> & resultaten );
    vector<int> geefStartIndexVanKinderen(unique_ptr<Knoop> *pKnoop);
};

suffixboom::suffixboom(string data_) : data(data_+"$"), root(make_unique<Knoop>(data.size(),data.size() , data.size(), nullptr)){
    //data input cleanup
    //data.erase(remove_if(data.begin(), data.end(), [](char c) { return !isalpha(c); } ), data.end());
    //std::cout  << data << std::flush;
    Knoop* prev = nullptr;
    for(int i = data.size()-1; i >= 0; i--){
        prev = voegToe(i, prev);
    }
}

Knoop* suffixboom::voegToe(int suffix,  Knoop* prev) {
    int startIndexOfEntireSuffix = suffix;
    std::cout << "adding: " << data.substr(suffix, data.length())<< std::endl;
    if(prev == nullptr){
        std::cout << "first value, adding " << data.substr(suffix) << " to root " << std::endl;
       return this->root->voegToe(0, suffix, startIndexOfEntireSuffix,this->root.get());
    }
    Knoop * current = prev->staart;
    while(current->beginIndex > 0 && data.substr(current->beginIndex, 1) != data.substr(suffix, 1) && current->staart){
        std::cout << "terug gaan via staart ptr want : " << data.substr(current->beginIndex, 1) << " != " << data.substr(suffix, 1) << std::endl;
        current = current->staart;
    }

    int prefixLengte = getLongestCommonPrefixLenght(suffix, current->beginIndex);
    std::cout << ((current->beginIndex == data.size()) ? "root " : "knoop met " + geefSubstring(current)) << " krijgt kind met suffix " << data.substr(suffix, data.size()) << std::endl;
    return current->voegToe(prefixLengte, suffix,startIndexOfEntireSuffix, prev);



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

string suffixboom::geefSubstring(Knoop* knoop) {
    return data.substr(knoop->beginIndex, knoop->eindIndex-knoop->beginIndex);
}

// is a a prefix of b
bool suffixboom::isPrefix(string a, string b){
    if(a.size() > b.size()){
        return false;
    }
    // a = tot
    // b = totaal
    b = b.substr(0, a.size());
    return (a==b);
}

/////////////////////////////////////////////////////////////////////////////////////////////
//                               zoek
/////////////////////////////////////////////////////////////////////////////////////////////

vector<int> suffixboom::zoek(const string  &zoekterm) {
    vector<int> resultaten;
    int eersteTeken = zoekterm[0];
    unique_ptr<Knoop> * current = &root;
    bool afdalen = true;
    int zoekdataindex = 0;
    while(afdalen){
        afdalen = false;

        for(int i=0; i < (*current)->kinderen.size(); i++) { // moet een while zijn
            unique_ptr<Knoop> * huidigKind = &(*current)->kinderen[i];
            std::cout << "processing:" << geefSubstring(huidigKind->get())[0] << " =?= " << zoekterm[zoekdataindex] << std::endl;
            if (geefSubstring(huidigKind->get())[0] == zoekterm[zoekdataindex]){
                // 3 scenarios
                // a) takdata =/= zoekterm -> afbreken
                // b) takdata prefix van zoekterm -> recursief verder gaan
                // c) zoekterm prefix van takdata -> alle kinderen zijn resultaat

                if(isPrefix(zoekterm, data.substr((*huidigKind)->startIndexOfEntireSuffix,(*huidigKind)->eindIndex- (*huidigKind)->startIndexOfEntireSuffix) )){
                    // scenario c
                    return geefStartIndexVanKinderen(huidigKind);}
                else if(isPrefix(geefSubstring(huidigKind->get()), zoekterm )){
                    // scenario b
                    current = huidigKind;
                    zoekdataindex += ((*current)->eindIndex - (*current)->beginIndex);
                    afdalen = true;
                    break;
                } else {
                    // scenario a
                    return resultaten;
                }
            }
        }
    }
    return resultaten;

}

vector<int> suffixboom::geefStartIndexVanKinderen(unique_ptr<Knoop> *pKnoop) {
    vector<int> resultaten;
    diepteEerstZoeken(pKnoop,  resultaten);
    return resultaten;
}

void suffixboom::diepteEerstZoeken(unique_ptr<Knoop> *pKnoop,vector<int> & resultaten ){
    if((*pKnoop)->isBlad()){
        resultaten.emplace_back((*pKnoop)->startIndexOfEntireSuffix);
    }else {
        for(int i=0; i <  (*pKnoop)->kinderen.size(); i++){
            diepteEerstZoeken(&(*pKnoop)->kinderen[i], resultaten);
        }
    }
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
        uit << wortelstring.str() << "[label=\"" << geefSubstring(knoop) << "\"]";
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
