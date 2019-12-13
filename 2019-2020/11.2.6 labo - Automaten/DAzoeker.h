//
// Created by Thomas on 13/12/2019.
//

#ifndef LABO_9___AUTOMATEN_DAZOEKER_H
#define LABO_9___AUTOMATEN_DAZOEKER_H

#include "thompsonna.h"
#include <map>

using namespace std;
class DAzoeker {

public:
    DAzoeker(ThompsonNA NA);
    vector<vector<int>> overgangstabel;

private:
    map<set<int>, int> statenmap;
    ThompsonNA NA;
    map<char, int> tekens;
    void convertNA2DA();
    void zoekAlleOvergansSymbolen();
    void voegEpsilonBurenToe(set<int> &set);
    void zoekEpsilonRec(set<int> &set, int statenbit);
    set<int> signaal(set<int> set, char overgansteken);
    void printTransitionTable();
};

DAzoeker::DAzoeker(ThompsonNA na): NA(na) {
    zoekAlleOvergansSymbolen();
    convertNA2DA();
    printTransitionTable();
}

void DAzoeker::convertNA2DA() {

    map<set<int>,int> staatIDmapper; // in een NA is een staat een verzameling van bits. Hier worden deze verzameld in een set "toestand" die gemapt wordt op een nummer. Deze nummer wordt de staat in de DA.
    stack<set<int>> DEZ;


    int toestandsID = 0;
    set<int> toestand = {0};
    voegEpsilonBurenToe(toestand);

    DEZ.push(toestand);
    staatIDmapper.emplace(toestand, toestandsID++);

    while(!DEZ.empty()){
        toestand = DEZ.top();
        DEZ.pop();

        for(auto const& [overgansteken, val]: tekens){
            set<int> nieuweToestand = signaal(toestand, overgansteken);
            voegEpsilonBurenToe(nieuweToestand);
            if(staatIDmapper.find(nieuweToestand) == staatIDmapper.end()){
                staatIDmapper.emplace(nieuweToestand, toestandsID++);
                DEZ.push(nieuweToestand);
            }

            //update overganstabel
            int huidigeToestandID = staatIDmapper[toestand];
            int toestandsIDdest = staatIDmapper[nieuweToestand];

            //sla op in overganstabel
            if(this->overgangstabel.size() < toestandsID){
                vector<int> v(tekens.size());
                overgangstabel.emplace_back(v);

            }
            overgangstabel[huidigeToestandID][tekens[overgansteken]] = toestandsIDdest;
            printTransitionTable();

        }
    }
}

void DAzoeker::zoekAlleOvergansSymbolen(){
    int teller = 0;
    for(int i =0; i  < NA.geefAantalVerbindingen(); i++){
        if( NA.operator[](i).geefKarakter() != epsilon){
            tekens[NA.operator[](i).geefKarakter()] = teller++;
        }
    }
}

void DAzoeker::zoekEpsilonRec(set<int> &set, int statenbit) {
    for(int i=0; i < NA.geefAantalVerbindingen(); i++){
        auto verbinding = NA.operator[](i);
        if(verbinding.geefBron() == statenbit && verbinding.geefKarakter() == epsilon){
            set.insert(verbinding.geefDoel());
            zoekEpsilonRec(set, verbinding.geefDoel());
        }
    }
}

void DAzoeker::voegEpsilonBurenToe(set<int> &staat) {
    for(int statenbit : staat){
        zoekEpsilonRec(staat, statenbit);
    }

}

//  input: oudeToestand & overgansteken
//  output: nieuwe toestand die bekomen wordt indien 'overgansteken' de inputwaarde geweest zou zijn.
set<int> DAzoeker::signaal(set<int> oudeToestand, char overgansteken) {
    set<int> nieuweToestand;
    for(int i=0; i < NA.geefAantalVerbindingen(); i++){
        auto verbinding = NA.operator[](i);
        if(oudeToestand.find(verbinding.geefBron()) != oudeToestand.end() && verbinding.geefKarakter() == overgansteken){
            nieuweToestand.insert(verbinding.geefDoel());
        }
    }
    return nieuweToestand;
}

void DAzoeker::printTransitionTable() {
//print uit de overgangstabel

    std::cout << "\t";
    for(auto const& [overgansteken, val]: tekens){
        std::cout << overgansteken << "\t";
    }
    std::cout << std::endl;
    for (int j = 0; j < tekens.size(); ++j) {
        std::cout << "–––––";
    }
    std::cout << std::endl;
    for (int i = 0; i < overgangstabel.size(); i++) {
        std::cout << i << " | ";
        for (int item: overgangstabel[i]) {
            std::cout << item << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

#endif //LABO_9___AUTOMATEN_DAZOEKER_H
