//
// Created by Thomas on 21/10/2019.
//

#ifndef LABO_4_PR_QUADTREES_PRQUADTREEV2_H
#define LABO_4_PR_QUADTREES_PRQUADTREEV2_H


#ifndef __PRQUADTREE_H__
#define __PRQUADTREE_H__

#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <stack>

#include <sstream>

using std::ostream;
using std::ofstream;
using std::ostringstream;
using std::unique_ptr;
using std::cerr;
using std::pair;
using std::string;
using std::unique_ptr;

/*****************************************************************************
    Bevat de headers en code voor een PRQuadtree
    en voor de abstracte klasse PRKnoop met deelklassen Blad en Nietblad

*****************************************************************************/


class PRKnoop;

typedef unique_ptr<PRKnoop> Knoopptr;

class PRQuadtree;


class PRQuadtree : public Knoopptr {
public:
    using Knoopptr::unique_ptr;

    PRQuadtree(int a) : maxcoordinaat{a} {};

    PRQuadtree(Knoopptr &&a) : Knoopptr(move(a)) {};

    PRQuadtree &operator=(Knoopptr &&a) {
        Knoopptr::operator=(move(a));
        return *this;
    };

    void preEnPostOrder(std::function<void(PRKnoop *)> &bezoekPre, std::function<void(PRKnoop *)> &bezoekPost) const;

    //te implementeren
    void voegToe(int x, int y);

    int geefDiepte();

    int maxcoordinaat;

    void teken(const char *bestandsnaam);

    pair<int,int>  zoek(int x, int y, Knoopptr *&);

    pair<int, int> findMiddle(int x_prev, int y_prev, int x_target, int y_target);
};

//Opmerking: om de functies specifiek aan een deelklasse te kunnen gebruiken moet je soms een
//static_cast doen, zoals in
//  PRKnoop* knoopptr=...;
//  if (knoopptr!=nullptr && knoopptr->isBlad() && static_cast<PRBlad*>(knoopptr)->x == 5)
class PRKnoop {
public:
    virtual bool isBlad() = 0;

    virtual int geefDiepte() = 0;

    virtual string tekenrec(ostream &uit, int &knoopteller, int diepte) = 0;
};

class PRBlad : public PRKnoop {
public:
    PRBlad(int x, int y) : x{x}, y{y} {};

    virtual bool isBlad() { return true; }

    int x, y;//co"ordinaten punt
    virtual int geefDiepte();

    virtual string tekenrec(ostream &uit, int &knoopteller, int diepte) {
        ostringstream wortelstring;
        wortelstring << '"' << ++knoopteller << '"';

        uit << wortelstring.str() << "[label=\"" << x << "," << y << "\"]";

        return wortelstring.str();
    }
};

class PRNietblad : public PRKnoop {
public:
    virtual bool isBlad() { return false; }

    int geefAantalKinderen() {
        int aantal = 0;
        for (int i = 0; i < 4; i++) {
            if (kind[i])
                ++aantal;
        }
        return aantal;
    };

    //xc, yc: co"ordinaten van het centrum van het gebied
    Knoopptr *geefKind(int x, int y, int xc, int yc) {
        int xindex = (x < xc ? WEST : OOST);
        int yindex = (y < yc ? NOORD : ZUID);
        return &kind[xindex + yindex];
    };
    static const int OOST, WEST, NOORD, ZUID;
    Knoopptr kind[4];//indexeren met windrichting (bv. kind[NOORD+WEST] bevat punten
    //met x en y kleiner dan grenswaarde)
    //leeg gebied: nulpointer
    int geefDiepte();

