#ifndef __SUFFIXTREE_H
#define __SUFFIXTREE_H
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <deque>
#include <memory>
using std::unique_ptr;
using std::make_unique;
#include "bitpatroon.h"
using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;
using std::map;
using std::deque;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;

class Sknoop;
class InwendigeKnoop;
class Blad;
class Presuffix;
class Sboom;

/**********************************************************************

   Class: Kindtab
   
    Klassieke trie heeft een tabel; ternaire boom een map. Dit wordt geencapsuleerd in de klasse
    Kindtab.
    Het doel van Kindtab is ook om het eigenaardige gedrag van een map op te vangen dat maakt
    dat het opvragen van van een mapelement de map kan veranderen. Verbergt de manier om de
    meerwegstrie te implementeren.
    
***************************************************************************/

typedef map<char, unique_ptr<Sknoop>> Kindtype;
typedef unsigned char uchar;
class Kindtab: public Kindtype{
public:
    void set(char a, unique_ptr<Sknoop>&& p);
    //geeft nulpointer terug als a geen beeld heeft in de map
    Sknoop* get(char a) const;
    unique_ptr<Sknoop> remove(char a);
};

/**********************************************************************

   Class: Presuffix
   
   De klasse Presuffix heeft alleen belang bij het opstellen van de Sboom.
   Beschrijving: Verwijst naar een expliciete knoop, anders gezegd naar
   het prefix van een suffix. De  expliciet-pointer wijst naar een expliciete knoop, de string
   die het zoekpad vanaf de wortel geeft wordt aangegeven door zijn lengte lengteTotExpliciet
   en beginpositie plaatsInHooiberg.

   De parameternaam i is voorbehouden voor het doorgeven van de huidige verwerkingsplaats in de hooiberg

   Het Presuffix is canoniek als het suffix (beginnend op plaatsInHooiberg
   en eindigend VOOR i) ofwel eindigt in *expliciet (hooiberg[i] duidt dan dus het kind
   van expliciet aan waarlangs we verder gaan; i= plaatsInHooiberg + lengteTotExpliciet
   en explicietEinde() = true )
   ofwel in een impliciete knoop die als expliciete ouder *expliciet heeft.
   
***************************************************************************/
class Presuffix{
    friend class Sboom;
public:
    Presuffix(InwendigeKnoop* _expliciet=0, int _plaats=0);
    void init(InwendigeKnoop* _expliciet=0, int _plaats=0);
    void verzet();
    //noot explicietEinde werkt alleen als het Presuffix canonisch van vorm is
    bool explicietEinde(int i) const{
        return lengteTotExpliciet ==i-plaatsInHooiberg;
    };
    //volgende functie ontploft (soms) als knoopwijzer niet canonisch is
    bool heeftKind(int i, const string& hooiberg) const;
    //als het stuk van plaatsInHooiberg tot i gekend is,
    //ga zo ver mogelijk naar beneden in de boom
    void maakCanoniek(int i, const string& hooiberg);
    bool isCanoniek(int i,  const string& hooiberg) const;
    int geefPlaatsInHooiberg(){return plaatsInHooiberg;};
protected:
//lidvelden
    InwendigeKnoop* expliciet;//laatste expliciete knoop, voor zover bekend, in pad van suffix.
    int plaatsInHooiberg;
    int lengteTotExpliciet;//lengte van string die tot expliciete knoop leidt. 0 als Expliciet naar wortel leidt
};

/**********************************************************************

   einde class Presuffix
   
***************************************************************************/



/**********************************************************************

   Class: Sknoop
   
   beschrijving: Vermits een Sboom gecomprimeerd is moet er een plaats zijn waar
   de gecomprimeerde takdata worden opgeslagen. Dit gebeurt door de indices startc en
   eindc die respectievelijk het eerste karakter en de plaats na het laatste karakter in
   de hooiberg aangeven. Als er geen tussenliggende impliciete knopen zijn dan is
   eindc==startc;

    Voor een blad wijst eindc voorbij het einde van het bijhorende suffix en dus het einde van
    de tekst waar het suffix bijhoort. Het afsluitkarakter voor die tekst is dan hooiberg[eindc-1];
   
   Als we in de ouder van een Sknoop k zitten dan is ouder->kind.get(hooiberg[k.startc]) de pointer naar k.
   Deelklasse: een InwendigeKnoop heeft kinderen; een Blad niet.
   
   
***************************************************************************/
class Sknoop{
friend class Sboom;
friend class Presuffix;
public:
    Sknoop();
    virtual bool isBlad()=0;
    //de volgende functie vult niet alleen knoopverzameling in, de verzameling teksten (gegeven als bitpatroon)
    //die horen bij bladeren onder de knoop, maar ook uit: als een knoop merkt dat hij bladeren onder
    //zich heeft voor alle bestanden in gezocht en zijn padstring is langer dan uit, dan vult hij uit in.
    //sluitmap: beeldt afsluitkarakters af op een bitpatroon met een 1 op de juiste plaats..
    // op nul gezet door oproeper.
    //uit: de tot nu toe langste string
    //int diepte: de lengte van het prefix tot aan knoop die deze functie oproept.
    //              (eigenlijk overbodig, volgt uit startp en eindp)
    //gezocht: Bitpatroon met enen dat de complete gezochte verzameling aanduidt
    virtual void geefLGDrec(const vector<Bitpatroon> &sluitmap ,Bitpatroon&  knoopverzameling,
                    string& uit,int diepte,const Bitpatroon& gezocht,
                     const string& hooiberg)=0;
protected:
    int startp; //p staat voor prefix: startp en eindp duiden begin en einde aan in hooiberg van prefix
    int eindp;  //nuttig voor toepassingen zoals lgd
    int startc; //zie uitleg bij klasse
    int eindc;


};

