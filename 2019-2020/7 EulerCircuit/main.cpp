#include <iostream>
#include "graaf.h"
#include<set>

using std::vector;
using std::set;


int vindCircuitRec(int startpunt, int startverbinding, Graaf<GERICHT> graaf){

    int nextBuurID;
    for(auto buur: graaf.operator[](startpunt)){
        if(buur.second == startverbinding){
            nextBuurID = buur.first;
            while(nextBuurID != startpunt){
                std::cout << nextBuurID << " >> ";
                nextBuurID =
            }

        }
    }
}

int main() {
    Graaf <GERICHT> graaf(8);
    // compontent A
    graaf.voegVerbindingToe(0,1);
    graaf.voegVerbindingToe(1,2);
    graaf.voegVerbindingToe(2,3);
    graaf.voegVerbindingToe(3,4);
    graaf.voegVerbindingToe(4,5);
    graaf.voegVerbindingToe(5,6);
    graaf.voegVerbindingToe(6,7);
    graaf.voegVerbindingToe(7,0);


    graaf.teken("output.dot");
    set<int> onafgewerkt;

    // teken circuit
    int startpunt = 0;
    int next = startpunt;
    do{
      auto buur = graaf.operator[](next);


       std::cout << next << " >> ";
    }while(startpunt != next);

    return 0;
}
