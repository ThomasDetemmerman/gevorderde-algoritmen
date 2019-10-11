//
// Created by Thomas on 10/10/2019.
//

#include "dynamicProgramming.h"

int main(){
    const int aantalEieren = 30;
    const int aantalVerdiepen = 30;
    dynamicProgramming dp(aantalEieren, aantalVerdiepen);
    dp.print();
}