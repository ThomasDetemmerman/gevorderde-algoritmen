#ifndef __BOYERMOORE_H
#define __BOYERMOORE_H

#include <queue>
#include <map>
#include <string>
#include <iostream>

typedef unsigned int uint;
typedef unsigned char uchar;
using namespace std;

class BoyerMoore {
public:
    BoyerMoore(const string &_naald);

//de zoekfunctie geeft in teller het aantal karaktervergelijkingen terug
    std::queue<int> zoek(const string &hooiberg, int teller);

    void printMRP();

private:

    const string naald;
    //een character loopt van -127 tot 128
    const int ALFABET_SIZE = abs(-127) + 128;

    vector<int> calculateMRP();
    vector<int> preprocessWeakShiftTable();

    void print(string naald, int naaldindex, string hooiber, int hooibergindex);
};


BoyerMoore::BoyerMoore(const string &_naald) : naald(_naald) {
    map<char, int> MRPLookup;

}

void BoyerMoore::printMRP() {
    vector<int>  MRP = calculateMRP();
    for (char i: naald) {
        std::cout << i << "\t";
    }
    std::cout << std::endl;
    for (char i: naald) {
        std::cout << MRP[i + 128] << "\t";
    }
    std::cout << std::endl;
}

std::queue<int> BoyerMoore::zoek(const string &hooiberg, int teller) {
    vector<int> MRP = calculateMRP();
    vector<int> shift = preprocessWeakShiftTable();
    std::queue<int> matches;
    int hooibergIndex_startpoint = naald.length() - 1;
    int hooibergIndex = hooibergIndex_startpoint;
    int naaldIndex = naald.length() - 1;

    while (hooibergIndex <= hooiberg.length()) {
        while (hooiberg[hooibergIndex] == naald[naaldIndex] && naaldIndex > 0) {
            hooibergIndex--;
            naaldIndex--;
        }

        if (naaldIndex == 0 && naald[naaldIndex] == hooiberg[hooibergIndex]) {
            matches.push(hooibergIndex);
            hooibergIndex_startpoint++;
            hooibergIndex = hooibergIndex_startpoint;
        } else {
            int verschuiven_H1 = naald.length()-1 - MRP[hooiberg[hooibergIndex] + 128];
            int verschuiven_H2 = naald.length()-1 - shift[hooiberg[hooibergIndex]];
            int aantalOpschuiven = max(max(verschuiven_H1,verschuiven_H2),1);

            //std::cout << "shifting over " << aantalOpschuiven << std::endl;
            //if (aantalOpschuiven <= 0) <-- dankzij de introductie van H2 zal de vershuiving altijd possitief zijn

            hooibergIndex_startpoint += aantalOpschuiven;
            hooibergIndex = hooibergIndex_startpoint;
        }
        naaldIndex = naald.length() - 1;
    }
return matches;
}
///////////////////////////////////////////////////////////////
////                    heuristieken                       ////
///////////////////////////////////////////////////////////////

/////////////////////
//// bad character //
/////////////////////
vector<int> BoyerMoore::calculateMRP() {
    vector<int> MRP(ALFABET_SIZE, -1);
    for (int i = 0; i < naald.length(); ++i) {
        MRP[naald[i] + 128] = i;
    }
    return MRP;
}

/////////////////////
//// good suffix  //
////////////////////
// Credits @ Martijn
// Deze oplossing is in O(p^2) en kan dus efficienter O(p)
// Deze oplossing kan nog beter door er een strong shift table van te maken
//  O(p^2) good suffix heuristic
//
//      i                   |   0   1   2   3   4   5   6
//      p-i-1               |   6   5   4   3   2   1   0
//      --------------------+-------------------------------
//      p[i]                |   a   b   b   a   b   a   b
//      --------------------+-------------------------------
//      suffix[i]           |   2   1   3   2   1   0   0
//      k                   |   /   /   /   2   3   4   6
//      i+1-k               |   /   /   /   2   2   2   1
//      verschuiving[i]     |   5   5   5   2   2   2   1
//                             ~~~~~~~~~~~             ~~~
//                                  |                   |
//                                  |                   V
//                                  |
//                                  |         Er is geen juiste suffix
//                                  V
//
//              Het juiste suffix komt niet meer in P voor:
//              p - s[0] = 7 - 2 = 5
//
vector<int> BoyerMoore::preprocessWeakShiftTable(){

    //PART 1: build suffix[i] (zie tekening hierboven)
    vector<int> commonSuffix(naald.size());
    int commonSuffixLenght = 0;
    for (int i = commonSuffix.size()-2; i >= 0 ; i--) {
        while(commonSuffixLenght > 0 && naald[i] != naald[naald.size()-1-commonSuffixLenght]){
            commonSuffixLenght = commonSuffix[naald.size()-commonSuffixLenght]; //geen -1
        }

        if(naald[i] == naald[naald.size()-1-commonSuffixLenght]){
            commonSuffixLenght++;
        }
        commonSuffix[i] = commonSuffixLenght;
    }

    //PART 2: build shift table (zie rij 'verschuiving' hierboven)
    int defaultValue = naald.size() - commonSuffix[0]; // Dit is die 5 in het voorbeeld hierboven (Het juiste suffix komt niet meer in P voor)
    vector<int> shift(naald.size(), defaultValue);
    for(int i=0; i < shift.size(); i++){
        int startIndex = naald.size() - 1 - commonSuffix[i];
        shift[startIndex] = (startIndex + 1 - i);
    }

    return  shift;
}


#endif
    
