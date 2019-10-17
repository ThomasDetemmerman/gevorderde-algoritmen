//
// Created by Thomas on 17/10/2019.
//

#ifndef LABO_4_PR_QUADTREES_PRQUADTREE_H
#define LABO_4_PR_QUADTREES_PRQUADTREE_H


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
#include <algorithm>
using std::unique_ptr;

/*****************************************************************************
    Bevat de headers en code voor een PRQuadtree
    en voor de abstracte klasse PRKnoop met deelklassen Blad en Nietblad

*****************************************************************************/


class PRKnoop;
typedef unique_ptr<PRKnoop> Knoopptr;
class PRQuadtree;



class PRQuadtree: public Knoopptr{
public:
    using Knoopptr::unique_ptr;
    PRQuadtree(int a):maxcoordinaat{a}{};
    PRQuadtree( Knoopptr&& a):Knoopptr(move(a)){};
    PRQuadtree& operator=(Knoopptr&& a){
        Knoopptr::operator=(move(a));
        return *this;
    };
    void preEnPostOrder(std::function<void(PRKnoop*)>& bezoekPre,std::function<void(PRKnoop*)>& bezoekPost) const;

    //te implementeren
    void voegToe(int x, int y);
    int geefDiepte();
//de PRquadtree kan alleen punten bevatten met
//-maxcoordinaat <= x < maxcoordinaat
//-maxcoordinaat <= y < maxcoordinaat
    int maxcoordinaat;
};

//Opmerking: om de functies specifiek aan een deelklasse te kunnen gebruiken moet je soms een
//static_cast doen, zoals in
//  PRKnoop* knoopptr=...;
//  if (knoopptr!=nullptr && knoopptr->isBlad() && static_cast<PRBlad*>(knoopptr)->x == 5)
class PRKnoop{
public:
    virtual bool isBlad()=0;
    virtual int geefDiepte()=0;
};
class PRBlad:public PRKnoop{
public:
    PRBlad(int x,int y):x{x},y{y}{};
    virtual bool isBlad(){ return true;}
    int x,y;//co"ordinaten punt
    virtual int geefDiepte(){
        return 1;
    };
};
class PRNietblad:public PRKnoop{
public:
    virtual bool isBlad(){ return false;}
    int geefDiepte(){
        assert("ERROR");
    };
    int geefAantalKinderen(){
        int aantal=0;
        for (int i=0; i<4; i++ ){
            if (kind[i])
                ++aantal;
        }
        return aantal;
    };
    //xc, yc: co"ordinaten van het centrum van het gebied
    Knoopptr* geefKind(int x, int y, int xc, int yc){
        int xindex=(x<xc? WEST : OOST);
        int yindex=(y<yc? NOORD : ZUID);
        return &kind[xindex+yindex];
    };
    static const int OOST, WEST, NOORD, ZUID;
    Knoopptr kind[4];//indexeren met windrichting (bv. kind[NOORD+WEST] bevat punten
    //met x en y kleiner dan grenswaarde)
    //leeg gebied: nulpointer
};

void PRQuadtree::preEnPostOrder(std::function<void(PRKnoop*)>& bezoekPre,std::function<void(PRKnoop*)>& bezoekPost) const{
    enum staat {pre,post};
    std::stack<std::pair<PRKnoop*,staat>> DEstack;//bevat alleen niet-nulpointers
    if (*this)
        DEstack.emplace(this->get(),pre);
    while (!DEstack.empty()){
        auto [nuknoop,nustaat]=DEstack.top();
        if (nustaat==pre){
            bezoekPre(nuknoop);
            DEstack.top().second=post;
            if (!nuknoop->isBlad()){
                for (int i=0; i<4; i++ ){
                    Knoopptr* kind=&(static_cast<PRNietblad*>(nuknoop)->kind[i]);
                    if (*kind)
                        DEstack.emplace(kind->get(),pre);
                };
            };
        } else{
            bezoekPost(nuknoop);
            DEstack.pop();
        };
    };

};

int PRQuadtree::geefDiepte()
{
    if(!(*this)){
        return 0;
    }
    int maxDiepte = 1;


    if(!(*this)->isBlad()){
        for(int i=0; i < 4; i++){
            int dieptekind = static_cast<PRNietblad*>(this->get())->kind[i]->geefDiepte();
            maxDiepte = std::max(maxDiepte, dieptekind);
        }
    }
    return maxDiepte;
};

void PRQuadtree::voegToe(int x, int y) {
    if(!(*this)){
       Knoopptr nieuw = std::make_unique<PRBlad>(x,y);
       *this = move(nieuw);
    } else{
        if((*this)->isBlad()){
            auto huidigBlad = static_cast<PRBlad*>(this->get());

            unique_ptr<PRNietblad> nieuwinwendigeKnoop = std::make_unique<PRNietblad>();
            Knoopptr* kind = nieuwinwendigeKnoop->geefKind(huidigBlad->x, huidigBlad->y, 0,0);

            *kind = std::move(*this);

            // om naar een bovenklasse te casten moeten we een tussenstap nemen.
            // de compiler zei dat het ambigu was, maw hij wist niet hoe het moest. Wij moeten dus expliciet aangeven dat we via een prknoop willen gaan.
            unique_ptr<PRKnoop> tussenstap = move(nieuwinwendigeKnoop);
            *this = std::move(tussenstap);

            //voeg nieuwe knoop toe
            auto oudBlad = std::make_unique<PRBlad>(x,y);
            

        }

    }
};

const int PRNietblad::OOST=0;
const int PRNietblad::WEST=1;
const int PRNietblad::NOORD=0;
const int PRNietblad::ZUID=2;
#endif




#endif //LABO_4_PR_QUADTREES_PRQUADTREE_H
