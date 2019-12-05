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
using std::ostream;

class LGP:public vector<int> {
public:
    LGP(SAIS suffixArray, string data);
    friend ostream& operator<<(ostream& os, const LGP& dt);
private:
    SAIS sa;
    string data;
    void primitieveManier(SAIS suffixArray, string data);
    void efficienteManier();
    //int geefIndexVanopvolger(int i);
    int getcommonPrefixLenght(string data1, string data2);
    int getcommonPrefixLenght(int startIndexA, int startIndexB, int startPunt);
    int opvolger(int i);
};

LGP::LGP(SAIS suffixArray, string data_) : data(data_), sa(suffixArray), vector<int>(suffixArray.size(),0) {
    //primitieveManier(suffixArray, data);
    efficienteManier();
}


////////////////////////////
//      efficiente manier //
////////////////////////////

// tijdscomplexiteit: O(t)
void LGP::efficienteManier() {
    //todo: werkt niet
    int k = 0;
    for (int i = 0; i < size(); i++) {
        if (opvolger(i) == this->size()-1) {
            k = 0;
            continue;
        }
        int j = opvolger(i+1);
       // while (i + k < size() && j + k < size() && data[i+k] == data[j+k])
         //   k++;
         k = getcommonPrefixLenght(i,j,k);
        this->operator[](opvolger(i)) = k;
        if (k){
            k--;
        }
    }
}

// cursus zegt: In SA zoeken we zijn opvolger. Dit
//houdt dus in dat we eerst j bepalen met SA[j] = i;
int LGP::getcommonPrefixLenght(int i, int j, int k) {
    while (i + k < size() && j + k < size() && data[i+k] == data[j+k]){
        k++;
    }
    return k;
}

int LGP::opvolger(int i) {
    for (int j = 0; j < this->size(); i++)
        if(sa[j] == i){
            return j;
        }
    return -1;
}


////////////////////////////
//      primitieve manier //
////////////////////////////

// tijdscomplexiteit: O(t^2)
void LGP::primitieveManier(SAIS suffixArray, string data){
    if(suffixArray.size() ==0){
        return;
    }
    if(suffixArray.size() > 1){
        for(int i=0; i < this->size()-1; i++){
            this->operator[](i) = getcommonPrefixLenght(data.substr(i),data.substr(i+1));
        }
    }
}

int LGP::getcommonPrefixLenght(string data1, string data2) {
    int i=0;
    while(data1[i] == data2[i]){
        i++;
    }
    return i;
}


std::ostream& operator<<(std::ostream& os, const LGP& dt)
{
    for (int i = 0; i < dt.size(); ++i) {
        os << i << "\t";
    }
    os << std::endl;
    for (int i = 0; i < dt.size(); ++i) {
        os << dt[i]<< "\t";
    }
    return os;
}



#endif //A___LABO_SUFFIXTABELLEN_LGP_H
