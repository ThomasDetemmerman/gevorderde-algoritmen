//
// Created by Thomas on 05/12/2019.
//

#ifndef A___LABO_SUFFIXTABELLEN_LCP_H
#define A___LABO_SUFFIXTABELLEN_LCP_H
#include <iomanip>
#include "LGP.h"

class LCP : public vector<int>{
public:
    LCP(SAIS suffixArray, string data_);
    friend std::ostream& operator<<(std::ostream & os,const  LCP & l);
private:
    SAIS sa;
    string data;
    void maakLCP();

    int getcommonPrefixLenght(int i, int j, int k);
};


LCP::LCP(SAIS suffixArray, string data_) : data(data_), sa(suffixArray), vector<int>(suffixArray.size(),0) {
    maakLCP();
}

void LCP::maakLCP() {
    int len = this->size();
    vector<int> rank(len, 0);
    for (int i = 0; i < len; i++)
        rank[sa[i]] = i;

    int l = 0;
    for (int i = 0; i < len; i++) {
        if (rank[i] == len - 1) {
            l = 0;
        } else {
            int j = sa[rank[i] + 1];
            l = getcommonPrefixLenght(i,j,l);
            this->operator[](rank[i]) = l;
            if (l) {
                l--;
            }
        }
    }
}

int LCP::getcommonPrefixLenght(int i, int j, int k) {
    while (i + k < size() && j + k < size() && data[i+k] == data[j+k]){
        k++;
    }
    return k;
}

std::ostream& operator<<(std::ostream & os,const  LCP & l){
    for(int i = 0; i < l.size(); i++){
        os << std::setw(5) << std::left << i;
        std::string woord;
        for(int j = l.sa[i]; j < l.size(); j++){
            char letter = l.data[j];
            woord += l.data[j];
        }
        os << std::setw(40) << std::left << woord;
        os << std::setw(3) << l.operator[](i) << std::endl;
    }
}
#endif //A___LABO_SUFFIXTABELLEN_LCP_H
