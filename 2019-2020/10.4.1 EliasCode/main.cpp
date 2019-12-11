#include <iostream>
#include "EliasGammeCode.h"

int main() {
    EliasGammeCode egc;
    egc.encode("../input.txt", "../output.txt");
    egc.decode("../output.txt", "../output2.txt");
    return 0;
}