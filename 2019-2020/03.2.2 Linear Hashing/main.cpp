#include <iostream>
#include "hashtabel.h"
int main() {
    hashtabel<int> ht;
    const int max = 30;
    for (int i = 0; i < 25; ++i) {
        ht.voegtoe(i);
    }

    std::cout << std::endl << "==================" << std::endl;
    int result;
    std::cout << "tests:" << std::endl;
    result =  ht.zoek(4);
    std::cout << "expected 4, got: " << result << std::endl;
    result = ht.zoek(max +4);
    std::cout << "expected -1, got " << result << std::endl;
    return 0;
}