#ifndef __STROOMNETOPGELOST_H
#define __STROOMNETOPGELOST_H
#include "graaf.h"
#include <cassert>
#include <iostream>
#include <functional>
using std::vector;
using std::function;
using std::ostream;
using std::cerr;
template< class T>
class Stroomnetwerk;


template< class T >
class Pad:public std::vector< int >{
public:
    T geefCapaciteit() const;
    void zetCapaciteit(const T& _capaciteit);
protected:
    T capaciteit;
};
template< class T >
T Pad<T>::geefCapaciteit() const{
    return capaciteit;
}

template< class T >
void Pad<T>::zetCapaciteit(const T& _capaciteit){
    capaciteit=_capaciteit;
}

template< class T >
ostream& operator<<(ostream& os,const Pad<T>& p){
    os<<"Capaciteit= "<<p.geefCapaciteit()<<" :: ";
    if (p.size() > 0){
        os<<p[0];
    }
    for (int i=1; i<p.size(); i++ ){
        os<<"->"<<p[i];
    }
    os<<"\n";
}

/**********************************************************************

   Class:Vergrotendpadzoeker
   
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
Stroomnetwerk(int grootte, int _van, int _naar);
//Copyconstructor. Let op: Graaf<GERICHT>(gr) moet toegevoegd,
//anders roept de copyconstructor van GraafMetTakdata de defaultconstructor
//van Graaf op en krijgen we een lege graaf.
//Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr):
//                    Graaf<GERICHT>(gr),GraafMetTakdata<GERICHT, T>(gr),van(0),naar(1){};
Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr, int _van, int _naar);
Stroomnetwerk(const Stroomnetwerk<T>& gr);
Stroomnetwerk(Stroomnetwerk<T>&& gr);
Stroomnetwerk<T>& operator+=(const Pad<T> & vergrotendpad);
Stroomnetwerk<T>& operator-=(const Pad<T> & vergrotendpad);
void updateVerbinding(int van, int naar, T stroom);
Stroomnetwerk<T> geefStroom();
T geefCapaciteit();

    int van,naar;
virtual void teken(const char* bestandsnaam) const;
protected:
    virtual std::string knooplabel(int i) const;

};
template <class T>
Stroomnetwerk<T>::Stroomnetwerk(int grootte,int _van,int _naar):
                    Graaf<GERICHT>(grootte),GraafMetTakdata<GERICHT, T>(grootte),van(_van),naar(_naar){}

template <class T>
Stroomnetwerk<T>::Stroomnetwerk(const GraafMetTakdata<GERICHT ,T>& gr,int _van,int _naar):
                    Graaf<GERICHT>(gr),GraafMetTakdata<GERICHT, T>(gr),van(_van),naar(_naar){}

template <class T>
Stroomnetwerk<T>::Stroomnetwerk(const Stroomnetwerk<T>& gr):
                    Graaf<GERICHT>(gr),GraafMetTakdata<GERICHT, T>(gr),van(gr.van),naar(gr.naar){}

template <class T>
Stroomnetwerk<T>::Stroomnetwerk(Stroomnetwerk<T>&& gr):Stroomnetwerk(0,0,0){
    swap(this->burenlijsten,gr.burenlijsten);
    swap(this->vrijgekomenVerbindingsnummers,gr.vrijgekomenVerbindingsnummers);
    std::swap(this->hoogsteVerbindingsnummer,gr.hoogsteVerbindingsnummer);
    swap(this->takdatavector,gr.takdatavector);
    std::swap(this->van,gr.van);
    std::swap(this->naar,gr.naar);
}

template <class T>
Stroomnetwerk<T> Stroomnetwerk<T>::geefStroom(){
    Stroomnetwerk<T> oplossing(this->aantalKnopen(),van,naar);
    Stroomnetwerk<T> restnetwerk(*this);
    Vergrotendpadzoeker<T> vg(restnetwerk);
    Pad<T> vergrotendpad=vg.geefVergrotendPad();
    while(vergrotendpad.size() !=0 ){
        restnetwerk-=vergrotendpad;
        oplossing+=vergrotendpad;
        vergrotendpad=vg.geefVergrotendPad();
    }
    restnetwerk.teken("restnet.dot");
    return oplossing;
}
template <class T>
T Stroomnetwerk<T>::geefCapaciteit(){
    T som=0;
    for (typename GraafMetTakdata<GERICHT,T>::Burenlijst::const_iterator it=this->burenlijsten[van].begin();
                it!=this->burenlijsten[van].end();it++)
        som+=this->takdatavector[it->second];
    return som;
}

template <class T>
void Stroomnetwerk<T>::teken(const char* bestandsnaam) const{
    std::ofstream uit(bestandsnaam);
    assert(uit);
    std::string pijl;
        uit<<"digraph {\n";
        pijl="->";
    uit<<"{ rank=source; "<<knooplabel(van)<<" }\n";
    uit<<"{ rank=sink; "<<knooplabel(naar)<<" }\n";
    for (int k=0; k<this->aantalKnopen(); k++){
        if (this->burenlijsten[k].empty())
            uit<<knooplabel(k)<<";\n";
        else{
        for (auto& p: this->burenlijsten[k])
                uit<<knooplabel(k)<<" "<<pijl<<" "
                   <<knooplabel(p.first)<<this->taklabel(p.second)<<";\n";
        };
    };
    uit<<"}";
    uit.close();
}

template <class T>
std::string Stroomnetwerk<T>::knooplabel(int i) const{
    std::ostringstream uit;
    if (i==van)
        uit<<'"'<<i<<":P\"";
    else if (i==naar)
        uit<<'"'<<i<<":V\"";
    else
        uit<<i;
    return uit.str();
}

/** eigen aanvulling **/

