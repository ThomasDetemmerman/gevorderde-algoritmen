#ifndef __Zoekboom_H
#define __Zoekboom_H
#include <cstdlib>
#include <iostream>
#include <queue>
#include <memory>
#include <functional>
#include <fstream>
#include <sstream>

#include <iostream>
#include "zoekknoop.h"

using std::cerr;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::pair;
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
    Zoekboom(){};                                                                                         //default
    Zoekboom(const Zoekboom &) = default;                                                                 // copy 1
    Zoekboom &operator=(Zoekboom &&) = default;                                                           //move operator 
    Zoekboom &operator=(const Zoekboom &) = default;                                                      //move operator 
    Zoekboom(Zoekboom &&) = default;                                                                      // move assignment

    // constructoren met zoekknoop als parameter
    // dit is de belangrijkste constructor van deze oefening. Het zet een pointer naar een zoekKnoop om naar een zoekboom.
    Zoekboom(unique_ptr<zoekKnoop<Sleutel, Data>> &&k) : unique_ptr<zoekKnoop<Sleutel, Data>>(move(k)){}; //move 2
    /*
    Unique pointers kun je niet kopieeren. 
    copy operatoren kunnen echter wel zinvol zijn op voorwaarde dat je een deep copy implementeerd en dus de volledige boom kopieerd.

    Zoekboom(const unique_ptr<zoekKnoop<Sleutel, Data>> &k) : unique_ptr<zoekKnoop<Sleutel, Data>>(k){};  //copy 2
    Zoekboom &operator=(const unique_ptr<zoekKnoop<Sleutel, Data>> &a){return Zoekboom(a); };             //copy assingment operator required by voegtoe
    */

    //destructor
    virtual ~Zoekboom() = default;

    void inorder(std::function<void(const zoekKnoop<Sleutel, Data> &)> bezoek) const;
    void schrijf(ostream &os) const;
    void teken(const char *bestandsnaam);
    string tekenrec(ostream &uit, int &knoopteller);

    //te implementeren
    bool repOK() const;
    int geefDiepte();
    // geefBoomBovenKnoop: gegeven een knooppointer, wele boom wijst naar de knoop
    // preconditie: knoop moet een naar een geldige knoop wijzen.
    Zoekboom<Sleutel, Data> *geefBoomBovenKnoop(zoekKnoop<Sleutel, Data> &knoopptr);
    void voegtoe(const Sleutel &sleutel, const Data &data, bool dubbelsToestaan = false);

    //eigen
    void roteer(bool naarRechts);
    void maakOnevenwichtig();

protected:
    //zoekfunctie zoekt sleutel en geeft de boom in waaronder de sleutel zit (eventueel een lege boom als de sleutel
    //ontbreekt) en de pointer naar de ouder (als die bestaat, anders nulpointer).
    //noot: alhoewel de functie niets verandert aan de boom is ze geen const functie.
    void zoek(const Sleutel &sleutel, zoekKnoop<Sleutel, Data> *&ouder, Zoekboom<Sleutel, Data> *&plaats);
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
string Zoekboom<Sleutel, Data>::tekenrec(ostream &uit, int &knoopteller)
{
    ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';
    if (!*this)
    {
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        uit << wortelstring.str() << "[label=\"" << (*this)->sleutel << ":" << (*this)->data << "\"]";
        uit << ";\n";
        string linkskind = (*this)->links.tekenrec(uit, knoopteller);
        string rechtskind = (*this)->rechts.tekenrec(uit, knoopteller);
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
    Zoekboom<Sleutel, Data>::zoek(sleutel, ouder, plaats);
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
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::zoek(const Sleutel &sleutel, zoekKnoop<Sleutel, Data> *&ouder, Zoekboom<Sleutel, Data> *&plaats)
{
    plaats = this;
    ouder = 0;
    while (*plaats && (*plaats)->sleutel != sleutel)
    {
        ouder = plaats->get();
        if ((*plaats)->sleutel < sleutel)
            plaats = &(*plaats)->rechts;
        else
            plaats = &(*plaats)->links;
    };

};

/* eigen implementatie */


template <class Sleutel, class Data>
int Zoekboom<Sleutel, Data>::geefDiepte(){
    int L_diepte, R_diepte;

    if((*this)->links){
        L_diepte = (*this)->links.geefDiepte();
    } else {
         L_diepte = 0;
    
    }   

     if((*this)->rechts){
       R_diepte = (*this)->rechts.geefDiepte();
    } else {
     
      R_diepte = 0;
    }   

    return (L_diepte > R_diepte) ? (L_diepte+1) : (R_diepte+1);
   
       
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
void Zoekboom<Sleutel, Data>::roteer(bool naarRechts){
    // koppel I los van P
    Zoekboom<Sleutel, Data> pointerToI = move((*this)->geefKind(naarRechts));
    //assert(!pointerToI); //a rotate function requires a child.
    if(!pointerToI){
        std::cout << (*this)->sleutel  << " heeft niet het juiste kind" << std::endl;
        (*this).teken("error");
    }
    // rechter kind van I wordt linkerkind van P
    (*this)->geefKind(naarRechts) = move(pointerToI->geefKind(!naarRechts)); // to do: kan dit een segmetation fault opleveren? kan ik een lege pointer moven?

    // P wordt nu linkerkind van I
    pointerToI->geefKind(!naarRechts) = move(*this);

    // zet I als nieuwe parent
    *this = move(pointerToI);

};

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::maakOnevenwichtig(){
   
   // als er een rechterkind is
    if((*this)->rechts){
        // zolang er links kinderen zijn gaan we naar rechts roteren
        while((*this)->links){
            (*this).roteer(true);
        }
        //recursief herhalen. Het is belangrijk dat je eerst alle kinderen van links naar rechts roteert. Als er namelijk linker kinderen zijn
        //met een rechter kleinkind zouden deze niet weggeroteerd worden. Als we eerst roteren en dan pas recursief herhalen heb je dit probleem niet.
        //snap je niet wat ik bedoel? wissel beide statements om en run het programma.
        (*this)->rechts.maakOnevenwichtig();
        
    }
};

#endif
