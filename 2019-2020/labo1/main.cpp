#include "zoekboom17.h"
#include <vector>
#include <iostream>
#include <unistd.h>

using std::vector;
using std::cout;
using std::endl;


int main()
{

    vector<int> keys =    {50,  30, 100,  20, 40,   110, 10,   49,  105, 120,  3,  1 ,   4};
    vector<char> values = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',  'j', 'h', 'i', 'j'};

    vector<int> keys_small =    {50, 30, 100, 20, 40};
    vector<char> value_small = {'a', 'b', 'c', 'd', 'k'};

    Zoekboom<int, char> zkbm;
    for (int i = 0; i < keys.size(); i++)
    {
        zkbm.voegtoe(keys[i], values[i]);
    }
    zkbm.teken("output_before.dot");
    cout << "Diepte van deze boom is " << zkbm.geefDiepte() << endl;
    zkbm.roteer(true);
    zkbm.maakOnevenwichtig();
    zkbm.teken("output_after.dot");

    // convert dot files to png.
    execl("./dot2png.sh", "./dot2png.sh", NULL);
}