#include <iostream>
#include "prquadtree.h"
int main() {
    PRQuadtree prq(1000);
    prq.voegToe(1,2);
    std::cout << prq.geefDiepte();
    return 0;
}