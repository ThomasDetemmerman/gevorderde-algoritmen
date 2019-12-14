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
    vector<int> preprocess_strong_suffix();

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
    vector<int> shift = preprocess_strong_suffix();
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
            int verschuiven_H2 = naald.length()-1 - MRP[hooiberg[hooibergIndex] + 128];
            int aantalOpschuiven = max(verschuiven_H1,verschuiven_H2);

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
// kijk naar martijn zijn oplossing
// https://www.geeksforgeeks.org/boyer-moore-algorithm-good-suffix-heuristic/

vector<int> BoyerMoore::preprocess_strong_suffix(){
    vector<int> shift(naald.size()+1), borderpositions(naald.size()+1);
    int i=naald.size(), j=naald.size()+1;
    borderpositions[i]=j;

    while(i>0)
    {
        /*if character at position i-1 is not equivalent to
          character at j-1, then continue searching to right
          of the pattern for border */
        while(j<=naald.size() && naald[i-1] != naald[j-1])
        {
            /* the character preceding the occurrence of t in
               pattern P is different than the mismatching character in P,
               we stop skipping the occurrences and shift the pattern
               from i to j */
            if (shift[j]==0)
                shift[j] = j-i;

            //Update the position of next border
            j = borderpositions[j];
        }
        /* p[i-1] matched with p[j-1], border is found.
           store the  beginning position of border */
        i--;j--;
        borderpositions[i] = j;
    }
    return  shift;
}



#endif
    
