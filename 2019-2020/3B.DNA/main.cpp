#include <iostream>
#include <fstream>
#include "LGD.h"

using std::string;
using std::ifstream;
using std::cerr;
using std::endl;

int main(int argc, char *argv[]) {

    static string FILENAME_MENS = "/Users/thomasdetemmerman/Documents/Gevorderde-Algoritmen/2019-2020/3B.DNA/human.dna";
    static string FILENAME_AAP = "/Users/thomasdetemmerman/Documents/Gevorderde-Algoritmen/2019-2020/3B.DNA/chimp.dna";
    LGD lgd1;
    lgd1.calculateLGD("aap", "app");

    ifstream istrm_mens(FILENAME_MENS);
    ifstream istrm_aap(FILENAME_MENS);

    if (!istrm_mens.is_open() || !istrm_aap.is_open()) {
        cerr << "failed to open files";

    }
    string mens;
    string aap;
    vector<string> mensDNA;
    vector<string> aapDNA;
    LGD lgd;
    while (istrm_mens >> mens) {
        mensDNA.push_back(mens);

    }

    istrm_mens.close();
    while (istrm_aap >> aap) {
        aapDNA.push_back(aap);

    }
    istrm_aap.close();

    for (int menscounter = 0; menscounter < mensDNA.size(); menscounter++) {
        for (int aapcounter = 0; aapcounter < aapDNA.size(); aapcounter++) {
            std::cout << "Mens " << menscounter << " komt overeen met chimpansee " << aapcounter << " met afstand "
                      << lgd.calculateLGD(mensDNA.at(menscounter), aapDNA.at(aapcounter)) << endl;
        }
    }

}