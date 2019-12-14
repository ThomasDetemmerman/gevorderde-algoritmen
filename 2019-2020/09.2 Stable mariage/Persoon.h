//
// Created by Thomas on 14/12/2019.
//

#ifndef INC_09_2_STABLE_MARIAGE_PERSOON_H
#define INC_09_2_STABLE_MARIAGE_PERSOON_H

#include <string>
#include <vector>

using namespace std;

class Persoon {
private:
    const char gender;
    std::vector<string> voorkeurlijst;

public:
    Persoon(const char g, const string n, vector<string> vl);
    string laatstepartner;
    const std::string naam;
    bool verkiestDezeManBovenHuidige(string naam);
    void nieuwePartner(string nieuwePartner);
    vector<string> geefResterendeOpties();
};

Persoon::Persoon(const char g, const string n, vector<string> vl) : gender(g), naam(n), voorkeurlijst(vl) {}

bool Persoon::verkiestDezeManBovenHuidige(string naam){
    if(laatstepartner.empty()){
        return true;
    }
    auto huidigePartner = find(voorkeurlijst.begin(), voorkeurlijst.end(), laatstepartner);
    auto nieuweCandidaat = find(voorkeurlijst.begin(), voorkeurlijst.end(), naam);
    return (nieuweCandidaat > huidigePartner);
}

void Persoon::nieuwePartner(string nieuwePartner){
    laatstepartner = nieuwePartner;
}

vector<string> Persoon::geefResterendeOpties(){
    vector<string>v;
    if(laatstepartner.empty() ){
        v =  voorkeurlijst;
    } else{
        auto it = find(voorkeurlijst.begin(), voorkeurlijst.end(), laatstepartner);
        auto end = voorkeurlijst.end();
        v= vector<string>(it+1, end);
    }
    return v;
}

#endif //INC_09_2_STABLE_MARIAGE_PERSOON_H