    virtual string tekenrec(ostream &uit, int &knoopteller, int diepte) {
        ostringstream wortelstring;
        wortelstring << '"' << ++knoopteller << '"';

        // HARDCODED 100 HIER: verwijderen achteraf
        uit << wortelstring.str() << "[label=\"" << "." << "\"]";
        uit << ";\n";

        for (int i = 0; i < 4; i++) {
            if (this->kind[i]) {
                string res = this->kind[i]->tekenrec(uit, knoopteller, diepte + 1);
                uit << wortelstring.str() << " -> " << res << ";\n";
            } else {
                ostringstream wortelstring2;
                wortelstring2 << '"' << ++knoopteller << '"';

                uit << wortelstring2.str() << " [shape=point];\n";
                uit << wortelstring.str() << " -> " << wortelstring2.str() << ";\n";
            }
        }

        return wortelstring.str();
    }

    void voegToe(int x, int y);
};

void PRQuadtree::preEnPostOrder(std::function<void(PRKnoop *)> &bezoekPre,
                                std::function<void(PRKnoop *)> &bezoekPost) const {
    enum staat {
        pre, post
    };
    std::stack<std::pair<PRKnoop *, staat>> DEstack;//bevat alleen niet-nulpointers
    if (*this)
        DEstack.emplace(this->get(), pre);
    while (!DEstack.empty()) {
        auto[nuknoop, nustaat]=DEstack.top();
        if (nustaat == pre) {
            bezoekPre(nuknoop);
            DEstack.top().second = post;
            if (!nuknoop->isBlad()) {
                for (int i = 0; i < 4; i++) {
                    Knoopptr *kind = &(static_cast<PRNietblad *>(nuknoop)->kind[i]);
                    if (*kind)
                        DEstack.emplace(kind->get(), pre);
                };
            };
        } else {
            bezoekPost(nuknoop);
            DEstack.pop();
        };
    };

};

/********* teken **************/

void PRQuadtree::teken(const char *bestandsnaam) {
    std::ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\n";
    (*this)->tekenrec(uit, knoopteller, 0);
    uit << "}";
}

/*********************************
 *
 * start eigen implementaties
 *
 ********************************/

/******* geef diepte functies *******/
int PRQuadtree::geefDiepte() {
    if (!(*this)) {
        return 0;
    }
    return (*this)->geefDiepte();
};

int PRNietblad::geefDiepte() {

    int maxDiepte = 0;

    if (!(*this).isBlad()) {
        for (int i = 0; i < 4; i++) {
            if((*this).kind[i])
            {
                int dieptekind = (*this).kind[i]->geefDiepte();
                maxDiepte = std::max(maxDiepte, dieptekind);
            }

        }
    }
    return maxDiepte+1;
};

int PRBlad::geefDiepte() {
    return 1;
};

/**** andere functies: voegtoe en zoek ****/

