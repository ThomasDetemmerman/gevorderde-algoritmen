#include "zoekboom17.h"
#include <vector>
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;
using std::vector;

int main()
{

    vector<int> keys = {50, 30, 100, 20, 40, 110, 10, 49, 105, 120, 3, 1, 4};
    vector<char> values = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'h', 'i', 'j'};

    vector<int> keys_small = {50, 30, 100, 20, 40};
    vector<char> value_small = {'a', 'b', 'c', 'd', 'k'};

    Zoekboom<int, char> zkbm;
    for (int i = 0; i < keys.size(); i++)
    {
        zkbm.voegtoe(keys[i], values[i]);
        zkbm.teken(("input"+std::to_string(i)+".dot").c_str());
    }



    // convert dot files to png.
    execl("./dot2png.sh", "./dot2png.sh", NULL);
}