class Blad: public Sknoop{
friend class Sboom;
public:
    virtual bool isBlad();
    virtual void geefLGDrec(const vector<Bitpatroon> &sluitmap ,Bitpatroon&  knoopverzameling,
                    string& uit,int diepte,const Bitpatroon& gezocht,
                     const string& hooiberg);
};



class InwendigeKnoop: public Sknoop{
friend class Sboom;
friend class Presuffix;
public:
    InwendigeKnoop();
    virtual bool isBlad();
    virtual void geefLGDrec(const vector<Bitpatroon> &sluitmap ,Bitpatroon&  knoopverzameling,
                    string& uit,int diepte,const Bitpatroon& gezocht,
                     const string& hooiberg);
protected:
//datavelden
    Kindtab kind;
    InwendigeKnoop* staart;
};


/**********************************************************************

   Class: Sboom
   
   beschrijving: Suffixboom. Wordt opgebouwd vanuit een cstring die uiteraard
   geen nulkarakter bevat.
   Noteer: de wortelknoop representeert de lege string, die uiteraard een
   geldige suffix is van een willekeurige string en dus steeds aanwezig is.
   
***************************************************************************/
class Sboom{
public:
    Sboom();
    //voegtoe voegt een tekst toe met het opgegeven afsluitkarakter
    //Verschillende teksten moeten verschillende afsluitkarakters hebben!
    void voegtoe (const string& s, char afsluiter=-1);

    //geefLGD geeft de langste gemeenschappelijke deelstring van alle teksten
    //met de opgegeven afsluitkarakters.
    string geefLGD(const vector<char>& sluit) const;
protected:
    //datavelden
    string hooiberg;//let op: afgesloten met afsluitkarakter
    int lengte;//lengte van de hooiberg.
    unique_ptr<InwendigeKnoop> wortel;
};


/**********************************************************************


    Implementatie


*********************************************************************/


/**********************************************************************
     Klasse Presuffix
*********************************************************************/
void Presuffix::verzet(){
    plaatsInHooiberg++;
    if (lengteTotExpliciet > 0)//we zitten niet in wortel
        lengteTotExpliciet--;
    expliciet=expliciet->staart;
};

bool Presuffix::heeftKind(int i,  const string& hooiberg) const{
    bool gevonden;
    if (explicietEinde(i))
        gevonden=expliciet->kind.get(hooiberg[i]) != 0;
    else{
        Sknoop* kind=expliciet->kind.get(hooiberg[plaatsInHooiberg+lengteTotExpliciet]);
        int verder= i - lengteTotExpliciet - plaatsInHooiberg;//hoeveel impliciete knopen verder?
        char c=hooiberg[kind->startc+verder];
        gevonden=(hooiberg[i]==c);
    }
    return gevonden;
}



Presuffix::Presuffix(InwendigeKnoop* _expliciet,int _plaats):expliciet(_expliciet),plaatsInHooiberg(_plaats),
                                    lengteTotExpliciet(0){
                                    }
void Presuffix::init(InwendigeKnoop* _expliciet,int _plaats){
    expliciet=_expliciet;
    plaatsInHooiberg=_plaats;
    lengteTotExpliciet=0;
}

bool Presuffix::isCanoniek(int i,  const string& hooiberg) const{
    int overschot=i-lengteTotExpliciet-plaatsInHooiberg;//gaat voorbij expliciete knoop; 0 --> einde in expliciete knoop
    if (overschot > 0){
        Sknoop* kind=expliciet->kind.get(hooiberg[i-overschot]);
        return(kind->isBlad() || kind->eindc - kind->startc >= overschot);
    }
    else
        return true;
};

void Presuffix::maakCanoniek(int i,  const string& hooiberg){
    int overschot=i-lengteTotExpliciet-plaatsInHooiberg;//gaat voorbij expliciete knoop; 0 --> einde in expliciete knoop
    if (overschot > 0){
        Sknoop* kind=expliciet->kind.get(hooiberg[i-overschot]);
        while (overschot > 0 && !kind->isBlad()
            && kind->eindc - kind->startc < overschot){
            expliciet=(InwendigeKnoop*) kind;
            lengteTotExpliciet=lengteTotExpliciet + (kind->eindc -kind->startc) +1 ;
            overschot=i-lengteTotExpliciet-plaatsInHooiberg;//gaat voorbij expliciete knoop; 0 --> einde in expliciete knoop
            kind=expliciet->kind.get(hooiberg[i-overschot]);
        };
    }
}

