#ifndef __Zoekboom_H
#define __Zoekboom_H
#include <cstdlib>
#include <queue>
#include <memory>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>

#include <stack>
#include "zoekknoop.h"

using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::pair;
using std::stack;
using std::string;
using std::unique_ptr;
/**********************************************************************

   Class: Zoekboom
   
   beschrijving: Binaire Zoekboom waarin duplicaatsleutels wel of niet zijn toegestaan.
   
***************************************************************************/

template <class Sleutel, class Data>
class zoekKnoop;

template <class Sleutel, class Data>
class Zoekboom : public unique_ptr<zoekKnoop<Sleutel, Data>>
{
public:
    //....move en copy. Noot: als er geen copy nodig is, zet hem beste op delete.

    // constructoren met zoekboom als parameter
    Zoekboom(){};                                    //default
    Zoekboom(const Zoekboom &) = default;            // copy 1
    Zoekboom &operator=(Zoekboom &&) = default;      //move operator
    Zoekboom &operator=(const Zoekboom &) = default; //move operator
    Zoekboom(Zoekboom &&) = default;                 // move assignment

    // constructoren met zoekknoop als parameter
    // dit is de belangrijkste constructor van deze oefening. Het zet een pointer naar een zoekKnoop om naar een zoekboom.
    Zoekboom(unique_ptr<zoekKnoop<Sleutel, Data>> &&k) : unique_ptr<zoekKnoop<Sleutel, Data>>(move(k)){}; //move 2

    //destructor
    virtual ~Zoekboom() = default;

    void inorder(std::function<void(const zoekKnoop<Sleutel, Data> &)> bezoek) const;
    void schrijf(ostream &os) const;
    void teken(const char *bestandsnaam);
    string tekenrec(ostream &uit, int &knoopteller);

    //te implementeren
   
    int geefDiepte();
    // geefBoomBovenKnoop: gegeven een knooppointer, wele boom wijst naar de knoop
    // preconditie: knoop moet een naar een geldige knoop wijzen.
    Zoekboom<Sleutel, Data> *geefBoomBovenKnoop(zoekKnoop<Sleutel, Data> &knoopptr);
    void voegtoe(const Sleutel &sleutel, const Data &data, bool dubbelsToestaan = false);
    
private:
    void roteer(bool naarRechts);
    bool repOK() const;
    bool repOK_redParentWithBlackChilds() const;
    bool repOK_blackRoot() const;
    void fixBoom(stack<Zoekboom<Sleutel, Data> *> voorouders);
    bool isLeftChildOfParent( Zoekboom<Sleutel, Data> &p);
    Kleur getColorOfBrother(Zoekboom<Sleutel, Data> &p);

protected:
    //zoekfunctie zoekt sleutel en geeft de boom in waaronder de sleutel zit (eventueel een lege boom als de sleutel
    //ontbreekt) en de pointer naar de ouder (als die bestaat, anders nulpointer).
    //noot: alhoewel de functie niets verandert aan de boom is ze geen const functie.
    stack<Zoekboom<Sleutel, Data> *> zoek(const Sleutel &sleutel, zoekKnoop<Sleutel, Data> *&ouder, Zoekboom<Sleutel, Data> *&plaats);
};

