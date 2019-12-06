#include "zoekboom17.h"
#include "splayboom.h"
#include <vector>
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;
using std::vector;


int main()
{

    Splayboom<int, string> splayboom;


   splayboom.voegtoe(50,"hi");
    splayboom.voegtoe(60,"hi");
    splayboom.voegtoe(30,"hi");
    splayboom.voegtoe(10,"hi");
    splayboom.voegtoe(80,"hi");
    splayboom.voegtoe(0,"hi");
    splayboom.voegtoe(20,"hi");
    splayboom.teken("1_input.dot");
    zoekKnoop<int, string> *a;
    Zoekboom<int, string> *b;
    int searchValue = 20;
    splayboom.zoek(searchValue,a, b);
    cout << "Looked up " << searchValue << ". Root is now " << splayboom->sleutel << endl;

    splayboom.teken("1_output2.dot");
    // convert dot files to png.
    execl("./dot2png.sh", "./dot2png.sh", NULL);

}