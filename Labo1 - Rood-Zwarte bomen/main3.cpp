#include "rzwboom14.h"
#include "vector"
#include "iostream"

using std::cout;
using std::vector;

void printRepOK(RZWboom<int> & zkbm){
    cout << (zkbm.repOK() ? "Voldoet aan opgelegde voorwaarden\n" : "Voldoet niet aan opgelegd voorwaarden  (inorder + ptr naar ouder)\n");  
};

int main()
{
    const vector<int> smallTree({10, 3, 15, 2, 6, 12, 18, 4, 8, 11, 13, 16, 20});
    //const vector<int> smallTree({5,2,10,8,12,6,9});   //werkt enkel met een left-rotate
   
    RZWboom<int> zkbm;

    for (int i = 0; i < smallTree.size(); i++)
    {
        zkbm.voegtoe(smallTree[i]);
    }
   
    // Start
    zkbm.tekenAlsBinaireBoom("default_smallTree.dot");
    cout << "\nfile generated: default_smallTree.dot\n";
    printRepOK(zkbm);

    // roteer rechts
    cout << "\nRotating smallTree to the right\n";
    zkbm.roteer(false);
    zkbm.tekenAlsBinaireBoom("rotate_right_smallTree.dot");
    cout << "\nfile generated: rotate_right_smallTree.dot\n";
    printRepOK(zkbm);

    cout << "\nPlease copy content of the created file here: http://webgraphviz.com\n";
}