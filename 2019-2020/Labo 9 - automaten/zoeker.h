//
// Created by Thomas on 25/11/2019.
//

#ifndef LABO_9___AUTOMATEN_ZOEKER_H
#define LABO_9___AUTOMATEN_ZOEKER_H

#include "regexp11.h"
#include "thompsonna.h"
#include <queue>
using std::set;
using std::cout;
using std::endl;
using std::queue;

class ZoekerNA {
public:

    ZoekerNA(const ThompsonNA& thompson):thompsonNa(std::move(thompson)){};
    queue<int> zoek(const string & data);
private:
    const ThompsonNA& thompsonNa;
    bool exist(const set<int> toestanden,int element) const;
    void printSet(const set<int> &toestanden) const;
    void enableEpsilonNeighbours(set<int> &toestanden);


};

queue<int> ZoekerNA::zoek(const string & data){
    queue<int> results;
    set<int> states;
    set<int> statesThatShouldBeActiveInNextIteration;
    states.insert(0);
    enableEpsilonNeighbours(states);
    for (int j = 0; j < data.length(); ++j) {
        char teken = data[j];
        for(int i=0; i < thompsonNa.geefAantalVerbindingen(); i++){
            auto verbinding = thompsonNa.operator[](i);
            if(exist(states,verbinding.geefBron()) && teken == verbinding.geefKarakter()){
                statesThatShouldBeActiveInNextIteration.insert( verbinding.geefDoel());
            }
        }
        states = statesThatShouldBeActiveInNextIteration;
        states.insert(0);
        enableEpsilonNeighbours(states);
        auto iterator = states.find(thompsonNa.geefAantalVerbindingen()-1);
        if(iterator != states.end()){
            results.push(j);
            states.erase(iterator);
        }
        statesThatShouldBeActiveInNextIteration.clear();
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
