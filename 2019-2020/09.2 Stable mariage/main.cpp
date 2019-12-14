#include <iostream>
#include "voorkeurenlezer.h"
#include "stableMariage.h"

int main() {

    voorkeurenlezer voorkeurenlezer;
    vector<vector<Persoon>> iedereen = voorkeurenlezer.lees("../voorkeuren.txt");
    vector<Persoon> mannen = iedereen[0];
    vector<Persoon> vrouwen = iedereen[1];

    stableMariage sm(mannen, vrouwen);
    sm.solve();
    sm.printResults();

    return 0;
}