//
// Created by Thomas on 12/12/2019.
//

#ifndef INC_11_2_6_3_MINIMALISTATIE_DA_DAMINIMIZER_H
#define INC_11_2_6_3_MINIMALISTATIE_DA_DAMINIMIZER_H


#include <set>
#include "graaf.h"

using namespace std;

class DAMinimizer {
public:
    DAMinimizer(GraafMetTakdata<GERICHT, char> DA, int SIZE, int alfabetsize);
    void minimize();
    void printEquivalentieGroupen();
    vector<vector<int>> equivalentiegroepen;
private:
    vector<vector<int>> overgangstabel;
    int SIZE;
    int alfabetsize;
    GraafMetTakdata<GERICHT, char> DA;

    void buildTransitionTable();

    void printTransitionTable();

    bool areEquivalent(int i, int j);




};

DAMinimizer::DAMinimizer(GraafMetTakdata<GERICHT, char> DA_, int s, int a) : DA(DA_), SIZE(s), alfabetsize(a) {
    buildTransitionTable();
    printTransitionTable();
}


void DAMinimizer::buildTransitionTable() {
//maak overgangstabel

    for (int i = 0; i < SIZE; i++) {
        vector<int> rij(alfabetsize, -1);
        for (auto buur: DA.operator[](i)) {
            int data = *(DA.geefTakdata(i, buur.first)) - 'a';
            rij[data] = buur.first;
        }
        overgangstabel.emplace_back(rij);
    }
}

void DAMinimizer::printTransitionTable() {
//print uit de overgangstabel
    std::cout << "\t";
    for (int j = 0; j < alfabetsize; ++j) {
        std::cout << (char) ('a' + j) << "\t";
    }
    std::cout << std::endl;
    for (int j = 0; j < alfabetsize; ++j) {
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

bool DAMinimizer::areEquivalent(int i, int j) {
    bool areEquivalent = true;
    int k = 0;
    while (areEquivalent && k < alfabetsize) {
        if (overgangstabel[i][k] == overgangstabel[j][k]) {
            k++;
        } else {
            areEquivalent = false;
        }
    }
    return areEquivalent;
}

void DAMinimizer::printEquivalentieGroupen() {
    for (int i = 0; i < equivalentiegroepen.size(); ++i) {

        std::cout << "{ ";
        for (auto teken: equivalentiegroepen[i]) {
            std::cout << teken << " ";
        }
        std::cout << "}";
        if (i != equivalentiegroepen.size() - 1) {
            std::cout << ",";
        }
    }
    std::cout << std::endl << std::flush;

}

void DAMinimizer::minimize() {
    //minimaliseer
    //init phase (zero equivalence)
    vector<int> finalStates = {SIZE - 1};
    vector<int> nonFinalStates;
    for (int k = 0; k < SIZE - 1; ++k) {
        nonFinalStates.emplace_back(k);
    }

    equivalentiegroepen = {nonFinalStates};

    bool somethingChanged = true;
    while (somethingChanged) {
        for (int j = 0; j < equivalentiegroepen[0].size(); j++) {
            printEquivalentieGroupen();
            somethingChanged = false;
            if (!areEquivalent(equivalentiegroepen[0][j], equivalentiegroepen[0][j + 1])) {
                somethingChanged = true;
                //kijken of er andere verzamelingen zijn waarmee deze dan wel equivalent is.
                int k = 1;

                while (k < equivalentiegroepen.size() && !areEquivalent(equivalentiegroepen[0][j], equivalentiegroepen[k][0])) {
                    k++;
                }
                equivalentiegroepen[0].erase(equivalentiegroepen[0].begin() + j - 1);

                if (k >=  equivalentiegroepen.size()) {
                    vector<int> tmp = { j };
                    equivalentiegroepen.emplace_back(tmp);

                } else {
                    equivalentiegroepen[k].emplace_back(j);

                }
            }
        }
    }
    equivalentiegroepen.emplace_back(finalStates);
}

#endif //INC_11_2_6_3_MINIMALISTATIE_DA_DAMINIMIZER_H
