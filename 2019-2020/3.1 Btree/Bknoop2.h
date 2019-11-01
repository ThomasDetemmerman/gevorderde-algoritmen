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
class Bknoop : private Schijf<Bknoop<Sleutel, Data>> {
private:
    int m; //orde / aantal kinderen
    int k = 0; //huidig aantal sleutels. Dit is max m-1
    vector<Sleutel> sleutels;
    vector<Data> data;
    vector<blokindex> kinderen;

    int eigenSchijfpagina;

    int geefSchijfIDVanLinkerKind(int sleutelIndex) const;

    int geefSchijfIDVanRechterKind(int sleutelIndex) const;

    void setSchijfIDVanLinkerKind(int sleutelIndex, int schijfID);

    void setSchijfIDVanRechterKind(int sleutelIndex, int schijfID);
    bool isBlad = true;

public:
    Bknoop(){};

    Bknoop(int orde) : m(orde) {};

    Bknoop(int orde, bool blad) : m(orde), isBlad(blad){};



    void toString();


    void splits(stack<int> parents);


    void splits(stack<blokindex> parents);

    void voegToe(Sleutel s, Data d);
};


template<class Sleutel, class Data>
void Bknoop<Sleutel, Data>::voegToe(Sleutel s, Data d, int schijfIDLinkerKind, int schijfIDRechterKind) {

    k++;
    std::cout << "aantal sleutels in deze knoop: " << k << endl;
    sleutels.resize(k);   //k telt vanaf nul, maar de size moet dus plus 1 zijn.
    data.resize(k);
    kinderen.resize(k + 1);

    int i = k;
    while (i > 1 &&  sleutels[i - 1] > s) {
        sleutels[i] = move(sleutels[i - 1]);
        data[i] = move(data[i - 1]);
        setSchijfIDVanRechterKind(geefSchijfIDVanRechterKind(i - 1), i); //no need to move() int.
        i--;
    }
    sleutels[i] = s;
    data[i] = d;
    setSchijfIDVanLinkerKind(i, schijfIDLinkerKind);
    setSchijfIDVanRechterKind(i, schijfIDRechterKind);
    //knoop.setSchijfIDVanLinkerKind(i, schijfIDLinkerKind);



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

#endif //GEVORDERDE_ALGORITMEN_BTREE_H
