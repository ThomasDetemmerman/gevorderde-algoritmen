//
// Created by Thomas on 11/10/2019.
//

#ifndef INC_3_1_BTREE_BTREE_H
#define INC_3_1_BTREE_BTREE_H

#include "Bknoop2.h"
#include "schijf.h"
#include <stack>
template <class Sleutel, class Data>
class Btree  : private Schijf<Bknoop<Sleutel, Data>>{
private:
    int m;
    const blokindex root = 0;
    void zoek(Sleutel &s,Bknoop<Sleutel,Data> &doel,  stack<blokindex>   &parents);
    void splits(stack<blokindex> parents);
public:
    Btree(int orde);
    void voegToe(Sleutel, Data);

    void toString();
};

template <class Sleutel, class Data>
Btree<Sleutel,Data>::Btree(int orde) :m(orde){}

template<class Sleutel, class Data>
void Btree<Sleutel, Data>::voegToe(Sleutel s, Data d) {
    Bknoop<Sleutel,Data> doel;
    stack<int> parents;
    Btree<Sleutel,Data>::zoek(&s, &doel, &parents);
    assert(!doel.contains(s)); // sleutel bestaat al.
    doel.voegToe(s,d);
    if(doel.k = m){
        splits(parents);
    }
};

/***
 *
 * @tparam Sleutel
 * @tparam Data
 * @param s
 * @param doel contains the BKnoop where the value should be added.
 * @param parents
 */

template <class Sleutel, class Data>
void  Btree<Sleutel,Data>::zoek(Sleutel &s,Bknoop<Sleutel,Data> &doel,  stack<blokindex>  &parents){
    Bknoop<Sleutel, Data> *current;
    lees(&(*current), root);
    //parents.push(root);
    while(!current->contains(s) && !current->isBlad){

        int i = 0;
        while(s > current->sleutels[i] && i < current->k){
            i++;
        }
        if(s > current->sleutels[i] && i < current->k){
            i++;
        }
        parents.push(current->kinderen[i]);
       (*this).lees(&(*current), current->kinderen[i]);
    }
    return current;
}

/*
 *  Splits stops if k == m
 */
template<class Sleutel, class Data>
void Btree<Sleutel, Data>::splits(stack<blokindex> parents) {

    // geval 1: knoop is de wortel
    if (parents.empty()) {
        Bknoop<Sleutel, Data> parent;
        lees(&(parent), root);
        if(parent.k == m){
            int midden = (int) parent.k / 2;

            Bknoop<Sleutel, Data> Lknoop(m);
            Lknoop.isBlad = true;
            for (int i = 0; i < midden; i++) {
                Lknoop.voegToe(parent.sleutels[i], parent.data[i], parent.geefSchijfIDVanLinkerKind(i), parent.geefSchijfIDVanRechterKind(i));
            }

            blokindex lknoopID = this->schrijf(Lknoop);

            Bknoop<Sleutel, Data> Rknoop(m);
            Rknoop.isBlad = true;
            for (int i = midden + 1; i < parent.k; i++) {

                int sleutelTobemoved = parent.sleutels[i];
                Rknoop.voegToe(sleutelTobemoved, parent.data[i], parent.geefSchijfIDVanLinkerKind(i),
                               parent.geefSchijfIDVanRechterKind(i));
            }

            blokindex rknoopID = this->schrijf(Lknoop);

            Bknoop<Sleutel, Data> nieuweWortel(m);
            nieuweWortel.voegToe(parent.sleutels[midden], parent.data[midden], lknoopID, rknoopID);
            nieuweWortel.isBlad = false;

            this->herschrijf(nieuweWortel, root);
        }



    } else {
        blokindex parentBlokIndex = parents.top();
        parents.pop();
        Bknoop<Sleutel, Data> parent;
        lees(&(parent), parentBlokIndex);
        if(parent.k == m){

            splits(parents);
        }


        //todo



    }


}

template<class Sleutel, class Data>
void Btree<Sleutel, Data>::toString() {
    Bknoop<Sleutel, Data> parent;
    lees(&(parent), root);
    parent.toString();
}


#endif //INC_3_1_BTREE_BTREE_H
