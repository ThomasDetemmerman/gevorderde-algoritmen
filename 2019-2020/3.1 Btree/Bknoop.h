//
// Created by Thomas on 11/10/2019.
//

#ifndef GEVORDERDE_ALGORITMEN_BTREE_H
#define GEVORDERDE_ALGORITMEN_BTREE_H

#include <iostream>
#include <vector>
#include <stack>
#include "schijf.h"

using namespace std;

template<class Sleutel, class Data>
class Bknoop {
private:


    int eigenSchijfpagina;

    int geefSchijfIDVanLinkerKind(int sleutelIndex) const;

    int geefSchijfIDVanRechterKind(int sleutelIndex) const;

    void setSchijfIDVanLinkerKind(int sleutelIndex, int schijfID);

    void setSchijfIDVanRechterKind(int sleutelIndex, int schijfID);


public:
    vector<Sleutel> sleutels;
    vector<Data> data;
    vector<blokindex> kinderen;
    bool isBlad = true;
    int m; //orde / aantal kinderen
    int k = 0; //huidig aantal sleutels. Dit is max m-1

    Bknoop(){};

    Bknoop(int orde) : m(orde) {};

    Bknoop(int orde, bool blad) : m(orde), isBlad(blad){};

    void voegToe(Sleutel s, Data d);

    bool contains(Sleutel &s);

    void voegToe(Sleutel, Data, int, int);

    void toString();


    void splits(stack<int> parents);


};


template<class Sleutel, class Data>
void Bknoop<Sleutel, Data>::voegToe(Sleutel s, Data d, int schijfIDLinkerKind, int schijfIDRechterKind) {
    Bknoop<Sleutel,Data>& knoop = zoek(s,d);
    knoop.k++;
    std::cout << "aantal sleutels in deze knoop: " << knoop.k << endl;
    knoop.sleutels.resize(knoop.k);   //k telt vanaf nul, maar de size moet dus plus 1 zijn.
    knoop.data.resize(knoop.k);
    knoop.kinderen.resize(knoop.k + 1);

    int i = knoop.k - 1;
    if ( knoop.isBlad) {
        while (i > 1 &&  knoop.sleutels[i - 1] > s) {
            knoop.sleutels[i] = move(knoop.sleutels[i - 1]);
            knoop.data[i] = move(knoop.data[i - 1]);
            knoop.setSchijfIDVanRechterKind(knoop.geefSchijfIDVanRechterKind(i - 1), i); //no need to move() int.
            i--;
        }
        knoop.sleutels[i] = s;
        knoop.data[i] = d;
        knoop.setSchijfIDVanLinkerKind(i, schijfIDLinkerKind);
        knoop.setSchijfIDVanRechterKind(i, schijfIDRechterKind);

        if ( knoop.k ==  knoop.m && knoop.k != 0) {
            std::cout << "splits" << endl;
            knoop.splits();
        }
    }

}

template<class Sleutel, class Data>
void Bknoop<Sleutel, Data>::toString() {
    std::cout << "\t";
    for (Sleutel s: sleutels) {
        std::cout << s << "\t";
    }
    std::cout << std::endl;

    std::cout << "\t";
    for (Data d: data) {
        std::cout << d << "\t";
    }
    std::cout << std::endl;
    std::cout << " ";
    for (int k: kinderen) {
        std::cout << k << "  ";


    }
 //herhaal recursief
    std::cout << std::endl;
    std::cout << std::endl;
    for (int k: kinderen) {
        Bknoop kind(m);
        if (k != -1) {
            this->lees(kind, k);
            kind.toString();
        }

    }

}

// Lijkt misschien beetje belachelijk om hiervan functies te maken. Maar het verduidelijkt wel veel. Moet je niet telkens
// opnieuw nadenken hoe de kinder arrays zijn opgebouwd tov de sleutels.
template<class Sleutel, class Data>
int Bknoop<Sleutel, Data>::geefSchijfIDVanLinkerKind(int sleutelIndex) const {
    return kinderen[sleutelIndex];
}

template<class Sleutel, class Data>
int Bknoop<Sleutel, Data>::geefSchijfIDVanRechterKind(int sleutelIndex) const {
    return kinderen[sleutelIndex + 1];
}

template<class Sleutel, class Data>
void Bknoop<Sleutel, Data>::setSchijfIDVanLinkerKind(int sleutelIndex, int schijfID) {
    kinderen[sleutelIndex] = schijfID;
}

template<class Sleutel, class Data>
void Bknoop<Sleutel, Data>::setSchijfIDVanRechterKind(int sleutelIndex, int schijfID) {
    kinderen[sleutelIndex + 1] = schijfID;
}

template<class Sleutel, class Data>
void Bknoop<Sleutel, Data>::splits(stack<int> parents) {


    // geval 1: knoop is de wortel
    if (parents.emtpy()) {
        int midden = (int) k / 2;

        Bknoop Lknoop(m);
        Lknoop.isBlad = true;
        for (int i = 0; i < midden; i++) {
            Lknoop.voegToe(sleutels[i], data[i], geefSchijfIDVanLinkerKind(i), geefSchijfIDVanRechterKind(i));
        }

        blokindex lknoopID = this->schrijf(Lknoop);

        Bknoop Rknoop(m);
        Rknoop.isBlad = true;
        for (int i = midden + 1; i < k; i++) {

            int sleutelTobemoved = sleutels[i];
            Rknoop.voegToe(sleutelTobemoved, data[i], geefSchijfIDVanLinkerKind(i),
                           geefSchijfIDVanRechterKind(i));
        }

        blokindex rknoopID = this->schrijf(Lknoop);

        Bknoop nieuweWortel(m);
        nieuweWortel.voegToe(sleutels[midden], data[midden], lknoopID, rknoopID);
        nieuweWortel.isBlad = false;
        *this = move(nieuweWortel);


    } else {
        int parent = parents.top();
        parents.pop();
        //todo
    }
}

template<class Sleutel, class Data>
Bknoop<Sleutel, Data>& Bknoop<Sleutel, Data>::zoek(Sleutel s, Data d) {
    if (isBlad) {
        return *this;
    }
    blokindex schijfID;

    int i = 0;
    while (i < k && s < sleutels[i]) {
        i++;
    }
    //i--;
    if(i == k && s > sleutels[i-1]){
        schijfID = geefSchijfIDVanRechterKind(i);
    } else {
        schijfID = geefSchijfIDVanLinkerKind(i);
    }

    Bknoop<Sleutel, Data> tmp;
    this->lees(tmp, schijfID);
    return tmp.zoek(s, d);
}
template<class Sleutel, class Data>
bool Bknoop<Sleutel, Data>::contains(Sleutel &s){
    for (int i = 0; i < k; ++i) {
        if(sleutels.at(i)== s){
            return true;
        }
    }
    return false;

}

#endif //GEVORDERDE_ALGORITMEN_BTREE_H
