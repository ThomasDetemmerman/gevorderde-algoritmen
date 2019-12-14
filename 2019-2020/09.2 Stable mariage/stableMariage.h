//
// Created by Thomas on 14/12/2019.
//

#ifndef INC_09_2_STABLE_MARIAGE_STABLEMARIAGE_H
#define INC_09_2_STABLE_MARIAGE_STABLEMARIAGE_H

#include <vector>
#include <stack>
#include "Persoon.h"

using namespace std;

class stableMariage {
private:
    vector<Persoon> mannen;
    vector<Persoon> vrouwen;
    Persoon * geefPersoon(string naam, char geslacht);
    bool tryToMatch(Persoon * zoekendeMan, Persoon * mogelijkeVrouw,  deque<Persoon *> &singleMannen);
    bool manIsAlSingle(string naam, deque<Persoon *> &singleMannen);
public:
    stableMariage(vector<Persoon> mannen, vector<Persoon> vrouwen);
    void solve();
    void printResults();
};

stableMariage::stableMariage(vector<Persoon> m, vector<Persoon> v) : mannen(m), vrouwen(v){}

void stableMariage::solve() {
    deque<Persoon *> singleMannen;
    for (int i = 0; i < mannen.size(); ++i) {
        singleMannen.push_back(&(mannen[i]));
    }

    while(!singleMannen.empty()){
        Persoon* zoekendeMan = singleMannen.front(); singleMannen.pop_front();
        vector<string> nogTeOnderzoekenPartners = zoekendeMan->geefResterendeOpties();
        bool match = false;
        int counter = 0;
        while(!match){
            Persoon* mogelijkeVrouw = geefPersoon(nogTeOnderzoekenPartners[counter], 'v');
            match = tryToMatch(zoekendeMan, mogelijkeVrouw, singleMannen);
            counter++;
        }
    }

}

Persoon* stableMariage::geefPersoon(string naam, char geslacht) {
    if(geslacht == 'v'){
        for (int i = 0; i < vrouwen.size(); ++i) {
           if(vrouwen[i].naam == naam){
               return &(vrouwen[i]);
           }
       }
    }
    if(geslacht == 'm'){
        for (int i = 0; i < mannen.size(); ++i) {
            if(mannen[i].naam == naam){
                return &(mannen[i]);
            }
        }
    }
    return nullptr;
}


bool stableMariage::tryToMatch(Persoon * zoekendeMan, Persoon * mogelijkeVrouw,  deque<Persoon *> &singleMannen) {
    int matchSuccesvol;
    if(mogelijkeVrouw->verkiestDezeManBovenHuidige(zoekendeMan->naam)){
        matchSuccesvol = true;
        if(!mogelijkeVrouw->laatstepartner.empty() && !manIsAlSingle(mogelijkeVrouw->laatstepartner, singleMannen)){
            singleMannen.push_front(geefPersoon(mogelijkeVrouw->laatstepartner, 'm'));
            std::cout << mogelijkeVrouw->laatstepartner << " wordt single want ";
        }
        mogelijkeVrouw->laatstepartner = zoekendeMan->naam;
        zoekendeMan->laatstepartner = mogelijkeVrouw->naam;
        std::cout << zoekendeMan->naam << " is samen met " << mogelijkeVrouw->naam << std::endl;
    } else {
        std::cout << zoekendeMan->naam << " vraagt " << mogelijkeVrouw->naam << " maar is afgewezen" << std::endl;
        matchSuccesvol = false;
    }
    return  matchSuccesvol;
}

bool stableMariage::manIsAlSingle(string naam, deque<Persoon *> &singleMannen) {
    auto it = singleMannen.begin();
    bool presentInList = false;
    while(it != singleMannen.end() && !presentInList){
        if((*it)->naam == naam){
            presentInList = true;
        }
        it++;
    }
    return presentInList;
}

void stableMariage::printResults() {
    std::cout << std::endl << "resultaten:" <<std::endl;
    for(auto vrouw: vrouwen){
        std::cout << vrouw.naam << " - " << vrouw.laatstepartner << std::endl;
    }
}


#endif //INC_09_2_STABLE_MARIAGE_STABLEMARIAGE_H
