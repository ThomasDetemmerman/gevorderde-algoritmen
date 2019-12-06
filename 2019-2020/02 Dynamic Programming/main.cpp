#include <iostream>
#include <vector>
#include "DynamicTree.h"

using std::vector;

int G(int i, int j);

int main() {
    vector<int> data ={ 10, 20, 30, 40, 50, 60, 70};
    vector<int> freq ={ 10, 20, 30, 40, 50, 60, 70};//{ 99, 16, 66, 89, 33, 88, 90};

    DynamicTree dt(data, freq);
    std::cout << "Weights" << std::endl;
    dt.printWeights();
    std::cout << "Roots" << std::endl;
    dt.printRoots();

   dt.printTree();

    return 0;
}
