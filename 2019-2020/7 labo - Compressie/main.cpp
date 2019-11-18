#include <iostream>
#include "decoderingstrie.h"

int main() {
    decoderingstrie dt("../code.dat");
    //dt.teken("out.dot");
    dt.decode("../TimErweten.cps", "../resultaat.txt");

    dt.encodeAsEliasGammaCode("../eliasCode.dat");
    return 0;
};
<<<<<<< HEAD


=======
>>>>>>> 78332085c787a0ea5c261994fae14953a284577b
