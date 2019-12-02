
#include "regexp11.h"
#include "thompsonna.h"
#include "zoeker.h"

using namespace std;


int main() {
    Regexp regex("trie|tree");
    Regexp regexReversed("trie|tree");
    regexReversed.keerOm();

    ThompsonNA thompsonNa(regex);
    ThompsonNA thompsonNaReversed(regexReversed);

    std::ifstream file("../4algo.txt");
    std::string data;
    string red = "\033[0;31m";
    string noColor = "\033[0m";

    while (std::getline(file, data)) {
        queue<pair<int,int>> results = ZoekerNA(thompsonNa, thompsonNaReversed).zoek(data);
        while(!results.empty()){
            auto p =  results.front();
            data.insert(p.second+p.first+1, noColor);
            data.insert(p.first, red);
            cout << data << endl;

            results.pop();
        }


    }
    file.close();


    return 0;
}