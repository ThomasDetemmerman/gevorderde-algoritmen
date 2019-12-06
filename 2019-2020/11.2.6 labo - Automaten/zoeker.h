//
// Created by Thomas on 25/11/2019.
//

#ifndef LABO_9___AUTOMATEN_ZOEKER_H
#define LABO_9___AUTOMATEN_ZOEKER_H

#include "regexp11.h"
#include "thompsonna.h"
#include <queue>
#include <ostream>
#include <fstream>
using std::set;
using std::cout;
using std::endl;
using std::queue;
using std::iostream;
using std::pair;

class ZoekerNA {
public:

    ZoekerNA(const ThompsonNA& thompson, const ThompsonNA& thompsonReversed):thompsonNa(std::move(thompson)), thompsonNaReversed(std::move(thompsonReversed)){};
    queue<pair<int,int>> zoek(const string &data);
private:
    const ThompsonNA& thompsonNa;
    const ThompsonNA& thompsonNaReversed;
    bool exist(const set<int> toestanden,int element) const;
    void printSet(const set<int> &toestanden) const;
    void enableEpsilonNeighbours(set<int> &toestanden);
    vector<int> vindIndexen(const string &data, const ThompsonNA &thompsonna);
};

vector<int> ZoekerNA::vindIndexen(const string & data,const ThompsonNA& thompsonna){
    vector<int> results;
    set<int> states;
    set<int> statesThatShouldBeActiveInNextIteration;
    states.insert(0);
    enableEpsilonNeighbours(states);
    for (int j = 0; j < data.length(); ++j) {
        char teken = data[j];
        for (int i = 0; i < thompsonna.geefAantalVerbindingen(); i++) {
            auto verbinding = thompsonna.operator[](i);
            if (exist(states, verbinding.geefBron()) && teken == verbinding.geefKarakter()) {
                statesThatShouldBeActiveInNextIteration.insert(verbinding.geefDoel());
            }
        }
        states = statesThatShouldBeActiveInNextIteration;
        states.insert(0);
        enableEpsilonNeighbours(states);
        auto iterator = states.find(thompsonna.geefAantalVerbindingen() - 1);
        if (iterator != states.end()) {
           // std::cout << "match found, returning " << j << endl;
            results.push_back(j);
            std::cout << results.size();
            states.erase(iterator);
        }
        statesThatShouldBeActiveInNextIteration.clear();
    }
    return results;
}

queue<pair<int,int>> ZoekerNA::zoek(const string & data){
    vector<int> eindIndexen = vindIndexen(data, thompsonNa);
    vector<int> beginOffsets;
    for(int eindIndex: eindIndexen){
        string datab = data;
        datab = datab.substr(0, eindIndex+1);
        reverse(datab.begin(), datab.end());

        vector<int> tmp = vindIndexen(datab, thompsonNaReversed);
        assert(tmp.size()!=0);
        beginOffsets.push_back(tmp.back()); //pop back, we willen namelijk de langste match
    }

    std::queue<pair<int,int>> results;
    for (int i = 0; i < eindIndexen.size(); ++i) {
        int beginOffset = beginOffsets[i];
        int eindIndex = eindIndexen[i];
        std::pair<int,int> p(eindIndex-beginOffset, beginOffset);
        results.push(p);
    }
    return results;
}

// returns true if value is present and was erased
// returns false if value is not present in set
bool ZoekerNA::exist(set<int> toestanden, int element) const{
    return toestanden.find(element) != toestanden.end();
}

/*
 *  will enable all neighbours connected with epsillon edge.
 */
void ZoekerNA::enableEpsilonNeighbours(set<int>& toestanden){
    int debugcounter =0;
    for(int i=0; i < thompsonNa.geefAantalVerbindingen(); i++){

        if(toestanden.find(thompsonNa.operator[](i).geefBron()) != toestanden.end() && thompsonNa.operator[](i).geefKarakter() == epsilon){
            toestanden.insert(thompsonNa.operator[](i).geefDoel());
            debugcounter++;
        }
    }
    //std::cout << debugcounter << " staten zijn geactiveerd dankzij epsilon verbindingen" << endl;
}

void ZoekerNA::printSet(const set<int> &toestanden) const{
    set<int>::const_iterator it;
    std::cout << "states contains:";
    for (it=toestanden.begin(); it!=toestanden.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';

}




#endif //LABO_9___AUTOMATEN_ZOEKER_H
