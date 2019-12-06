#include <iostream>
#include "saisclass.h"
#include "LGP.h"
#include "LCP.h"


int main() {
//hottentottententententoonstelling
    std::string data("hottentottententententoonstelling");
    SAIS suffixArray(reinterpret_cast<const unsigned char *>(data.c_str()), data.length());
    //LGP lgp(suffixArray, data);
    LCP lcp(suffixArray, data);
    std::cout << lcp << std::endl;


    //std::cout << lgp << std::endl;
    return 0;


}