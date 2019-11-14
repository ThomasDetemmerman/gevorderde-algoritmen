#ifndef __VARBITPATROON
#define __VARBITPATROON
#include <iostream>
#include <vector>
#include "bitstream.h"
using std::ostream;
using std::istream;
using std::vector;
using std::string;
typedef unsigned int uint;
const int uintlengte=sizeof(uint)*8;
/**********************************************************************

   Class:Varbitpatroon
   
   beschrijving: een klasse van bitpatronen met variabele lengte
   
***************************************************************************/

class Varbitpatroon{
public:
    Varbitpatroon():lengte(0){};
    uint geeflengte() const;
    bool geefbit(uint i) const;
    //voegToe voegt een bit achteraan toe;
    void voegToe(bool bit);
    void schrijf(obitstream& ob);
    const Varbitpatroon& operator=(const Varbitpatroon& vb);
    friend ostream& operator<<(ostream& os, const Varbitpatroon& b);
    friend istream& operator>>(istream& is, Varbitpatroon& b);

private:
    vector<uint> uints;
    uint lengte;//aantal ingevulde bits
};

uint Varbitpatroon::geeflengte() const{
    return lengte;
}

bool Varbitpatroon::geefbit(uint i) const{
    if (i>=lengte)
        return 0;
    else{
        return ( (uints[i/uintlengte] &
                 (uint(1)<<(uintlengte-1-(i%uintlengte)))) != 0);
    }
}
void Varbitpatroon::voegToe(bool bit){
    if (lengte==uints.size()*uintlengte)
        uints.push_back(0);
    ++lengte;
    uint bij=uint(1)<<(uintlengte-(lengte % uintlengte));
    if (bit)
        uints[(lengte-1)/uintlengte]|=bij;
    else
        uints[(lengte-1)/uintlengte]&=~bij;
}

void Varbitpatroon::schrijf(obitstream& ob){
    for (int i=0;i<lengte;i++){
        ob.schrijfbit(geefbit(i));
    }
}

const Varbitpatroon& Varbitpatroon::operator=(const Varbitpatroon& vb){
    uints=vb.uints;
    lengte=vb.lengte;
    return *this;
}

ostream& operator<<(ostream& os, const Varbitpatroon& b){
    for (int i=0;i<b.lengte;i++){
        os<<b.geefbit(i);
    }
    return os;
}

istream& operator>>(istream& is, Varbitpatroon& b){
    string nuleen;
    is>>nuleen;
    b.lengte=0;
    for (int i=0;i<nuleen.size();i++)
        b.voegToe(nuleen[i]=='1');
    return is;
}
#endif
