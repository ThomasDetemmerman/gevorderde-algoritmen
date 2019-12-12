#ifndef __BOYERMOORE_H
#define __BOYERMOORE_H

#include <queue>
#include <map>
#include <string>
#include <iostream>

typedef unsigned int uint;
typedef unsigned char uchar;
using namespace std;

class BoyerMoore {
public:
    BoyerMoore(const string &_naald);

//de zoekfunctie geeft in teller het aantal karaktervergelijkingen terug
    std::queue<int> zoek(const string &hooiberg, int teller);

    void printMRP();

private:
    vector<int> MRP;
    const string naald;
    //een character loopt van -127 tot 128
    const int ALFABET_SIZE = abs(-127) + 128;

    void print(string naald, int naaldindex, string hooiber, int hooibergindex);
};


BoyerMoore::BoyerMoore(const string &_naald) : naald(_naald) {
    map<char, int> MRPLookup;
    MRP = vector<int>(ALFABET_SIZE, -1);
    for (int i = 0; i < naald.length(); ++i) {
        MRP[naald[i] + 128] = i;
    }
}

void BoyerMoore::printMRP() {
    for (char i: naald) {
        std::cout << i << "\t";
    }
    std::cout << std::endl;
    for (char i: naald) {
        std::cout << MRP[i + 128] << "\t";
    }
    std::cout << std::endl;
}

std::queue<int> BoyerMoore::zoek(const string &hooiberg, int teller) {
    std::queue<int> matches;
    int hooibergIndex_startpoint = naald.length() - 1;
    int hooibergIndex = hooibergIndex_startpoint;
    int naaldIndex = naald.length() - 1;

    while (hooibergIndex <= hooiberg.length()) {
        //print(naald, naaldIndex, hooiberg, hooibergIndex);
        while (hooiberg[hooibergIndex] == naald[naaldIndex] && naaldIndex > 0) {
            hooibergIndex--;
            naaldIndex--;
            //print(naald, naaldIndex, hooiberg, hooibergIndex);
        }
        if (naaldIndex == 0 && naald[naaldIndex] == hooiberg[hooibergIndex]) {
            matches.push(hooibergIndex);
            hooibergIndex_startpoint++;
            hooibergIndex = hooibergIndex_startpoint;
            naaldIndex = naald.length() - 1;
            //print(naald, naaldIndex, hooiberg, hooibergIndex);
        } else {
            //print(naald, naaldIndex,hooiberg,hooibergIndex);
           // cout << "looking up " << hooiberg[hooibergIndex] << " it has " << MRP[hooiberg[hooibergIndex] + 128] << endl;

            int aantalOpschuiven = naald.length()-1 - MRP[hooiberg[hooibergIndex] + 128];
            //indien MRPval -1 is krijgen we naaldlengte -1 - - 1 = naaldlengte. maw, indien een teken niet voorkomt kunnen we ons patroon tot volledig na het niet voorkomende teken schuiven.
            //cout << "moving " << aantalOpschuiven << " possitions" << endl;
            //als het -1 is krijgen we naaldlengte-1 - - MRPvalue. Maw, we gaan de volledige woordlengte opschuiven want het teken komt toch niet voor in het patroon

            if (aantalOpschuiven <= 0) {
                // als je potron hebt en je zit aan de t die je wilt matchen met o in de teskt gaat hij de o rechts van jou geven
                //waardoor je een negatieve aantalOpschuiven hebt. Je weet dus niet dus kies je maar voor het veilige incrementeren.
                // dus niet enkel -1 maar ook alles kleiner dan -1 vallen in deze categorie
                hooibergIndex_startpoint++;
            } else {
                hooibergIndex_startpoint += aantalOpschuiven;
            }
            hooibergIndex = hooibergIndex_startpoint;
        }
        naaldIndex = naald.length() - 1;
    }
return matches;
}

void BoyerMoore::print(string naald, int naaldindex, string hooiber, int hooibergindex) {
    for (char c: naald) {
        std::cout << c;
    }
    cout << endl;
    for (int i = 0; i < naaldindex; ++i) {
        cout << " ";
    }
    cout << "^" << endl;
    for (char c: hooiber) {
        std::cout << c;
    }
    cout << endl;
    for (int i = 0; i < hooibergindex; ++i) {
        cout << " ";
    }
    cout << "^" << endl << endl;
}

#endif
    
