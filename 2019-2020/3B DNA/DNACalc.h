//
// Created by Thomas on 16/10/2019.
//

#ifndef INC_3B_DNA_DNACALC_H
#define INC_3B_DNA_DNACALC_H

#include <string>

using namespace std;

class DNACalc {
public:
    DNACalc();
    //staat car voor Contents of the Address part of Register number
    //car(A) is dus de eerste letter van de string A
    string car(const string &a);
    //cdr voor Contents of the Decrement part of Register number.
    //cdr(A) is wat overblijft als we uit A de eerste letter schrappen.
    string cdr(const string &a);
};

DNACalc::DNACalc() {};

string DNACalc::car(const string &a) {
    if(a.empty()){
        throw("string cannot be empty");
    }
    return a.substr(0,1);
}

string DNACalc::cdr(const string &a) {
    if(a.empty()){
        throw("string cannot be empty");
    }
    return a.substr(1);
}


#endif //INC_3B_DNA_DNACALC_H
