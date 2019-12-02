#ifndef __WOORDSTROOM_H
#define __WOORDSTROOM_H
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::string;

/**********************************************************************

   Class: Woordstroom
   
   beschrijving: Een Woordstroom beheert een bestand (naam opgegeven in de constructor).
   Met de >>-operator lees je strings in: afwisselend krijg je een woord en een string met
   karakters tussen twee opeenvolgende woorden.
   
***************************************************************************/

class Woordstroom: protected ifstream{
public:
    //constructor opent het bestand.
    Woordstroom(const char* c):ifstream(c){laatstgelezen=get();};
    bool fail(){return ifstream::fail();}
    friend Woordstroom& operator>>(Woordstroom& ws, string& str){
        str.clear();
        str+=ws.laatstgelezen;
        char c=ws.get();
        while (!ws.fail() && (bool)isalpha(ws.laatstgelezen)==(bool)isalpha(c)){
            str+=c;
            c=ws.get();
        };
        ws.laatstgelezen=c;
        return ws;
    };
protected:
    char laatstgelezen;
};

#endif

