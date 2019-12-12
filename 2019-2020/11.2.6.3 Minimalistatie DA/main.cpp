
#include <set>
#include "graaf.h"
#include "DAMinimizer.h"
using namespace std;


int main() {
    int SIZE = 5;
    int alfabetsize = 2;
    GraafMetTakdata<GERICHT, char> DA(SIZE);

    DA.voegVerbindingToe(0,1,'a');
    DA.voegVerbindingToe(0,2,'b');
    DA.voegVerbindingToe(1,1,'a');
    DA.voegVerbindingToe(2,2,'b');
    DA.voegVerbindingToe(2,1,'a');
    DA.voegVerbindingToe(1,3,'b');
    DA.voegVerbindingToe(3,1,'a');
    DA.voegVerbindingToe(3,4,'a');
    DA.voegVerbindingToe(4,1,'a');
    DA.voegVerbindingToe(4,2,'b');

    DA.teken("output.dot");

    DAMinimizer da(DA,SIZE,alfabetsize);
    da.minimize();
    da.printEquivalentieGroupen();

    return 0;
}