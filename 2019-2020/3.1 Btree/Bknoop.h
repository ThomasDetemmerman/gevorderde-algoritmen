//
// Created by Thomas on 11/10/2019.
//

#ifndef GEVORDERDE_ALGORITMEN_BTREE_H
#define GEVORDERDE_ALGORITMEN_BTREE_H

#include <iostream>
#include <vector>
#include "schijf.h"

using namespace std;

template<class Sleutel, class Data>
class Bknoop : private Schijf<Bknoop<Sleutel, Data>> {
private:
    int m; //orde / aantal kinderen
    int k = 0; //huidig aantal sleutels. Dit is max m-1
    vector<Sleutel> sleutels;
    vector<Data> data;
    vector<int> kinderen;

    int eigenSchijfpagina;

    int geefSchijfIDVanLinkerKind(int sleutelIndex) const;

    int geefSchijfIDVanRechterKind(int sleutelIndex) const;

    void setSchijfIDVanLinkerKind(int sleutelIndex, int schijfID);

    void setSchijfIDVanRechterKind(int sleutelIndex, int schijfID);
    bool isBlad = true;
    int parent = -1;
public:
    Bknoop();

    Bknoop(int orde) : m(orde) {};

    Bknoop(int orde, bool blad) : m(orde), isBlad(blad){};

    void voegToe(Sleutel s, Data d);

    Bknoop zoek(Sleutel s, Data d);

    void voegToe(Sleutel, Data, int, int);

    void toString();


    void splits();


};


template<class Sleutel, class Data>
void Bknoop<Sleutel, Data>::voegToe(Sleutel s, Data d, int schijfIDLinkerKind, int schijfIDRechterKind) {
    Bknoop<Sleutel,Data> knoop = Bknoop<Sleutel,Data>();//zoek(s,d);
    k++;
    knoop.sleutels.resize(k);   //k telt vanaf nul, maar de size moet dus plus 1 zijn.
    knoop.data.resize(k);
    knoop.kinderen.resize(k + 1);

    int i = k - 1;
    if (isBlad) {
        while (i > 1 && sleutels[i - 1] > s) {
            knoop.sleutels[i] = move(knoop.sleutels[i - 1]);
            knoop.data[i] = move(knoop.data[i - 1]);
            knoop.setSchijfIDVanRechterKind(knoop.geefSchijfIDVanRechterKind(i - 1), i); //no need to move() int.
            i--;
        }
        sleutels[i] = s;
        data[i] = d;
        knoop.setSchijfIDVanLinkerKind(i, schijfIDLinkerKind);
        knoop.setSchijfIDVanRechterKind(i, schijfIDRechterKind);

        if (k == m) {
            knoop.splits();
        }
    }

}

template<class Sleutel, class Data>
void Bknoop<Sleutel, Data>::voegToe(Sleutel s, Data d) {
    voegToe(s, d, -1, -1);
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
void Bknoop<Sleutel, Data>::splits() {
    // geval 1: knoop is de wortel
    if (parent == -1) {
        int midden = (int) k / 2;

        Bknoop Lknoop(m);
        for (int i = 0; i < midden; i++) {
            Lknoop.voegToe(sleutels[i], data[i], geefSchijfIDVanLinkerKind(i), geefSchijfIDVanRechterKind(i));
        }

        int lknoopID = this->schrijf(Lknoop);

        Bknoop Rknoop(m);
        for (int i = midden + 1; i < k; i++) {

            int sleutelTobemoved = sleutels[i];
            Rknoop.voegToe(sleutelTobemoved, data[i], geefSchijfIDVanLinkerKind(i),
                           geefSchijfIDVanRechterKind(i));
        }

        int rknoopID = this->schrijf(Lknoop);

        Bknoop nieuweWortel(m);
        nieuweWortel.voegToe(sleutels[midden], data[midden], lknoopID, rknoopID);
        nieuweWortel.isBlad = false;
        *this = move(nieuweWortel);


    }
}

template<class Sleutel, class Data>
Bknoop<Sleutel, Data> Bknoop<Sleutel, Data>::zoek(Sleutel s, Data d) {
    if (isBlad) {
        return *this;
    }
    int schijfID;

    int i = 0;
    while (i < k && s < sleutels[i]) {
        i++;
    }
    if(i == k && s > sleutels[i]){
        schijfID = geefSchijfIDVanRechterKind(i);
    } else {
        schijfID = geefSchijfIDVanLinkerKind(i);
    }

    Bknoop<Sleutel, Data> tmp;
    this->lees(tmp, schijfID);
    return tmp.zoek(s, d);
}


#endif //GEVORDERDE_ALGORITMEN_BTREE_H
