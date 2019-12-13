#include <iostream>
#include "Knoop.h"
#include "suffixboom.h"

string getText();

int main() {
    string data = "blabltotabla";
    suffixboom sb(data);
    sb.teken("output.dot");
    string zoekterm = "tot";
    vector<int> resultaten = sb.zoek(zoekterm);
    for(auto resultaat: resultaten){
        std::cout << "Found " << zoekterm << "at index : " << resultaat << ". Got: " << data.substr(resultaat, zoekterm.size()) << std::endl;
    }
    return 0;
}
