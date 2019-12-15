#include <iostream>
#include "woordstroom.h"
#include "boyermoore.h"
using namespace std;

int main() {

    string zoekterm1 = "totottot";
    BoyerMoore bm(zoekterm1);
    std::queue<int> matches = bm.zoek("taaltottaoaldlsqjfktotottotldsqjfldstotdlsfjdstotottototottottklfjzeototj", 0);
    cout << "Zoekterm " << zoekterm1 << " was found " << matches.size() << " times (eventueel met overlapping)" << endl << endl;


    string zoekterm = "Thomas";
    cout << zoekterm << " zoeken in de bijbel: " << endl;
    BoyerMoore zoeker(zoekterm);
    std::ifstream ifs("../bible.txt");
    //om de computer te sparen en niet heel de bijbel in te lezen, lezen we het bestand lijn per lijn in.
    std::string str;
    int linecounter = 0;
    while (std::getline(ifs, str)) {
        linecounter++;
        //cout << " line " << linecounter << endl;
        std::queue<int> matches = zoeker.zoek(str,0);
        while (!matches.empty())
        {
            int index = matches.front();
            std::cout << " match: " << str.substr(index, zoekterm.length()) << " at index " << index << " of line " << linecounter << endl;
            matches.pop();
        }
    }
    ifs.close();


    return 0;

}