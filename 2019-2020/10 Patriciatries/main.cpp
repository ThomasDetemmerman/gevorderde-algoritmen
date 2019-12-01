
#include "Patriciatrie.h"
#include <iostream>


int main() {
    Patriciatrie pt;
    string data;
    pt.voegToe("sleutelabc", "dataabc");
    pt.zoek("sleutelabc", data);
    cout << data << endl;
    pt.voegToe("sleutelbac", "databac");

    pt.voegToe("sleutelzac", "datazac");
    pt.voegToe("sleutelabd", "datazac");
    pt.teken("output.dot");

    string test = "sleutelanke";

    //int verschilIndex = pt.zoek(test, data);

    return 0;
}