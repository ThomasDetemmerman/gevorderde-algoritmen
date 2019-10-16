//
// Created by Thomas on 16/10/2019.
//

#ifndef INC_3_1_BTREE_BINQUEUE_H
#define INC_3_1_BTREE_BINQUEUE_H
#include <memory>
#include <iostream>
#include <vector>
#include "BinomialTree.h"

template <class Sleutel, class Data>
class BinQueue: private vector<BinomialTree<Sleutel, Data>>{

public:
    BinQueue(int size){
        this->resize(size);
    };
    void voegToe(Sleutel, Data);
private:
    void voegToe(BinomialTree<Sleutel,Data> &bq);
};

template <class Sleutel, class Data>
void BinQueue<Sleutel,Data>::voegToe(Sleutel prioriteit, Data data) {
    BinomialTree<Sleutel, Data> bt(prioriteit, data);
    voegtoe(0, move(bt));

}

template <class Sleutel, class Data>
void BinQueue<Sleutel,Data>::voegToe(int diepte, BinomialTree<Sleutel,Data> &bt) {




    if(this->at(diepte) == NULL){
        this->at(diepte) = move(bt);
    } else {
        BinomialTree<Sleutel,Data> tmp = move(this->at(diepte).voegToe(bt));
        voegToe(++diepte, move(tmp));

    }
}

#endif //INC_3_1_BTREE_BINQUEUE_H
