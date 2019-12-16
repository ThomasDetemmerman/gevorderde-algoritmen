//
// Created by Thomas on 04/12/2019.
//

#ifndef C___KNUTH_MORRIS_PRATT__PREFIX_H
#define C___KNUTH_MORRIS_PRATT__PREFIX_H

#include <vector>
#include <string>

using namespace std;

class Prefix : public vector<int>{
public:
    string data;
    Prefix(const string &zoekstring);

    friend ostream& operator<<(ostream& os, const Prefix& dt);
};

Prefix::Prefix(const string &zoekstring) : data(zoekstring), vector<int>(zoekstring.length(),0){
    for (int i = 1; i < zoekstring.length(); i++) {
        int j = this->operator[](i-1);
        while(j > 0 && zoekstring[i] != zoekstring[j]){
            j = this->operator[](j-1);
        }
        if( zoekstring[i] == zoekstring[j]){
            //j++;
            this->operator[](i) = ++j;
        }
    }
}

ostream& operator<<(ostream& os, const Prefix& dt)
{
    for (int i = 0; i < dt.size(); ++i) {
        os << dt.data[i] << "\t";
    }
    os << endl;
    for (int i = 0; i < dt.size(); ++i) {
        os << dt[i]<< "\t";
    }
    return os;
}

#endif //C___KNUTH_MORRIS_PRATT__PREFIX_H
