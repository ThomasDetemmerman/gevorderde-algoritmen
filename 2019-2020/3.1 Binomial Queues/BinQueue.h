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
    void teken();
    Data min();
    //todo: waarom kan dit geen private functie zijn:
    void voegToe(int, BinomialTree<Sleutel,Data> &&bq);
};

template <class Sleutel, class Data>
void BinQueue<Sleutel,Data>::voegToe(int diepte, BinomialTree<Sleutel,Data> &&bt) {
assert(bt);
    if(this->at(diepte) == NULL){
        this->at(diepte) = move(bt);

    } else {

        (this->at(diepte)).voegtoe((bt));

       voegToe(diepte+1, move(this->at(diepte)));


    }
}

template <class Sleutel, class Data>
void BinQueue<Sleutel,Data>::voegToe(Sleutel prioriteit, Data data) {
    BinomialTree<Sleutel, Data> bt(prioriteit, data);
    this->voegToe(0,move(bt));
}

template<class Sleutel, class Data>
void BinQueue<Sleutel, Data>::teken() {
    for(int i = 0; i < this->size(); i++){
        this->at(i).teken(("output_" + to_string(i) + ".dot").c_str());
    }
}

template<class Sleutel, class Data>
Data BinQueue<Sleutel, Data>::min() {

    BinomialTree<Sleutel, Data> *min = nullptr;
    int diepte;
    for(int i = 1; i < this->size(); i++){
        if(this->at(i) != NULL && min == nullptr){
            min = &(this->at(i));
            diepte = i;
        }
        if(this->at(i) != NULL && *min != NULL && this->at(i)->prioriteit < (*min)->prioriteit){
            min = &(this->at(i));
            diepte = i;
        }
    }
    if(*min == NULL){
        return -1;
    }

    Data mindata = (*min)->data;

    BinomialTree<Sleutel, Data> broer = move((*min)->kind);
    while(broer){
        BinomialTree<Sleutel, Data> tmp = move(broer->broer);

        voegToe(diepte--, move(broer));
        broer = move(tmp);
    }


    return mindata;
}


#endif //INC_3_1_BTREE_BINQUEUE_H
