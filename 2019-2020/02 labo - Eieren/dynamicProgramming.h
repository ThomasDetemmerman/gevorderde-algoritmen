//
// Created by Thomas on 10/10/2019.
//

#ifndef GEVORDERDE_ALGORITMEN_MAIN_H
#define GEVORDERDE_ALGORITMEN_MAIN_H

#include <vector>
#include <iostream>

using namespace std;

class dynamicProgramming {

private:
    // elke rij is een verdiep
    // elke kolom is aantal eieren
    // [ ei ] [ verdiep ]
    vector<vector<int>> eggdrops;
    int totEi;
    int totverdiep;

public:
    dynamicProgramming(int aantalEieren, int aantalVerdiepen);

    void run();
    void print();
};

dynamicProgramming::dynamicProgramming(int aantalEieren, int aantalVerdiepen){
    totEi = aantalEieren;
    totverdiep = aantalVerdiepen;
    eggdrops.resize(totEi+1);   //geen idee waarom hier +1 moet staan. Anders maakt hij maar (totEi -1) aan. Je zou zeggen voor nul, maar waarom moet dat dan niet voor binneste lus?
    for(int rij=0; rij <= totverdiep; rij++){
        eggdrops[rij].resize(totverdiep);   // hier geen +1
    }
    run();
}

void dynamicProgramming::run(){
    //init phase
    for(int currentAantalEi=0; currentAantalEi <= totEi; currentAantalEi++) {
        eggdrops[currentAantalEi][0] = 0;
        eggdrops[currentAantalEi][1] = 1; // zou ook door tool berekend moeten worden denk ik.
    }
    for(int currentAantalVerdiepen=0; currentAantalVerdiepen <= totverdiep; currentAantalVerdiepen++) {
        eggdrops[0][currentAantalVerdiepen] = 0;
        eggdrops[1][currentAantalVerdiepen] = currentAantalVerdiepen;
    }

    //calculation phase

    // Matrix opvullen
    // Start bij 2 eieren want 0 en 1 hebben we al opgevuld
    for(int aantalEi = 2; aantalEi <= totEi; aantalEi++){
        for(int huidigverdiep=2; huidigverdiep <= totverdiep; huidigverdiep++){ //aantalEI mag starten op 1. Maar hoeft niet aangezien we heb bij init phase op 1 zetten.

            // Bij aanvang is het minimum oneindig groot
            int minimumEggDrops = INT_MAX;

            // Nu overlopen we alle mogelijke startpunten en
            // zoeken we het startpunt met de kleinste waarde
            for(int x=1; x<=huidigverdiep; x++){

                // Als het ei niet breekt, neem je de waarde uit de matrix bij hetzelfde aantal eieren
                // Maar je weet dat het ei niet breekt op de eerste x verdiepingen dus kijk je bij het
                // v-x
                int alsNietBreekt = eggdrops[aantalEi][huidigverdiep - x];

                // ALs het ei breekt, neem je de waarde uit de matrix bij 1 ei minder
                // aangezien je al 1x geworpen hebt op punt x moet je x-1 nemen
                // zodat x + (x-1) = x
                int alsBreekt = eggdrops[aantalEi - 1][x-1];

                // De waarde die gekozen wordt voor deze x is het maximum van eiBreekt en eiBreekt niet
                // We hadden al 1x geworpen om te zien of het ei breekt op x of niet
                // dus moeten we de waarde + 1 doen
                int currentEggDrops = 1 + max(alsBreekt, alsNietBreekt);

                // Check of gekozenWaarde het nieuwe minimum is
                if(currentEggDrops < minimumEggDrops){
                    minimumEggDrops = currentEggDrops;
                }

                // Vul het gevonden minimum in
                eggdrops[aantalEi][huidigverdiep] = minimumEggDrops;

            }
        }
    }
}

void dynamicProgramming::print() {
//Prints numeggs
    cout << "\t\t";
    for (int i = 0; i <= totEi; i++) {
        cout << i << "\t";
    }
    cout << endl  << "\t\t";
    for (int i = 0; i <= totEi; i++) {
        cout << "_" << "\t";
    }
    cout << endl;
    for (int j = 0; j <= totverdiep; j++) {
        cout << j << "\t |\t";
        for (int i = 0; i <= totEi; i++) {
            cout << eggdrops[i][j] << "\t";
        }
        cout << endl;
    }
}
#endif //GEVORDERDE_ALGORITMEN_MAIN_H