/**********************************************************************
     Klasse Sknoop
*********************************************************************/


Sknoop::Sknoop():startc(eindc){}

bool Blad::isBlad(){return true;}

bool InwendigeKnoop::isBlad(){return false;}

InwendigeKnoop::InwendigeKnoop():Sknoop(),staart(0){}



/**********************************************************************
     Klasse Kindtab
*********************************************************************/


void Kindtab::set(char a,unique_ptr<Sknoop>&& p){
//    Sknoop* in=p.get();
    std::pair<char,unique_ptr<Sknoop>> paar(a,move(p));
    Kindtype::insert(move(paar));
//    assert (this->operator[](a).get()==in);

}

Sknoop* Kindtab::get(char a) const{
    Kindtype::const_iterator plaats=find(a);
    if (plaats==Kindtype::end())
        return 0;
    else
        return plaats->second.get();
}

unique_ptr<Sknoop> Kindtab::remove(char a){
    Kindtype::iterator plaats=find(a);
    if (plaats==Kindtype::end())
        return 0;
    else{
        unique_ptr<Sknoop> res(move(plaats->second));
        erase(plaats);
        return res;
    };
};



/**********************************************************************
     Klasse Sboom
*********************************************************************/

Sboom::Sboom():wortel(make_unique<InwendigeKnoop>()){
    wortel->staart=wortel.get();
    wortel->startp=wortel->eindp=0;
    wortel->startc=wortel->eindc=666;
}


string Sboom::geefLGD(const vector<char>& sluit) const{
    string uit;
    assert (sluit.size()<=patroonlengte);
    Bitpatroon knoopverzameling;//verzameling waar suffix inzit
    vector<Bitpatroon> sluitmap(256);
    Bitpatroon gezocht;
    for (int i=0; i<sluit.size(); i++ ){
        sluitmap[static_cast<uchar>(sluit[i])]=Bitpatroon::eenbit(i);
        gezocht|=Bitpatroon::eenbit(i);
    }
    wortel->geefLGDrec(sluitmap,knoopverzameling,uit,0,gezocht, hooiberg);
    return uit;
}


void Sboom::voegtoe (const string& tekst,char afsluiter){
    int oudelengte=hooiberg.size();
    hooiberg+=tekst;
    hooiberg+=afsluiter;
    lengte=hooiberg.size();
    Presuffix actief(wortel.get(),oudelengte);
    for (int i=oudelengte; i<lengte ; i++ ){
        InwendigeKnoop* staartUpdate=0;//wijst eventueel naar knoop met nog in te vullen staartpointer
//eyecandy voor grote teksten
        if (i%1000==1)
            cerr<<"*";
        actief.maakCanoniek(i,hooiberg);
        Presuffix laatstestaart=actief;//verwijst naar de laatste knoop op het pad van het actief suffix
                                    //dat zeker een ingevulde staart heeft.
        char nuchar=hooiberg[i];
        while (actief.geefPlaatsInHooiberg()<=i && !actief.heeftKind(i, hooiberg)){
            laatstestaart=actief;
            if (!actief.explicietEinde(i)){
            //maak een expliciete knoop om blad aan te hangen
                unique_ptr<Sknoop> volgend=
                actief.expliciet->kind.remove(hooiberg[actief.plaatsInHooiberg+actief.lengteTotExpliciet]);
                int overschot=i-actief.plaatsInHooiberg-actief.lengteTotExpliciet;
                unique_ptr<InwendigeKnoop> nieuw=make_unique<InwendigeKnoop>();
                nieuw->startc=volgend->startc;
                nieuw->eindc=nieuw->startc+overschot-1;
                nieuw->startp=actief.plaatsInHooiberg;
                nieuw->eindp=i;
                
                if (staartUpdate != 0){
                    staartUpdate->staart=nieuw.get();
                }
                staartUpdate=nieuw.get();

                volgend->startc=volgend->startc+overschot;
                char oudchar=hooiberg[volgend->startc];
                nieuw->kind.set(oudchar,move(volgend));
                actief.expliciet->kind.set(hooiberg[i-overschot],move(nieuw));
                actief.expliciet=staartUpdate;
                actief.lengteTotExpliciet=i-actief.plaatsInHooiberg;
            }
            else{//actief eindigt in expliciete knoop
                if (staartUpdate!=0){
                    staartUpdate->staart=actief.expliciet;
                    staartUpdate=0;
                }
            };
            //toevoegen blad
            unique_ptr<Sknoop> blad=make_unique<Blad>();
            blad->startp=actief.plaatsInHooiberg;
            blad->eindp=lengte;
            blad->startc=i;
            blad->eindc=lengte;
            assert(actief.expliciet);
            actief.expliciet->kind.set(hooiberg[i],move(blad));
            //verzet actief punt
            laatstestaart.verzet();
            laatstestaart.maakCanoniek(i,hooiberg);
            actief=laatstestaart;//oppervlakkige kopie
        };
        if (staartUpdate!=0){
                staartUpdate->staart=actief.expliciet;
                staartUpdate=0;
        }
    };
}






#endif

