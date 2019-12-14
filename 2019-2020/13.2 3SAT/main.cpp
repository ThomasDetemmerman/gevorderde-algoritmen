#include <iostream>
#include <vector>
#include <map>
#include "graaf.h"
#include <set>
using namespace std;

string toString(int num, bool neg){
    string s = "xx" + std::to_string(num);
    return neg? s+="-" : s+="+";
}

int main() {
    // gegeven
    map<string,vector<string>> gegeven = {{ "F1", { "xx1+","xx2-","xx3+"}}, {"F2",{"xx4-","xx2+","xx6+"}}};

    map<string,int> translator;
    set<int> nummers;

    int counter= 0;
    GraafMetKnoopdata<GERICHT,string> gg;
    for(const auto [key,  val]: gegeven){
        nummers.insert(val[0][2]-'0');
        nummers.insert(val[1][2]-'0');
        nummers.insert(val[2][2]-'0');

        int  a = gg.voegKnoopToe(key+"_"+val[0]);
        int  b = gg.voegKnoopToe(key+"_"+val[1]);
        int  c = gg.voegKnoopToe(key+"_"+val[2]);

        translator[val[0]] = a;
        translator[val[1]] = b;
        translator[val[2]] = c;

        gg.voegVerbindingToe(a,b);
        gg.voegVerbindingToe(b,c);
        gg.voegVerbindingToe(c,a);
    }

    for (int i: nummers) {
        string  atoomA = "xx"+ to_string(i)+"-";
        string atoomB = "xx"+ to_string(i)+"+";

        int  a = gg.voegKnoopToe(atoomA);
        int  b = gg.voegKnoopToe(atoomB);

        gg.voegVerbindingToe(a,b);

        if( translator.find(atoomA) != translator.end()){
            gg.voegVerbindingToe(a, translator[atoomA]);
        }

        if( translator.find(atoomB) != translator.end()){
            gg.voegVerbindingToe(b, translator[atoomB]);
        }

    }
    gg.teken("output.dot");


    return 0;
}