//
// Created by Thomas on 30/11/2019.
//

#ifndef INC_10_PATRICIATRIES_PATRICIATRIE_H
#define INC_10_PATRICIATRIES_PATRICIATRIE_H

#include <memory>
#include <string>
#include <fstream>
#include "Blad.h"
#include "NietBlad.h"

using namespace std;

class Knoop;

typedef unique_ptr<Knoop> Knoopptr;

class Patriciatrie : public Knoopptr{
public:
    using Knoopptr::unique_ptr;
    Patriciatrie(){};
    int zoek(string sleutel , string &data);
    void voegToe(string sleutel, string data);
    void teken(const char *bestandsnaam);

    int geefVerschilIndex(string &basicString, string basicString1);
};


int Patriciatrie::zoek(string sleutel, string &data) {
    if(!(*this)){
        return -1;
    };
    if((*this)->isBlad()){
        data = static_cast<Blad*>(this->get())->data;
        return geefVerschilIndex(sleutel, static_cast<Blad*>(this->get())->sleutel);
    } else {
        Knoopptr *current = this;
        int testIndex =  static_cast<NietBlad*>(current->get())->testIndex;

        while(static_cast<NietBlad*>(current->get())->geeftKind(sleutel[testIndex])){
            current = static_cast<NietBlad*>(current->get())->geefKind(sleutel[testIndex]);
            testIndex = static_cast<NietBlad*>(current->get())->testIndex;
        }

        if((*current)->isBlad()){
            data = static_cast<Blad*>(current->get())->data;
            return geefVerschilIndex(sleutel, static_cast<Blad*>(this->get())->sleutel);
        } else {
            // else knoop niet aanwezig
            return -1;
        }
    }


}

void Patriciatrie::voegToe(string sleutel, string data) {
    if (*this == nullptr) {

        auto ptrToBlad = make_unique<Blad>(Blad(sleutel, data));
        *this = move(ptrToBlad);
    } else {


    if((*this)->isBlad()){
        string *ptrsleutel = &(static_cast<Blad *>(this->get())->sleutel);
        int verschilIndex = geefVerschilIndex(sleutel, static_cast<Blad*>((this)->get())->sleutel); NietBlad nb(verschilIndex);
        nb.voegToe(*this, (*ptrsleutel)[verschilIndex]);
        Knoopptr kptr = make_unique<Blad>(Blad(sleutel, data));
        nb.voegToe(kptr, sleutel[verschilIndex]);
        *this = move(make_unique<NietBlad>(move(nb)));
    }else {

        //afdalen naar plaats

        Knoopptr *current = this;
        bool stop = false;
        char testTeken;
        while ( !(*current)->isBlad() && !stop) {
            int testindex = static_cast<NietBlad *>(current->get())->testIndex;
            testTeken = sleutel[testindex];
            // indien deze een kind heeft dat geen blad is
            if(static_cast<NietBlad *>(current->get())->geeftKind(testTeken) && !(*(static_cast<NietBlad *>(current->get())->geefKind(testTeken)))->isBlad()){
                current = (static_cast<NietBlad *>(current->get())->geefKind(testTeken));
            } else {
                stop = true;
            }
        }
        NietBlad * laatsteNietblad = static_cast<NietBlad *>(current->get());
        Blad * willekeurigBlad = laatsteNietblad->geefWillekeurigKind();
        string willekeurigeSleutel = willekeurigBlad->sleutel;
        int verschilIndex = geefVerschilIndex(sleutel, willekeurigeSleutel);

        current = this;
        Knoopptr * parent = nullptr;
        while((*current) && verschilIndex >= static_cast<NietBlad *>(current->get())->testIndex && !(*current)->isBlad()) {
            int testIndex = static_cast<NietBlad *>(current->get())->testIndex;
            parent = current;
            current = static_cast<NietBlad *>(current->get())->geefKind(sleutel[testIndex]);
        }
        if((*current)== nullptr){
            Knoopptr knptr = make_unique<Blad>(sleutel, data);
            static_cast<NietBlad *>(parent->get())->voegToe(knptr ,sleutel[verschilIndex]);
        } else {
            //todo: dit testen:
            NietBlad nb(verschilIndex);
            Knoopptr kptr = make_unique<Blad>(Blad(sleutel, data));
            nb.voegToe(kptr, sleutel[verschilIndex]);
            nb.voegToe(*current, static_cast<Blad*>(current->get())->sleutel[verschilIndex]);
            Knoopptr knptr = make_unique<NietBlad>(move(nb));
            static_cast<NietBlad *>(parent->get())->voegNietBladToe(knptr,verschilIndex);
        }

        /*else         if((*current)->isBlad()) {
            std::cout << "dit blad moet gesplitst worden";
        }*/

    }
/*
            //toevoegen
            int i = 0;
            string *ptrsleutel = &(static_cast<Blad *>(this->get())->sleutel);
            do {
                i++;
            } while ((*ptrsleutel)[i] == sleutel[i] && i < sleutel.size());


        }

        /*else {




        }*/
    }
}
void Patriciatrie::teken(const char *bestandsnaam) {
    std::ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\n";
    (*this)->tekenrec(uit, knoopteller, 0);
    uit << "}";
}

int Patriciatrie::geefVerschilIndex(string &sleutelA, string sleutelB) {
    int i=0;
    while(sleutelA[i] == sleutelB[i]){
        i++;
    }
    return i;
}

#endif //INC_10_PATRICIATRIES_PATRICIATRIE_H
