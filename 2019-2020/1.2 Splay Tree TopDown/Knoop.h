//
// Created by Thomas on 01/11/2019.
//

#ifndef INC_1_2_SPLAY_TREE_TOPDOWN_KNOOP_H
#define INC_1_2_SPLAY_TREE_TOPDOWN_KNOOP_H
template <class Sleutel, class Data>
class SplayTree;


template <class Sleutel, class Data>
class Knoop {

public:
    Data data;
    Sleutel sleutel;
    SplayTree<Sleutel, Data> links, rechts;

    Knoop(const Sleutel & s,const Data & d): data(d), sleutel(s){};
    Knoop & operator=(Knoop && k){
            *this = move(k);
    };

    Knoop(const Data & d, const Sleutel & s) = delete; //moet in feite niet want aangezien we hierboven constructoren definieeren zijn deze ook verwijderd.




    SplayTree<Sleutel, Data>& geefKind(bool linkseKind){
        if(linkseKind){
            return links;
        } else {
            return rechts;
        }
    }

};


#endif //INC_1_2_SPLAY_TREE_TOPDOWN_KNOOP_H
