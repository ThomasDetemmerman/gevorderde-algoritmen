#ifndef __STROOMNET_H
#define __STROOMNET_H
#include "graaf.h"
#include <cassert>
#include <iostream>
#include <functional>
using std::vector;
using std::function;
using std::ostream;
using std::cout;

template< class T>
class Stroomnetwerk;


template< class T >
class Pad:public std::vector< int >{
public:
    T geefCapaciteit() const{
        return capaciteit;
    }
    void zetCapaciteit(const T& _capaciteit){
        capaciteit=_capaciteit;
    }
friend ostream& operator<<(ostream& os, const Pad& p){
    os<<"Capaciteit= "<<p.capaciteit<<" :: ";
    if (p.size() > 0){
        os<<p[0];
    }
    for (int i=1; i<p.size(); i++ ){
        os<<"->"<<p[i];
    }
    os<<"\n";
}
protected:
    T capaciteit;
};
/**********************************************************************

   Class:VergrotendPad
   
   beschrijving: Methodeklasse die, gegeven een stroomnetwerk,
                 een vergrotend pad teruggeeft.
                 Vermits ze hoort bij Ford-Fulkerson zal een object van deze klasse
                 elke keer een ander pad geven als het stroomnetwerk opgegeven
                 in de constructor verandert.

   
***************************************************************************/
template <class T>
class Vergrotendpadzoeker{
public:
    Vergrotendpadzoeker(const Stroomnetwerk<T>& stroomnetwerk):
                    q(stroomnetwerk),v(stroomnetwerk.van),v2(stroomnetwerk.naar),
                    l(q.aantalKnopen()), m(q.aantalKnopen(),false){};
    Pad<T> geefVergrotendPad();
protected:
    virtual void foo(int t, int x,Pad<T>& p);
    const Stroomnetwerk<T>& q;
    vector<int> l;
    vector<bool> m;
    int v,v2;
};


template <class T>
Pad<T>Vergrotendpadzoeker<T>::geefVergrotendPad(){
    for (int i=0; i<m.size(); i++ ){
        m[i]=false;
    }
    Pad<T> p;
    assert(v != v2);
    foo(v,0,p);
    assert(p.size()!=1);
    if (p.size() > 1){
        T g=*q.geefTakdata(p[0],p[1]);
        for (int i=2; i<p.size(); i++ ){
            T ychg=*q.geefTakdata(p[i-1],p[i]);
            if (ychg<g)
                g=ychg;
        }
        p.zetCapaciteit(g);
    }
    return p;
}


template <class T>
void Vergrotendpadzoeker<T>::foo(int t,int x, Pad<T>& p){
    m[t]=true;
    const typename GraafMetTakdata<GERICHT,T>::Burenlijst& a=q[t];
    int ychx=x+1;
    for (typename GraafMetTakdata<GERICHT,T>::Burenlijst::const_iterator it=a.begin();
                it!=a.end();it++){
        int u=it->first;
        if (*q.geefTakdata(t,u)> 0){
            if (it->first==v2 && ychx+1 > p.size()){
                l[v2]=t;
                p.resize(ychx+1);
                int ychf=v2;
                int i=ychx;
                while (ychf!=v){
                    p[i--]=ychf;
                    ychf=l[ychf];
                }
                p[0]=ychf;
            }
            else if(u!=v2 && !m[u]){
                l[u]=t;
                foo(u,ychx,p);
            }
        }
    }
}


/**********************************************************************

   Class: Stroomnetwerk
   
   beschrijving: Een stroomnetwerk gaat uit van een gewogen gerichte graaf
                 die in de constructor wordt opgegeven
   
***************************************************************************/

template <class T>//T = takdata
class Stroomnetwerk:public GraafMetTakdata<GERICHT, T >{
public:
//leeg netwerk; alleen aantal knopen en van en naar gegeven.
Stroomnetwerk(int grootte, int _van, int _naar):
                    Graaf<GERICHT>(grootte),GraafMetTakdata<GERICHT, T>(grootte),van(_van),naar(_naar){};
//Copyconstructor. Let op: Graaf<GERICHT>(gr) moet toegevoegd,
//anders roept de copyconstructor van GraafMetTakdata de defaultconstructor
//van Graaf op en krijgen we een lege graaf.
Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr, int _van, int _naar):
                    Graaf<GERICHT>(gr),GraafMetTakdata<GERICHT, T>(gr),van(_van),naar(_naar){};