void PRQuadtree::voegToe(int x, int y) {
    if (!(*this)) {
        // of in 1 lijn
        // *this = PRQuadtree(make_unique<PRBlad>(x, y));
        Knoopptr nieuw = std::make_unique<PRBlad>(x, y);
        *this = move(nieuw);
    } else {

        Knoopptr *doel;
        pair<int,int> midden = zoek(x, y, doel);

        // blad converten naar inwendige knoop met twee bladeren
        if ((*doel) != nullptr && (*doel)->isBlad()) {
            // we slaan eerst ons huidig blad op voor later
            PRBlad* huidigBlad = static_cast<PRBlad* >(doel->get());

            // we maken een nieuwe inwendige knoop aan
            unique_ptr<PRNietblad> nieuwinwendigeKnoop = std::make_unique<PRNietblad>();
            // en plaatsen deze op zijn doel
            *doel = move(nieuwinwendigeKnoop);
            //in doel zit nu een PRNietblad


            //zolang beide locaties hetzelfde zijn blijven we opsplitsen
            pair<int,int> middenA = findMiddle(midden.first, midden.second, x, y);
            pair<int,int> middenB = findMiddle(midden.first, midden.second, huidigBlad->x, huidigBlad->y);

            //alternatief kunnen we ook geefKind oproepen voor beide bladeren en vervolgens hun adressen vergelijken
            // while( knoopptr_kind1 == knoopptr_kind2)
            while (middenA.first == middenB.first && middenA.second == middenB.second) {
                unique_ptr<PRNietblad> nieuwinwendigeKnoop = std::make_unique<PRNietblad>();
                Knoopptr *locatieA = static_cast<PRNietblad *>(doel->get())->geefKind(x, y, middenA.first, middenB.second);
                *locatieA = move(nieuwinwendigeKnoop);
                doel = locatieA;

                middenA = findMiddle(middenA.first, middenA.second, x, y);
                middenB = findMiddle(middenB.first, middenB.second, huidigBlad->x, huidigBlad->y);

               //std::cout << "middenA: " << middenA.first << "," << middenA.second << "\t" << "middenB: " << middenB.first << "," << middenB.second << std::endl;

            }

            Knoopptr *locatieA = static_cast<PRNietblad *>(doel->get())->geefKind(huidigBlad->x, huidigBlad->y, middenA.first, middenA.second);
            Knoopptr *locatieB = static_cast<PRNietblad *>(doel->get())->geefKind(x, y, middenB.first, middenB.second);

            //voeg oud blad toe als kind
            *locatieA = std::move(std::make_unique<PRBlad>(*huidigBlad));

            //voeg nieuw blad toe
            Knoopptr *kindNieuw = nieuwinwendigeKnoop->geefKind(x, y, midden.first, midden.second);
            *locatieB = std::move(std::make_unique<PRBlad>(x, y));

            // om naar een bovenklasse te casten moeten we een tussenstap nemen.
            // de compiler zei dat het ambigu was, maw hij wist niet hoe het moest. Wij moeten dus expliciet aangeven dat we via een prknoop willen gaan.
           // unique_ptr<PRKnoop> tussenstap = move(nieuwinwendigeKnoop);
            //*doel = std::move(tussenstap);
            //voeg nieuwe knoop toe
            //auto oudBlad = std::make_unique<PRBlad>(x,y);
        } else {
            //als er nog geen blad bevindt kunnen we deze gewoon toevoegen
            Knoopptr nieuw = std::make_unique<PRBlad>(x, y);
            *doel = move(nieuw);

        }

    }
}

pair<int,int> PRQuadtree::zoek(int x, int y, Knoopptr *&output) {
    PRNietblad *current = static_cast<PRNietblad *>(this->get());
    //todo: denk dat onderstaand midden enkel klopt indiet zoeken vanuit de root wordt aangeroepen.
    pair<int,int> midden((*this).maxcoordinaat / 2, (*this).maxcoordinaat / 2);
    if (current->isBlad()) {
        output = move(this);
        return midden;
    }

    Knoopptr *knptr = current->geefKind(x, y, midden.first, midden.second);

    // het algoritme stopt als het naar een blad wijst of bij een inwendige knoop die het resultaat bevat.
    // eerst kijken of het een blad is, in andere geval casten
    while (*knptr != nullptr && !(*knptr)->isBlad() && !(static_cast<PRBlad *>(knptr->get())->x == x && static_cast<PRBlad *>(knptr->get())->y == y)) {
        current = static_cast<PRNietblad *>(knptr->get());
        pair<int,int> midden = findMiddle(midden.first, midden.second, x, y);
        knptr = current->geefKind(x, y, midden.first, midden.second);
    }
    output = move(knptr);
    return midden;

}

/***** hulp functies ******/

pair<int, int> PRQuadtree::findMiddle(int x_prev, int y_prev, int x_target, int y_target) {
    pair<int, int> coordinate;
    if (x_target < x_prev) {
        //west
        coordinate.first = x_prev -= (x_prev / 2);
    } else {
        //oost
        coordinate.first = x_prev += (x_prev / 2);
    }
    if (y_target < y_prev) {
        //noord
        coordinate.second = y_prev -= (y_prev / 2);
    } else {
        coordinate.second = y_prev += (y_prev / 2);
    }

    return coordinate;

}

const int PRNietblad::OOST = 0;
const int PRNietblad::WEST = 1;
const int PRNietblad::NOORD = 0;
const int PRNietblad::ZUID = 2;
#endif


#endif //LABO_4_PR_QUADTREES_PRQUADTREEV2_H
