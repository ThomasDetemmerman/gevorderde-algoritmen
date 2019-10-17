//
// Created by Thomas on 17/10/2019.
//

#ifndef PR_QUADTREES_KNOOP_H
#define PR_QUADTREES_KNOOP_H

#include "windrichting.h"

template <class Data>
class PRQuadtree;


template <class Data>
class Knoop {
friend class PRQuadtree<Data>;
private:
    int x;
    int y;
    PRQuadtree windrichting[4];
    Data d;
public:
    Knoop(){};
    Knoop(int x_, int y_): x(x_), y(y_);
    Knoop(Data data): d(data);
    Windrichting getDirection(const Knoop<Data> &doel);
};

template<class Data>
Windrichting Knoop<Data>::getDirection(const Knoop<Data> &doel){
    if(this->y < doel->y){
        //naar boven
        if(this->x < doel->x){
            // naar rechts
            return Windrichting.NO;
        } else{
            return Windrichting.NW;
        }
    }else{
        //naar onder
        if(this->x < doel->x){
            // naar rechts
            return Windrichting.ZO;
        } else{
            return Windrichting.ZW;
        }
    }
}


#endif //PR_QUADTREES_KNOOP_H
