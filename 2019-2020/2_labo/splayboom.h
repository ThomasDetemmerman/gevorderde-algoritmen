//
// Created by Thomas on 03/10/2019.
//

#ifndef INC_2_LABO_SPLAYBOOM_H
#define INC_2_LABO_SPLAYBOOM_H

#include "zoekboom17.h"

template <class Sleutel, class Data>
class Splayboom : public Zoekboom<Sleutel, Data> {

    public:
        void zoek(const Sleutel &sleutel, zoekKnoop<Sleutel, Data> *&ouder, Zoekboom<Sleutel, Data> *&plaats);

    private:
        void splay(zoekKnoop<Sleutel,Data> *& ouder, Zoekboom<Sleutel,Data> *& plaats );


};

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::zoek(const Sleutel &sleutel, zoekKnoop<Sleutel, Data> *&ouder, Zoekboom<Sleutel, Data> *&plaats)
{
    Zoekboom<Sleutel,Data>::zoek(sleutel, ouder, plaats);
    if(*plaats != NULL){
        //indien de knoop gevonden is
        splay(ouder, plaats);
    } else {
        //todo: breng zijn ouder naar de root
    }

}


template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::splay( zoekKnoop<Sleutel,Data> *& ouder, Zoekboom<Sleutel,Data> *& plaats ){

    //als jij de wortel bent moet er niets gebeuren
    if(*plaats == (*this) ){
        return;
    }

    //als de ouder wortel is, dan enkel een zig
    //*ouder == **this werkt niet want we kunnen geen knopen met elkaar vergelijken. Wel zijn adressen.
    if(&*ouder == &**this){
       // indien de knoop linkerkind is zullen we naar rechts roteren.
       //zig
        (*this).roteer((*plaats)->isLeftChild());
    }

    else {
        Zoekboom<Sleutel,Data>* grandparent = Zoekboom<Sleutel,Data>::geefBoomBovenKnoop(*(ouder->ouder));

        // c is child, p is parent, g is grandparent
        // relation of c to p
        bool cIsLeftChildOfP = (*plaats)->isLeftChild();
        bool pIsLeftChildOfG = ouder->isLeftChild();

        if (cIsLeftChildOfP == pIsLeftChildOfG) {
            //zigzig
            grandparent->roteer(cIsLeftChildOfP);
            grandparent->roteer(cIsLeftChildOfP);
        } else {
            //zigzag
            Zoekboom<Sleutel,Data>* pointerToP = Zoekboom<Sleutel,Data>::geefBoomBovenKnoop(*ouder);
            pointerToP->roteer(cIsLeftChildOfP);
            grandparent->roteer(pIsLeftChildOfG); //pointer to G

        }

        splay((*grandparent)->ouder, grandparent);
    }






}

#endif //INC_2_LABO_SPLAYBOOM_H
