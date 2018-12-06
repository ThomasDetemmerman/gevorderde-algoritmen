#include "knuthmorrispratt.h"
#include <string>
#include <iostream>
using namespace std;
int main(){
    string interesstantevoorPrefix = "abcbabababab";
    string interesstantevoorPrefix2 = "abacabac";
    KnuthMorrisPratt kmp((const uchar*)interesstantevoorPrefix.c_str(),interesstantevoorPrefix.size());
    kmp.printfailureTable();
}