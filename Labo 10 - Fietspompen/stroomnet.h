#ifndef __STROOMNET_H
#define __STROOMNET_H
#include "graaf.h"
#include <cassert>
#include <iostream>
using std::vector;
using std::ostream;

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

   Class:Vergrotendpadzoeker
   
   beschrijving: Methodeklasse die, gegeven een stroomnetwerk,
                 en vergrotend pad teruggeeft.
                 van en naar zijn de knoopnummers van bron en doel.

   
***************************************************************************/
template <class T>
class Vergrotendpadzoeker{
public:
    Vergrotendpadzoeker(const GraafMetTakdata<GERICHT,T>& stroomnetwerk, int van, int naar, Pad<T>& pad);
protected:
    virtual void foo(int t, int x);
    const GraafMetTakdata<GERICHT,T>& q;
    Pad<T>& p;
    vector<int> l;
    vector<bool> m;
    int v,v2;
};


template <class T>
Vergrotendpadzoeker<T>::Vergrotendpadzoeker(const GraafMetTakdata<GERICHT ,T>& _q,int _v,int _v2,Pad<T>& _p):
                    p(_p), q(_q),v(_v),v2(_v2),
                    l(q.aantalKnopen()), m(q.aantalKnopen(),false){
    p.clear();
    assert(v != v2);
    foo(v,0);
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
}

template <class T>
void Vergrotendpadzoeker<T>::foo(int t,int x){
    m[t]=true;
    const typename GraafMetTakdata<GERICHT,T>::Knoop& a=q[t];
    int ychx=x+1;
    for (typename GraafMetTakdata<GERICHT,T>::Knoop::const_iterator it=a.begin();
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
                foo(u,ychx);
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
//Copyconstructor. Let op: Graaf<GERICHT>(gr) moet toegevoegd,
//anders roept de copyconstructor van GraafMetTakdata de defaultconstructor
//van Graaf op en krijgen we een lege graaf.
Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr):
                    Graaf<GERICHT>(gr),GraafMetTakdata<GERICHT, T>(gr){};


Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr, int _van, int _naar):
                    Graaf<GERICHT>(gr.aantalKnopen()),van(_van),naar(_naar){
    Stroomnetwerk<T> restnetwerk(gr);
    Pad<T> vergrotendpad;
    Vergrotendpadzoeker<T> vg(restnetwerk, van, naar, vergrotendpad);
    while(vergrotendpad.size() !=0 ){
        restnetwerk-=vergrotendpad;
        *this+=vergrotendpad;
        Vergrotendpadzoeker<T> vg(restnetwerk, van, naar, vergrotendpad);
    }
}
protected:
int van,naar;
};
#endif
