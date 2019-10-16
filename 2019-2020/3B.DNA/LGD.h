//
// Created by Thomas on 14/10/2019.
//

#ifndef INC_2_LABO_LGD_H
#define INC_2_LABO_LGD_H

#include <string>
#include <vector>
using namespace std;

class LGD {



public:
    LGD();
    int calculateLGD(const string &a, const string &b);
};

LGD::LGD() {};

int LGD::calculateLGD(const string &a, const string &b) {
    //init phase
    vector<vector<int>> M;
    for(int rij=0; rij < a.length(); rij++){
        vector<int> kolom(b.length());
        M.push_back(kolom);
        M.at(rij).at(0) = 0;

    }
    for(int y = 0; y < b.length(); y++){
        M.at(0).at(y) = 0;

    }

    //calc phase
    for(int x=1; x < a.length(); x++){
        for(int y = 1; y < b.length(); y++){
            if(a[x] == b[y]){
                int prev = M.at(x-1).at(y-1);
                M.at(x).at(y) = prev+1;
            }
            else {
                int boven = M.at(x).at(y-1);
                int links = M.at(x-1).at(y);
                M.at(x).at(y) = max(boven, links);
            }
        }
    }
    return M.at(a.length()-1).at(b.length()-1);
}


#endif //INC_2_LABO_LGD_H
