#include <iostream>
#include "decoderingstrie.h"

int main() {
    decoderingstrie dt("../code.dat");
    dt.teken("out.dot");
    dt.decode("../TimErweten.cps");
    return 0;
};