/*****************************************************************************

    Implementatie
    
*****************************************************************************/

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::inorder(std::function<void(const zoekKnoop<Sleutel, Data> &)> bezoek) const
{
    if (*this)
    {
        (*this)->links.inorder(bezoek);
        bezoek(**this);
        (*this)->rechts.inorder(bezoek);
    };
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::schrijf(ostream &os) const
{
    inorder([&os](const zoekKnoop<Sleutel, Data> &knoop) {
        os << "(" << knoop.sleutel << " -> " << knoop.data << ")";
        os << "\n  Linkerkind: ";
        if (knoop.links)
            os << knoop.links->sleutel;
        else
            os << "-----";
        os << "\n  Rechterkind: ";
        if (knoop.rechts)
            os << knoop.rechts->sleutel;
        else
            os << "-----";
        os << "\n";
    });
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::teken(const char *bestandsnaam)
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\n";
    this->tekenrec(uit, knoopteller);
    uit << "}";
}

template <class Sleutel, class Data>
string Zoekboom<Sleutel, Data>::tekenrec(ostream &uit, int &nullteller)
{
    ostringstream wortelstring;
    if (!*this)
    {
        wortelstring << "null" << ++nullteller;
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        wortelstring << '"' << (*this)->sleutel << '"';
        uit << wortelstring.str() << "[label=\"" << (*this)->sleutel << "\"]";
        if ((*this)->kleur == Kleur::Rood)
            uit << "[color=red]";
        uit << ";\n";
        string linkskind = (*this)->links.tekenrec(uit, nullteller);
        string rechtskind = (*this)->rechts.tekenrec(uit, nullteller);
        uit << wortelstring.str() << " -> " << linkskind << ";\n";
        uit << wortelstring.str() << " -> " << rechtskind << ";\n";
    };
    return wortelstring.str();
}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data> *Zoekboom<Sleutel, Data>::geefBoomBovenKnoop(zoekKnoop<Sleutel, Data> &knoop)
{
    if (knoop.ouder == 0)
        return this;
    else if (knoop.ouder->links.get() == &knoop)
        return &(knoop.ouder->links);
    else
        return &(knoop.ouder->rechts);
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::voegtoe(const Sleutel &sleutel, const Data &data, bool dubbelsToestaan)
{
    
    zoekKnoop<Sleutel, Data> *ouder;
    Zoekboom<Sleutel, Data> *plaats;
    stack<Zoekboom<Sleutel, Data> *> voorouders = zoek(sleutel, ouder, plaats);
    if (dubbelsToestaan)
        while (*plaats)
            (*plaats)->geefKind(random() % 2).zoek(sleutel, ouder, plaats);
    if (!*plaats)
    {
        Zoekboom<Sleutel, Data> nieuw =
            std::make_unique<zoekKnoop<Sleutel, Data>>(sleutel, data);
        nieuw->ouder = ouder;
        *plaats = move(nieuw);
    }
    std::cout << "wij voegen toe: " << sleutel << std::endl;
    fixBoom(voorouders);
}

/*
*   Deze functie kan gezien worden als een verlengde van voegToe. Om de code van voegToe niet te onoverzichtelijk
*   te maken is de code om de boom te repareren na toevoegen in een apparte functie gestoken.
*/
template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::fixBoom(stack<Zoekboom<Sleutel, Data> *> voorouders)
{
    // Situatie 0: de wortel mag altijd zwart gemaakt worden
    // empty slaat op een boom met maar 1 knoop.
    if(voorouders.size() == 1 || voorouders.empty()){
        (*this)->kleur = Kleur::Zwart;
        if (voorouders.size() == 1) {
            voorouders.pop();
        }
        return;
    }
    
    if(voorouders.size()< 2){
        // we hebben minstens een ouder en grootouder nodig om verder te kunnen werken
        return;
    }

    // p van parent
    Zoekboom<Sleutel, Data> *p = voorouders.top();
    voorouders.pop();
    // g van grandparent
    Zoekboom<Sleutel, Data> *g = voorouders.top();

    if((*p)->kleur == Kleur::Zwart){
        //indien p zwart is moet er niets gebeuren. Want een nieuw toegevoegde rode knoop met zwarte ouder is ok.
        std::cout << "p is zwart. Niets te doen"<< std::endl;
        return;
    }
    bool pIsLeft = p->isLeftChildOfParent(*g);
    bool cIsLeft = this->isLeftChildOfParent(*p);

    // volgens p9 en volgende van de cursus

    // situatie 1: g is zwart en heeft twee rode kinderen. Uiteraard is het kleinkind ook rood.
    // oplossing: g wordt rood en zijn kinderen zwart.
    // TO DO: kan  (*(*g)->links)->kleur een segmentation fault werpen??

    // ik denk dat je niet moet controleren of G zwart is, volgens de cursus is dit altijd het geval
    // ((*g)->kleur == Kleur::Zwart) 
    if(getColorOfBrother(*g) == Kleur::Rood ){
        std::cout << "situatie 1: b(" << (*(*g)->geefKind(!pIsLeft)).sleutel << ") is rood"<< std::endl;

        (*(*g)->geefKind(!pIsLeft)).kleur = Kleur::Zwart;
        (*p)->kleur = Kleur::Zwart;
        (*g)->kleur = Kleur::Rood;
        if(!pIsLeft){
        std::cout << "b(" << (*(*g)->geefKind(!pIsLeft)).sleutel << ") is links kind and will be black" << std::endl;
        }
        else {
        std::cout << "b(" <<  (*(*g)->geefKind(!pIsLeft)).sleutel << ") is rechts kind and will be black" << std::endl;
        }
        
        std::cout << "p(" << (*p)->sleutel << ") will be black" << std::endl;
        std::cout << "g(" << (*g)->sleutel << ") will be red" << std::endl;
    } else {
        // situatie 3: De broer b van p is zwart
        // oplossing: roteer in de richting van de zwarte broer.
        if((pIsLeft && !cIsLeft) || (!pIsLeft && cIsLeft)){ //indien p rechterkind is en c linkerkind (of spiegelbeeld)
            std::cout << "situatie 3: b is zwart"<< std::endl;
            // stel p is linkerkind. Dus geefLinkerkind en dit gaan we in de andere richting roteren als p is. Dus roteer naar rechts = false
            (*g)->geefKind(pIsLeft).roteer(!pIsLeft); 
            
            
        }
        std::cout << "situatie 2: b is zwart"<< std::endl;
        g->roteer(cIsLeft);
        (*p)->kleur = Kleur::Zwart;
        (*g)->kleur = Kleur::Rood;
       
    }

    if( !repOK()){
        fixBoom(voorouders);
    }else {
        std::cout << "rep oke, next" << std::endl;
    }
  
  


}

/*
*   Sommige operaties zijn geïnteresseerd in de voorouder geschiedenis, andere niet. Optionele parameters zijn niet mogelijk in cpp
*   dus heb ik ervoor gekozen om de vooroudergeschiedenis te returnen. Dit kan niet als reference in tegenstelling tot wanneer we met een
*   parameter hadden gewerkt. We declareren namelijk voorouders lokaal (binnen de scope van de functie), wanneer de functie eindigd zal deze
*   variabele opgeruimd worden. We moeten dus het volledige object returnen. Wat je wel kan doen is voorouders = move(zoek(...)) doen ookal
*   ik vermoed dat de compiler dat automatisch doet aangezien die ook wel weet dat het obj ten einde is.
*/
template <class Sleutel, class Data>
stack<Zoekboom<Sleutel, Data> *> Zoekboom<Sleutel, Data>::zoek(const Sleutel &sleutel, zoekKnoop<Sleutel, Data> *&ouder, Zoekboom<Sleutel, Data> *&plaats)
{
    stack<Zoekboom<Sleutel, Data> *> voorouders;
    plaats = this;
    ouder = 0;
    while (*plaats && (*plaats)->sleutel != sleutel)
    {
        ouder = plaats->get();
        voorouders.push(plaats);
        if ((*plaats)->sleutel < sleutel)
            plaats = &(*plaats)->rechts;
        else
            plaats = &(*plaats)->links;
    };
    return voorouders;
};
template <class Sleutel, class Data>
bool Zoekboom<Sleutel, Data>::isLeftChildOfParent( Zoekboom<Sleutel, Data> &p){
    return (p->links == *this);
}

template <class Sleutel, class Data>
Kleur Zoekboom<Sleutel, Data>::getColorOfBrother(Zoekboom<Sleutel, Data> &p){
   
    if(p->geefKind(!this->isLeftChildOfParent(p))){ //indien het andere kind bestaat vragen we zijn kleur op
        return p->geefKind(!this->isLeftChildOfParent(p))->kleur;
    } else {
        // als het niet bestaat is het en virtuele knoop en is deze dus rood.
        return Kleur::Rood;
    }
    
    
};

template <class Sleutel, class Data>
bool Zoekboom<Sleutel, Data>::repOK() const{
 return repOK_blackRoot() && repOK_redParentWithBlackChilds();
};
   

   

template <class Sleutel, class Data>
bool  Zoekboom<Sleutel, Data>::repOK_redParentWithBlackChilds() const{
    bool statusOk = true;
    if((*this)->kleur == Kleur::Rood){
        if((*this)->links) {
            if((*this)->links->kleur == Kleur::Rood){
                statusOk = false;}
            else {
                statusOk = (*this)->links.repOK_redParentWithBlackChilds();
            }
        }
         if((*this)->rechts) {
            if((*this)->rechts->kleur == Kleur::Rood){
                statusOk = false;}
            else {
                statusOk = (*this)->rechts.repOK_redParentWithBlackChilds();
            }
        }
    }
    return statusOk;

};

template <class Sleutel, class Data>
bool  Zoekboom<Sleutel, Data>::repOK_blackRoot() const{
  // root is altijd zwart
    return ((*this)->kleur == Kleur::Zwart);
};

template <class Sleutel, class Data>
int Zoekboom<Sleutel, Data>::geefDiepte()
{
    int L_diepte, R_diepte;

    if ((*this)->links)
    {
        L_diepte = (*this)->links.geefDiepte();
    }
    else
    {
        L_diepte = 0;
    }

    if ((*this)->rechts)
    {
        R_diepte = (*this)->rechts.geefDiepte();
    }
    else
    {

        R_diepte = 0;
    }

    return (L_diepte > R_diepte) ? (L_diepte + 1) : (R_diepte + 1);
};

template <class Sleutel, class Data>
/*
*            P
*           / \
*          I   △
*         / \
*        △   △ 
*        α   ß
*/
void Zoekboom<Sleutel, Data>::roteer(bool naarRechts)
{
    //je kan niet roteren op een lege (deel)boom
    assert(this);

    // koppel I los van P
    Zoekboom<Sleutel, Data> pointerToI = move((*this)->geefKind(naarRechts));

    //a rotate function requires a child.
    assert(pointerToI);

    // rechter kind van I wordt linkerkind van P
    (*this)->geefKind(naarRechts) = move(pointerToI->geefKind(!naarRechts)); // to do: kan dit een segmetation fault opleveren? kan ik een lege pointer moven? antwoord: dit is ok.

    // P wordt nu linkerkind van I
    pointerToI->geefKind(!naarRechts) = move(*this);

    // zet I als nieuwe parent
    *this = move(pointerToI);
};

#endif
