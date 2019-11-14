#ifndef __BINCODE
#define __BINCODE
#include <fstream>
#include "varbitpatroon.h"
using std::ofstream;
using std::ifstream;

/**********************************************************************

   Class: Bincode
   
   beschrijving: Een Bincode is een code voor een alfabet bestaande uit
    256 verschillende bytewaarden + een afsluitteken. Er zijn dus 257 codewoorden
    die worden opgeslagen in een tabel van Varbitpatronen.
   
***************************************************************************/

class Bincode{
public:
//constructor leest code in uit bestand
    Bincode(const string& bestandsnaam);
//geeft i-de codewoord terug
    const Varbitpatroon& operator[](int i) const;
    Bincode(){};
    void lees(const string& bestandsnaam);
    void schrijf(const string& bestandsnaam);
    static const int size = 257;
//private:
    Varbitpatroon code[size];
};
Bincode::Bincode(const string& bestandsnaam){
    lees(bestandsnaam);
}
const Varbitpatroon& Bincode::operator[](int i) const{
    return code[i];
}

void Bincode::lees(const string& bestandsnaam){
    ifstream in(bestandsnaam);
    for (int i=0;i<257;i++)
        in>>code[i];
    in.close();
}
void Bincode::schrijf(const string& bestandsnaam){
    ofstream uit(bestandsnaam);
    for (int i=0;i<257;i++)
        uit<<code[i]<<'\n';
    uit.close();
}

#endif
    
