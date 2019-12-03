#include <iostream>
#include "saisclass.h"
#include "LGP.h"

int main() {

    std::string data("hottentotten");
    SAIS suffixArray(reinterpret_cast<const unsigned char *>(data.c_str()), data.length());
    std::cout << "Suffix Array:" << std::endl;
    for(int val: suffixArray){
        std::cout << "\t" << data.substr(val,data.length()) << std::endl;

    }

    LGP lgp(suffixArray, data);

    return 0;


}