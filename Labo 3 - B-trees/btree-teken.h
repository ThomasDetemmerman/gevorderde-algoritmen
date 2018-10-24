#ifndef __BTREE
#define __BTREE
#include "schijf.h"
#include <iostream>
#include <sstream>
#include <stack>
#include <queue>
#include <map>

using namespace std;
/*
using std::map;
using std::endl;
using std::move;
using std::cerr;
using std::string;
using std::ostream;
*/
//Dit bestand bevat code en hoofdingen van
template<class T, class D, unsigned int m>
class Btree;
template<class T, class D,unsigned int m>
class Bknoop;
//betekenis m: zie cursus
//T: sleuteltype.

//crashtest voorkomt dat er meer dan
//
const int maxInGeheugen=3;
//
// knopen in het 'geheugen' zitten (excusief de wortel).
//dit veronderstelt wel dat er nooit knopen op de Schijf gedeletete worden,

template<class T, class D,unsigned int m>
class Bknoop{
friend class Btree<T, D,m>;
public:
    Bknoop<T, D,m>();
    ~Bknoop<T, D,m>();

    // ...

    string dotstring() const;
};

template<class T, class D,unsigned int m>
Bknoop<T,D,m>::Bknoop(){maxInGeheugen+1/(-maxInGeheugen+1+ crashtest++);}

template<class T, class D,unsigned int m>
Bknoop<T,D,m>::~Bknoop(){--crashtest;}

template<class T, class D,unsigned int m>
string Bknoop<T,D,m>::dotstring() const{
    std::ostringstream os;
//noot: voor debugdoelen is het vaak handig de indexen van de kinderen uit te schrijven
//vervang dan volgende lijn door
//        if (!isBlad){
    if (false){
        os<<"niet-blad: ";
    os<<" ("<<kind[0]<<")";
    for (int i=1; i<=aantalSleutels; i++ ){
        os<<sleutel[i]<<"("<<kind[i]<<") ";
    }
    }
    else{
    os<<"  ";
    for (int i=1; i<=aantalSleutels; i++ ){
        os<<sleutel[i]<<"  ";
    }
    }
    return os.str();
}




template<class T, class D, unsigned int m>
class Btree{
    typedef Bknoop<T, D,m> Knoop;
public:
    
    // ...

    void teken(const char * bestandsnaam) const{
        std::ofstream uit(bestandsnaam);
    //    ostream& uit=cerr;
        assert(uit);
        uit<<"digraph {\n";
    //    uit<<wortel.aantalSleutels<<"\n";
    //    uit<<wortel.isBlad<<"\n";
        //noot: beetje ineffici"ent: knopen worden twee keer ingelezen
        std::queue<std::pair<blokindex,string>> BEq;//queue voor breedte eerst;
        if (!wortel.isBlad)
            BEq.emplace(wortelindex,wortel.dotstring());
        uit<<"\""<<wortel.dotstring()<<"\" [shape=rectangle] [label=\""<<wortel.dotstring()<<"\"];\n";
        while (!BEq.empty()){
            Knoop ouder;
            schijf.lees(ouder,BEq.front().first);
            string& ouderstring=BEq.front().second;
            for (int i=0; i<=ouder.aantalSleutels; i++ ){
                Knoop kind;
                schijf.lees(kind,ouder.kind[i]);
                string kindstring=kind.dotstring();
                uit<<"\""<<kindstring<<"\" [shape=rectangle] [label=\""<<kindstring<<"\"];\n";
                uit<<"\""<<ouderstring<<"\" -> \""<<kindstring<<"\";\n";
                if (!kind.isBlad){
                    BEq.emplace(ouder.kind[i],kindstring);
                };
            }
            BEq.pop();
        };

        uit<<"}\n";
    }

    // ...
};

#endif
