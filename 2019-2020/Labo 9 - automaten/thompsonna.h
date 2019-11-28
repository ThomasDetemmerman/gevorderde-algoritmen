#ifndef __THOMPSONNA_H
#define __THOMPSONNA_H
#include "regexp11.h"
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
using std::vector;
const uchar epsilon=0;
class ZoekNA;
//ThompsonNA: met een beginstatenbit (index 0)
//en een eindstatenbit (index aantalStatenbits-1)
//Alleen gebruikt om algoritme ven Thompson te implementeren

class ThompsonNA{
//friend class ZoekNA;
public:
    ThompsonNA(const Regexp& re);
    void schrijf(ostream& os) const;
    class Verbinding{
    friend class ThompsonNA;
    public:
        Verbinding(int _bron,int _doel,uchar _a);                             ;
        int geefBron() const;
        int geefDoel() const;
        int geefKarakter() const;
    private:
        int bron;
        int doel;
        uchar a;
    };
    int geefAantalVerbindingen() const;
    int geefAantalStatenbits() const;
    const Verbinding& operator[](int i) const;

void teken(const char* bestandsnaam) const{
    std::ofstream uit(bestandsnaam);
    assert(uit);
    std::string pijl;
    pijl="->";
    uit<<"digraph {\n";
    uit<<"  rankdir=\"LR\";";
    uit<<"  -1 [shape=point];\n";
    uit<<"  -1 -> 0;\n";
    uit<<"  "<<geefAantalStatenbits()-1<<" [shape=square];\n";
    for (auto&& v:overgang){
         uit<<"  "<<v.geefBron()<<" "<<pijl<<" "<<v.geefDoel();
         uit<<"[label=\"";
         if (v.geefKarakter()==epsilon)
            uit<<"epsilon";
         else
            uit<<(char)v.geefKarakter();
         uit<<"\"];\n";
    };
    uit<<"}";
    uit.close();
};

protected:
//haalt de Verbindingen van TNA binnen, maar verschuift de knoopnrs;
    void haalbinnen(const ThompsonNA& TNA,int verschuiving);
    void verbind(int bron, int doel, uchar c);
    int aantalStatenbits;
    vector<Verbinding> overgang;
};


ThompsonNA::Verbinding::Verbinding(int _bron,int _doel,uchar _a):bron(_bron),doel(_doel),a(_a){}

int ThompsonNA::Verbinding::geefBron() const{
    return bron;
}

int ThompsonNA::Verbinding::geefDoel() const{
    return doel;
}

int ThompsonNA::Verbinding::geefKarakter() const{
    return a;
}


int ThompsonNA::geefAantalVerbindingen() const{
    return overgang.size();
}

int ThompsonNA::geefAantalStatenbits() const{
    return aantalStatenbits;
}

const ThompsonNA::Verbinding& ThompsonNA::operator[](int i) const{
    return overgang[i];
}


void ThompsonNA::schrijf(ostream& os) const{
    os<<"aantal statenbiten: "<<aantalStatenbits<<" aantal verbindingen: "<<geefAantalVerbindingen()<<'\n';
    for (int i=0; i<overgang.size() ;i++){
        os<<overgang[i].bron<<' '
          <<overgang[i].doel<<' ';
        if (overgang[i].a==epsilon)
            os<<"epsilon"<<'\n';
        else
            os<<overgang[i].a<<'\n';
    }
    os<<"--------\n";
}

void ThompsonNA::haalbinnen(const ThompsonNA& TNA,int verschuiving){
    for (int i=0; i<TNA.overgang.size() ;i++ ){
        const Verbinding& oud=TNA.overgang[i];
        overgang.push_back(Verbinding(oud.bron+verschuiving,
                oud.doel+verschuiving,oud.a));
    }
}
void ThompsonNA::verbind(int bron, int doel, uchar c=epsilon){
    overgang.push_back(Verbinding(bron,doel,c));
}


ThompsonNA::ThompsonNA(const Regexp& re){
    switch (re.geefOpcode()){
     case Regexp::letter:
         aantalStatenbits=2;
         verbind(0,1,re.geefLetter());
     break;
     case Regexp::plus:{
            ThompsonNA op1(*re.geefEersteOperand());
            ThompsonNA op2(*re.geefTweedeOperand());
            //beginknoop tweede operand overlapt met eindknoop eerste
            aantalStatenbits=op1.aantalStatenbits+op2.aantalStatenbits-1;
            haalbinnen(op1, 0);
            haalbinnen(op2, op1.aantalStatenbits-1);
        }
        break;
        case Regexp::of:{
            ThompsonNA op1(*re.geefEersteOperand());
            ThompsonNA op2(*re.geefTweedeOperand());
            aantalStatenbits=op1.aantalStatenbits+op2.aantalStatenbits+2;
            haalbinnen(op1, 1);
            haalbinnen(op2, 1+op1.aantalStatenbits);
            verbind(0, 1);
            verbind(0, op1.aantalStatenbits+1);
            verbind(op1.aantalStatenbits, aantalStatenbits-1);
            verbind(aantalStatenbits-2, aantalStatenbits-1);
        }
        break;
        case Regexp::ster:{
            ThompsonNA op1(*re.geefEersteOperand());
            aantalStatenbits=op1.aantalStatenbits+2;
            haalbinnen(op1, 1);
            verbind(0, 1);
            verbind(0, aantalStatenbits-1);
            verbind(aantalStatenbits-2, aantalStatenbits-1);
            verbind(aantalStatenbits-2, 1);
        }
    }
};

#endif