template <class T>
Stroomnetwerk<T>& Stroomnetwerk<T>::operator+=(const Pad<T> & vergrotendpad){

    int vergrotendPadCapaciteit = vergrotendpad.geefCapaciteit();
    for(int i=1; i < vergrotendpad.size(); i++){
        int van = vergrotendpad[i];
        int naar = vergrotendpad[i-1];



        //eerst controleren of er al een terugverbinding is.
        if(this->verbindingsnummer(naar, van) == -1){
            int terugCapaciteit = *(this->geefTakdata(naar, van));

            // als de capaciteit van de terugverbinding groter is dan de capaciteit dat we aan de heencapaciteit zouden geven, trekken we deze gewoon af van elkaar
            if(terugCapaciteit > vergrotendPadCapaciteit){
                *(this->geefTakdata(naar, van)) -= vergrotendPadCapaciteit;
            }
            //indien de heencapaciteit groter is mogen we de terugcapaciteit gewoon verwijderen en wat nog overblijft toevoegen als een heencapaciteit
            else {
                this->verwijderVerbinding(naar, van);
                // als terugcapaciteit == heencapaciteit moeten we de terugverbinding gewoon verwijderen. In het andere geval moet onderstaande nog gebeuren
                if(vergrotendPadCapaciteit > terugCapaciteit){
                    this->voegVerbindingToe(van, naar, (vergrotendPadCapaciteit - terugCapaciteit));
                }

            }

        } else {
           updateVerbinding(van, naar,vergrotendPadCapaciteit );

        }
    }
    return *this;

}


template <class T>
Stroomnetwerk<T>& Stroomnetwerk<T>::operator-=(const Pad<T> & vergrotendpad) {
    T vergrotendPadCapaciteit = vergrotendpad.geefCapaciteit();
    for(int i=1; i < vergrotendpad.size(); i++){
        int van = vergrotendpad[i];
        int naar = vergrotendpad[i-1];

        this->teken("error.dot");
        std::cout << "Trying to get connection between:" << van << " -> " << naar << std::endl;

        //assert(!(this->verbindingsnummer(van, naar) == -1));
        if(this->verbindingsnummer(van, naar) == -1){
            this->teken("error.dot");
            std::cout << "Trying to get connection between:" << van << " -> " << naar << std::endl;

            exit(-1);
        }


        updateVerbinding(van, naar, -vergrotendPadCapaciteit);
        updateVerbinding(naar, van, vergrotendPadCapaciteit);

    }

}

template <class T>
void Stroomnetwerk<T>::updateVerbinding(int van, int naar, T stroom){
    if(this->verbindingsnummer(van, naar) == -1){
        this->voegVerbindingToe(van, naar, stroom);
    } else {
        *(this->geefTakdata(van, naar)) += stroom;
    }
    if( *(this->geefTakdata(van, naar)) == 0){
        this->verwijderVerbinding(van, naar);
    }
}


#endif
