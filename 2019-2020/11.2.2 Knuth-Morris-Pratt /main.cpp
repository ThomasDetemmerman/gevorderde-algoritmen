#include <iostream>
#include "prefix.h"
#include "KnuthMorrisPratt.h"

int main() {
    std::string  zoekstring= "ababab";
    std::string data = "jdkslfsdmabababdeffqmkdqmfabcdababablsqfjqsm";
    Prefix prefix(zoekstring);
    std::cout << "Prefix tabel:" << std::endl << prefix << std::endl << std::endl;

    KnuthMorrisPratt KMP(prefix);
    std::queue<int> resultaten = KMP.zoek(data);

    int beginIndex;
    while (!resultaten.empty())
    {
        beginIndex = resultaten.front();
        std::cout << "Result at index " << beginIndex << " : " << data.substr(beginIndex,zoekstring.size()) << std::endl;
        resultaten.pop();
    }

    return 0;
}