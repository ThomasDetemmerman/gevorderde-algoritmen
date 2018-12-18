#include "knuthmorrispratt.h"
#include <string>
#include <iostream>
using namespace std;
int main(){
    string interesstantevoorPrefix = "abcbabababab";
    string interesstantevoorPrefix2 = "abacabac";
    string YT = "acacabacacabacacac";
    string hooiberg = "thomas was op weg naar het sportpaleis dus maakte hij gebruik van zijn tomtom om er te geraken. Gelaas was de tomtom plat dus moet gij alsnog de weg vragen. Volgende keer de tomtom beter opladen thomas.";
   
    // Dit dient gewoon dat je niet elke variabele in onderstaand programma zou moeten wisselen. Je moet hieronder gewoon activeText gelijk stellen aan een van de teksten hierboven.
    string activeText = hooiberg;
    string activeSearchString = "tomtom";

    // failure table
    cout << "Builing failure table...." << endl;
    KnuthMorrisPratt kmp((const uchar*)activeSearchString.c_str(),activeSearchString.size());
    cout << "Failure table:" << endl;
    kmp.printfailureTable();

    cout << "Searching for "<<  activeSearchString << " in :" << endl << activeText << endl << endl;
    queue<const uchar*> resultaten;
    kmp.zoek(resultaten, (const uchar*)activeText.c_str(),activeText.size());

    cout << "results: " <<endl;
    while(!resultaten.empty()){
        cout << resultaten.front() << endl;
        resultaten.pop();
    }
}

/* Expected output YT:
    a       c       a       c       a       b       a       c       a       c       a       b       a       c       a       c       a       c
    0       0       1       2       3       0       1       2       3       4       5       6       7       8       9       10      11      4
*/