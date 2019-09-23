#include "rzwboom14.h"
#include <vector>

using namespace std;

void maakBoom(const vector<tuple<int, int, char>> *input)
{
    for (tuple<int, int, char> item : *input)
    {
        cout << get<0>(item);
    }
}

int main()
{
    vector<tuple<int, int, char>> input = {
        make_tuple(0, NULL, 'Z'),
        make_tuple(-4, 0, 'Z'),
        make_tuple(-5, -4, 'Z'),
        make_tuple(-2, -5, 'Z'),
        make_tuple(30, 0, 'R'),
    };

    maakBoom(&input);
}