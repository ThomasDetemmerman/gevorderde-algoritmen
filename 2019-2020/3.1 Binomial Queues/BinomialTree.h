//
// Created by Thomas on 16/10/2019.
//

#ifndef INC_4_1_BINOMIAL_QUEUES_BINOMIALTREE_H
#define INC_4_1_BINOMIAL_QUEUES_BINOMIALTREE_H

#include <memory>
#include <sstream>
#include <fstream>
#include "zoekknoop.h"

using namespace std;

template<class Sleutel, class Data>
class zoekKnoop;

template<class Sleutel, class Data>
class BinomialTree : public unique_ptr<zoekKnoop<Sleutel, Data>> {
public:
    BinomialTree() = default;
    BinomialTree(BinomialTree &&) = default;
    BinomialTree& operator=(BinomialTree &&) = default;

    BinomialTree( const BinomialTree & bt) = delete;
    BinomialTree& operator=(const BinomialTree &) = delete;

    //BinomialTree(unique_ptr<BinomialTree<Sleutel, Data>> &&k) : unique_ptr<BinomialTree<Sleutel, Data>>(move(k)){};
    BinomialTree(Sleutel prioriteit, Data data) : unique_ptr<zoekKnoop<Sleutel, Data>>(move(make_unique<zoekKnoop<Sleutel, Data>>(zoekKnoop<Sleutel, Data>(prioriteit, data)))) {};

    void voegtoe(BinomialTree<Sleutel, Data> &bt);

    //BinomialTree(unique_ptr<zoekKnoop<Sleutel, Data>> &&k) : unique_ptr<zoekKnoop<Sleutel, Data>>(move(k)){};
    void teken(const char *bestandsnaam);

private:
    string tekenrec(ostream &uit, int &knoopteller);
};

template<class Sleutel, class Data>
void BinomialTree<Sleutel, Data>::voegtoe( BinomialTree<Sleutel, Data> &bt) {
    assert(bt);
    assert(*this);

    BinomialTree<Sleutel, Data> grootstePrior = move(*this);
    BinomialTree<Sleutel, Data> laagstePrior = std::move(bt);

    if (grootstePrior->prioriteit > laagstePrior->prioriteit) {
        swap(grootstePrior, laagstePrior);
    }


    (*laagstePrior).broer = move((*grootstePrior).kind);
    (*grootstePrior).kind = move(laagstePrior);

    *this = move(grootstePrior);
}


/* teken functie */


template<class Sleutel, class Data>
void BinomialTree<Sleutel, Data>::teken(const char *bestandsnaam) {
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\n";
    this->tekenrec(uit, knoopteller);
    uit << "}";
}

template<class Sleutel, class Data>
string BinomialTree<Sleutel, Data>::tekenrec(ostream &uit, int &knoopteller) {
    ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';
    if (!*this) {
        uit << wortelstring.str() << " [shape=point];\n";
    } else {
        uit << wortelstring.str() << "[label=\"" << (*this)->prioriteit << ":" << (*this)->data << "\"]";
        uit << ";\n";
        string linkskind = (*this)->kind.tekenrec(uit, knoopteller);
        string rechtskind = (*this)->broer.tekenrec(uit, knoopteller);
        uit << wortelstring.str() << " -> " << linkskind << ";\n";
        uit << wortelstring.str() << " -> " << rechtskind << ";\n";
    };
    return wortelstring.str();
}

#endif //INC_4_1_BINOMIAL_QUEUES_BINOMIALTREE_H
