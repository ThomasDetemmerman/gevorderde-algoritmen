#include "shiftand.h"
#include <string>
#include <iostream>
#include <queue>

using std::queue;
using std::endl;
using std::cout;
using std::string;

int main(){

    // match
    string naald = "abba";
    string hooiberg = "ccccccc abba cc abba";
    queue<const uchar*> resultaten;

    Shiftand shiftand((const uchar*)naald.c_str(),naald.size());
    cout << shiftand.to_string() << endl;
    shiftand.zoek(resultaten, (const uchar*)hooiberg.c_str(),hooiberg.size());

    while(!resultaten.empty()){
        cout << "result found:" << resultaten.front() - naald.length() +1 << endl;
        // Als er alleen resultaten.front() zou staan dan printen we af vannaf het woord gedaan is. 
        resultaten.pop();
    }

}