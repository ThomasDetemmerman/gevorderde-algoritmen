#ifndef __STROOMNETOPGELOST_H
#define __STROOMNETOPGELOST_H
#include "graaf.h"
#include <cassert>
#include <iostream>
#include <functional>
#include <list>
#include <algorithm>

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
    Pad<T> geefVergrotendBFS();
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

    Stroomnetwerk<T> geefStroom();

    Stroomnetwerk<T>& operator-=(const Pad<T>& pad);
    Stroomnetwerk<T>& operator+=(const Pad<T>& pad);
    void vergrootTak(int start, int eind, T delta);
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
    Pad<T> vergrotendpad=vg.geefVergrotendBFS();//vg.geefVergrotendPad();
    while(vergrotendpad.size() !=0 ){
        restnetwerk-=vergrotendpad;
        oplossing+=vergrotendpad;
        vergrotendpad=vg.geefVergrotendBFS();//vg.geefVergrotendPad();
    }
    restnetwerk.teken("restnet.dot");
    return oplossing;
}

template <class T>
Stroomnetwerk<T>& Stroomnetwerk<T>::operator-=(const Pad<T>& pad){
    T padcapaciteit=pad.geefCapaciteit();
    for (int i=1; i<pad.size(); i++ ){
        int start=pad[i-1];//start en eind van de tak
        int eind=pad[i];
        int taknr=this->verbindingsnummer(start,eind);
        assert (taknr >= 0);
        assert(this->takdatavector[taknr]>=padcapaciteit);
        if (this->takdatavector[taknr]==padcapaciteit)
            this->verwijderVerbinding(start,eind);
        else
            this->takdatavector[taknr]-=padcapaciteit;
        vergrootTak(eind,start, padcapaciteit);
    }
}

template <class T>
Stroomnetwerk<T>& Stroomnetwerk<T>::operator+=(const Pad<T>& pad){
    T padcapaciteit=pad.geefCapaciteit();
    for (int i=1; i<pad.size(); i++ ){
        T nucapaciteit=padcapaciteit;
        int van=pad[i-1];
        int naar=pad[i];
        int terugtak=this->verbindingsnummer(naar,van);
        if (terugtak != -1){
            if (this->takdatavector[terugtak] <= nucapaciteit){
                nucapaciteit-=this->takdatavector[terugtak];
                this->verwijderVerbinding(naar,van);
                if (nucapaciteit > 0)
                    vergrootTak(van, naar, nucapaciteit);
            }else{
                this->takdatavector[terugtak]-=nucapaciteit;
            }
        }else
            vergrootTak(van, naar, padcapaciteit);
    }
}

template <class T>
void Stroomnetwerk<T>::vergrootTak(int start,int eind,T delta){
    int taknr=this->verbindingsnummer(start,eind);
    if (taknr==-1)
        taknr=this->voegVerbindingToe(start,eind,delta);
    else
        this->takdatavector[taknr]+=delta;
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

/*
 * Een Grootstecapaciteitszoeker. Deze geeft een vergrotend pad met maximale capaciteit.
 */
template <class T>
Pad<T>Vergrotendpadzoeker<T>::geefVergrotendBFS(){

    int size = this->q.aantalKnopen();
    vector<bool> visited(size, false);
    vector<int> voorhanger(size);
    std::list<int> queue;
    int laatstBezochte;

    visited[0] = true;
    queue.push_back(0); //v is start (v2 is consumpent)


    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        int s = queue.front();
        voorhanger[0] = -1;
        queue.pop_front();

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        std::cout << "behandelen van " << s << std::endl;
        for(auto buur: this->q.operator[](s)){
            if(!visited[buur.first]){
                visited[buur.first] = true;
                queue.push_back(buur.first);
                voorhanger[buur.first] = s;
                laatstBezochte = buur.first;
                std::cout << "buur " << buur.first << " heeft als ouder " << s << std::endl;
            }
        }
    }
    Pad<T> p;
    p.zetCapaciteit(INT_MAX);
    int i = 4; //todo: vervang door v2 wat destination zou moeten zijn
    while(i != -1){
        p.emplace_back(i);
        if(voorhanger[i] != -1){
            p.zetCapaciteit(std::min(p.geefCapaciteit(),*(this->q.geefTakdata(voorhanger[i], i))));
        }
        i = voorhanger[i];
    }
    std::reverse(p.begin(), p.end());
    return p;
}
/*
 * bijkomende opgave: Een Kortstepadzoeker. Deze geeft een vergrotend pad terug dat zo kort mogelijk is.
 */

#endif
