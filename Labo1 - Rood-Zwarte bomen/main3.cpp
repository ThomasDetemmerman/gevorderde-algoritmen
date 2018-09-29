#include "rzwboom14.h"
#include "vector"
#include "iostream"

using std::cout;
using std::vector;

int main()
{
    const vector<int> smallTree({10, 3, 15, 2, 6, 12, 18, 4, 8, 11, 13, 16, 20});
    RZWboom<int> zkbm;

    for (int i = 0; i < smallTree.size(); i++)
    {
        zkbm.voegtoe(smallTree[i]);
    }
    zkbm.tekenAlsBinaireBoom("smallTree.dot");

    cout << (zkbm.repOK() ? "Voldoet aan opgelegde voorwaarden\n" : "Voldoet niet aan opgelegd voorwaarden  (inorder + ptr naar ouder)\n");
    cout << "\noef 1: file generated\n";
    cout << "Please copy content of the created file here: http://webgraphviz.com\n";
}