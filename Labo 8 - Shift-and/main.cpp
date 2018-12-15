#include "shiftand.h"
#include <string>
#include <iostream>
#include <queue>

using std::queue;
using std::endl;
using std::cout;
using std::string;

int main(){

    string naald = "tomtom";
    string hooiberg = "thomas was op weg naar het sportpaleis dus maakte hij gebruik van zijn tomtom om er te geraken. Gelaas was de tomtom plat dus moet gij alsnog de weg vragen. Volgende keer de tomtom beter opladen thomas.";
    queue<const uchar*> resultaten;

    Shiftand shiftand((const uchar*)naald.c_str(),naald.size());
    //cout << shiftand.to_string();
    shiftand.zoek(resultaten, (const uchar*)hooiberg.c_str(),hooiberg.size());

    while(!resultaten.empty()){
        cout << resultaten.front() << endl;
        resultaten.pop();
    }
}