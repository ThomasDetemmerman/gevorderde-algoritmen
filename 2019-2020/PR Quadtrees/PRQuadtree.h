//
// Created by Thomas on 17/10/2019.
//

#ifndef INC_2019_2020_PRQUADTREE_H
#define INC_2019_2020_PRQUADTREE_H

#include <memory>

#include "windrichting.h"
#include "knoop.h"

using namespace std;

template<class Data>
class PRQuadtree: public unique_ptr<Knoop<Data>> {
private:
    int dimentielengte;
public:
    PRQuadtree(int size):PRQuadtree(size, 0, 0){};
    PRQuadtree(int size, int x, int y){
        dimentielengte = size;
        Knoop<Data> newKnoop(x, y);
        this->unique_ptr<Knoop<Data>>(move(newKnoop));
    };
    void zoek(Knoop<Data> zoekknoop, PRQuadtree<Data> &locatie);
    void voegToe(int x, int y, const Data d);
};

template<class Data>
void PRQuadtree<Data>::zoek(Knoop<Data> zoekknoop, PRQuadtree<Data> &locatie){
    //indien de data is geïnitialeseerd zitten we in een blad
    if((*this)->windrichting[0] == NULL && (*this)->windrichting[1] == NULL && (*this)->windrichting[2] == NULL && (*this)->windrichting[3] == NULL ){
        return *this;
    }
    PRQuadtree<Data> dest = *this;
    Windrichting windrichting = dest->getDirection(zoekknoop);
    dest = dest->windrichting[windrichting];
    zoek(dest, locatie);
};

template<class Data>
void PRQuadtree<Data>::voegToe(int x, int y, const Data d){
    Knoop<Data> zoekknoop(x,y);
    Knoop<Data> locatie;

    zoek(zoekknoop, locatie);

}



#endif //INC_2019_2020_PRQUADTREE_H