Stroomnetwerk(const Stroomnetwerk<T>& gr):
                    Graaf<GERICHT>(gr),GraafMetTakdata<GERICHT, T>(gr),van(gr.van),naar(gr.naar){};
Stroomnetwerk(Stroomnetwerk<T>&& gr):Stroomnetwerk(0,0,0){
    swap(this->burenlijsten,gr.burenlijsten);
    swap(this->vrijgekomenVerbindingsnummers,gr.vrijgekomenVerbindingsnummers);
    std::swap(this->hoogsteVerbindingsnummer,gr.hoogsteVerbindingsnummer);
    swap(this->takdatavector,gr.takdatavector);
    std::swap(this->van,gr.van);
    std::swap(this->naar,gr.naar);
};

//Eigen aanvulling
//denk zonder &.

//Stroomnetwerk<T> operator+=(const Pad<T> pad){};
//Stroomnetwerk<T> operator-=(const Pad<T> pad){}
Stroomnetwerk<T> addVergrotendpad(Stroomnetwerk<T>* restnetwork, const Pad<T> pad){
 if (pad.empty())
    {
        return *this;
    }
    for(int i=1; i < pad.size();i++){
        int from = pad[i-1];
        int to = pad[i];
        int connectionID = this->verbindingsnummer(from,to); //methode van graaf.h
        //de connectie is nog niet toegevoegd geweest.
        if(connectionID == -1){
            this->voegVerbindingToe(from,to, pad.geefCapaciteit());
        }
         // als toe te voegen stromm > terugstroom -> toe te voegen stroom aftrekken van terugstroom 
         // als terustroom kleiner is dan toe te voegen strom: terustroom aftekken van toe te voegen stroom. Terugstroom op nul zetten en toevoegen aan graaf
        else {
            T terugstroom = restnetwork->geefTakdata(to,from);
            //als toe te voegen stromm > terugstroom
            if(this->geefCapaciteit() < pad.capaciteit){
                // toe te voegen stroom aftrekken van terugstroom 
                //to do: efficienter maken door effectieve update?
                this->verwijderVerbindingUitDatastructuur(from,to);
                this->voegVerbindingToe(from,to, pad.geefCapaciteit());
            } else { //als terustroom kleiner is dan toe te voegen stroom
               //Terugstroom op nul zetten en toevoegen aan graaf
                pad.capaciteit = 0;
                
            }
        }
    }
}

  return *this;

//Einde eigen aanvulling

Stroomnetwerk<T> geefStroom(){
    Stroomnetwerk<T> oplossing(this->aantalKnopen(),van,naar);
    Stroomnetwerk<T> restnetwerk(*this);
    Vergrotendpadzoeker<T> vg(restnetwerk);
    Pad<T> vergrotendpad=vg.geefVergrotendPad();
    while(vergrotendpad.size() !=0 ){
       // restnetwerk-=vergrotendpad;
        //oplossing+=vergrotendpad;
        oplossing.addVergrotendpad(restnetwerk, vergrotendpad);
        vergrotendpad=vg.geefVergrotendPad();
    }
    return oplossing;
}

void vergrootTak(int start, int eind, T delta){
        int taknr=this->verbindingsnummer(start,eind);
        if (taknr==-1)
            taknr=this->voegVerbindingToe(start,eind,delta);
        else
            this->takdatavector[taknr]+=delta;
}
T geefCapaciteit(){
    T som=0;
    for (typename GraafMetTakdata<GERICHT,T>::Burenlijst::const_iterator it=this->burenlijsten[van].begin();
                it!=this->burenlijsten[van].end();it++)
        som+=this->takdatavector[it->second];
    return som;
}

    int van,naar;
protected:
    virtual std::string knooplabel(int i) const{
        std::ostringstream uit;
        if (i==van)
            uit<<'"'<<i<<":P\"";
        else if (i==naar)
            uit<<'"'<<i<<":V\"";
        else
            uit<<i;
        return uit.str();
    };
};
#endif
