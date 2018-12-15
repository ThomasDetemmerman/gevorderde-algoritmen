#include "knuthmorrispratt.h"
#include <string>
#include <iostream>
using namespace std;
int main(){
    string interesstantevoorPrefix = "abcbabababab";
    string interesstantevoorPrefix2 = "abacabac";
    string YT = "acacabacacabacacac";

    KnuthMorrisPratt kmp((const uchar*)YT.c_str(),YT.size());
    kmp.printfailureTable();
}

/* Expected output YT:
    a       c       a       c       a       b       a       c       a       c       a       b       a       c       a       c       a       c
    0       0       1       2       3       0       1       2       3       4       5       6       7       8       9       10      11      4
*/