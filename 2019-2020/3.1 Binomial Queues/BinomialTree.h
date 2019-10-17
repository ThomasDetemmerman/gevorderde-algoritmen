//
// Created by Thomas on 16/10/2019.
//

#ifndef INC_4_1_BINOMIAL_QUEUES_BINOMIALTREE_H
#define INC_4_1_BINOMIAL_QUEUES_BINOMIALTREE_H

#include <memory>
#include "zoekknoop.h"

using namespace std;

template<class Sleutel, class Data>
class zoekKnoop;

template<class Sleutel, class Data>
class BinomialTree : public unique_ptr<zoekKnoop<Sleutel, Data>> {
public:
    BinomialTree() {};
    BinomialTree(Sleutel prioriteit, Data data): unique_ptr<zoekKnoop<Sleutel, Data>>(move(make_unique<zoekKnoop<Sleutel, Data>>(zoekKnoop<Sleutel, Data>(prioriteit, data)))){};
    void voegtoe(const BinomialTree<Sleutel,Data> bt);
    //BinomialTree(unique_ptr<zoekKnoop<Sleutel, Data>> &&k) : unique_ptr<zoekKnoop<Sleutel, Data>>(move(k)){};
};



template<class Sleutel, class Data>
void BinomialTree< Sleutel,  Data>::voegtoe(const BinomialTree<Sleutel,Data> bt){

    if(bt->prioriteit > (*this)->prioriteit){
        bt->broer = move((*this)->kind);
        (*this)->kind = move(bt);

    }
}


#endif //INC_4_1_BINOMIAL_QUEUES_BINOMIALTREE_H
