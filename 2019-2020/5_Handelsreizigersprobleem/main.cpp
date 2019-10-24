#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "graaf.h"
#include "Handelsreizigersprobleem.h"

using namespace std;

int main(int argc, char *argv[]) {
    static string FILENAME = "/Users/thomasdetemmerman/Documents/Gevorderde-Algoritmen/2019-2020/5_Handelsreizigersprobleem/rss.txt";

    ifstream istrm(FILENAME);


    if (!istrm.is_open() || !istrm.is_open()) {
        cerr << "failed to open file: " << FILENAME;

    }

    Graaf <GERICHT> graaf(5000); //todo: this should not be hardcoded
    Graaf <GERICHT> omgekeerdeGraaf(5000);
    int source, dest;
    string a;

    while (istrm >> a >> source >> a >> a >> a >> a >> a >> dest) {

        try {
            graaf.voegVerbindingToe(source, dest);
            omgekeerdeGraaf.voegVerbindingToe(dest, source);
        }catch(GraafExceptie e){
            std::cout << "warning: " << e << std::endl;
        }
    }
    istrm.close();

    Handelsreizigersprobleem hrp(graaf, omgekeerdeGraaf);
    hrp.run();

}