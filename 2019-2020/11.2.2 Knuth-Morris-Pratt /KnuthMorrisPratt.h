//
// Created by Thomas on 04/12/2019.
//

#ifndef C___KNUTH_MORRIS_PRATT__KNUTHMORRISPRATT_H
#define C___KNUTH_MORRIS_PRATT__KNUTHMORRISPRATT_H


#include <queue>
#include "prefix.h"

class KnuthMorrisPratt {
private:
    Prefix prefixTabel;
public:
    KnuthMorrisPratt(Prefix pt);
    queue<int> zoek(const string & tekst);
};

KnuthMorrisPratt::KnuthMorrisPratt(Prefix pt): prefixTabel(pt) {}

queue<int> KnuthMorrisPratt::zoek(const string &tekst) {
    queue<int> resultaten;
    int Pc = 0; //patrooncounter
    int Tc = 0; //tekstcounter

    while(Tc < tekst.size()){
        if(tekst[Tc] == prefixTabel.data[Pc]){
            Tc++;
            Pc++;
        }
        if(Pc == prefixTabel.data.size()){
            resultaten.push(Tc-Pc);
            //Pc=0; Tc++; //fout
            Pc = prefixTabel[Pc-1]; 
        } else if (Tc < tekst.size() && tekst[Tc] != prefixTabel.data[Pc] ){
            if(Pc == 0){
                Tc++;
            }
            else {
                //backtracking by using prefix function
                Pc = prefixTabel[Pc-1];
            }
        }
    }
    return resultaten;
};

#endif //C___KNUTH_MORRIS_PRATT__KNUTHMORRISPRATT_H
