#include "knuthmorrispratt.h"
#include <string>
#include <iostream>
using namespace std;
int main(){
    string input = "abacabacaab";
    KnuthMorrisPratt kmp((const uchar*)input.c_str(),input.size());
    kmp.printfailureTable();
}