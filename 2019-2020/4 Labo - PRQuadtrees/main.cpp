#include <iostream>
#include <cmath>

#include "prquadtreev2.h"
void main2();


int main() {
    PRQuadtree prq(100);
    prq.voegToe(0,0);
    prq.voegToe(0,100);
    prq.voegToe(100,0);
    prq.voegToe(20,30);
    std::cout << "diepte is: " << prq.geefDiepte();
    prq.teken("output.dot");

    main2();
    return 0;
}


/****** stress test ******/


int func(int i){
    return ( (10000-5*i)* cos(i*0.25*M_PI), (10000-5*i)* sin(i*0.25*M_PI) );
}

void main2() {
    std::cout << std::endl << std::endl << "Main 2" <<std::endl;
    const int size = 1000;
    PRQuadtree prq(size);

    for(int i=0; i < 5; i += 2){
        int a = abs(func(i));
        int b = abs(func(i+1));
        std::cout << "adding " << a << " by " << b << std::endl;
        prq.voegToe(a,b);
    }



    std::cout << "diepte is " << prq.geefDiepte() << std::endl;
    prq.teken("output2.dot");
}

