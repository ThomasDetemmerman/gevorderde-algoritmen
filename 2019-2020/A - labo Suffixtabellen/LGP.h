//
// Created by Thomas on 03/12/2019.
//

#ifndef A___LABO_SUFFIXTABELLEN_LGP_H
#define A___LABO_SUFFIXTABELLEN_LGP_H

#include <vector>
#include <string>
#include "saisclass.h"

using std::vector;
using std::string;

class LGP:public vector<int> {
public:
    LGP(SAIS suffixArray, string data);
private:
    int getcommonPrefixLenght(string data1, string data2);
};

LGP::LGP(SAIS suffixArray, string data) {
    if(suffixArray.size() ==0){
        return;
    }
    int l = data.size();
    this->operator[](0) = 0;
    if(suffixArray.size() > 1){
        for(int i=1; i < suffixArray.size(); i++){
            string previousPrefix = data.substr(suffixArray[i-1],l);
            string currentPrefix = data.substr(suffixArray[i],l);
            this->operator[](i) = getcommonPrefixLenght(previousPrefix,currentPrefix);
        }
    }
}

int LGP::getcommonPrefixLenght(string data1, string data2) {
    int i = 0;
    while(data1[i] = data2[i]){
        i++;
    }
    return i;
}


#endif //A___LABO_SUFFIXTABELLEN_LGP_H
