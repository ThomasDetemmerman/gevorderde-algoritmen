//
// Created by Thomas on 14/12/2019.
//

#ifndef INC_09_2_STABLE_MARIAGE_VOORKEURENLEZER_H
#define INC_09_2_STABLE_MARIAGE_VOORKEURENLEZER_H

#include <string>
#include <sstream>
#include <fstream>
#include "Persoon.h"
#include <iostream>

using namespace std;

class voorkeurenlezer {
    vector<string> getValue(string rawline){
        int start = rawline.find(':');
        rawline = rawline.substr(start+2);

        vector<string> array;
        stringstream ss(rawline);
        string temp;
        while (ss >> temp)
            array.push_back(temp);
        return array;
    }
public:
    vector<vector<Persoon>> lees(const string datafile){
        vector<Persoon> vrouwen;
        vector<Persoon> mannen;

        std::ifstream file(datafile);
        std::string str;
        if (!file.is_open()) {
            std::cout << "Error opening file" << std::flush;
            assert(false);
        }

        while (!file.eof()){
            string naam, geslacht_s, voorkeurenlijst_s;
            char geslacht;
            vector<string> voorkeurenlijst;

            getline(file, geslacht_s);
            geslacht = getValue(geslacht_s)[0][0]; //convert string to char

            getline(file, naam);
            naam = getValue(naam)[0];

            getline(file, voorkeurenlijst_s);
            voorkeurenlijst = getValue(voorkeurenlijst_s);

            if(geslacht == 'm'){
                mannen.emplace_back(Persoon(geslacht,naam, voorkeurenlijst));
            } else {
                vrouwen.emplace_back(Persoon(geslacht,naam, voorkeurenlijst));
            }
            if(!file.eof()){
                string emptyline;
                getline(file,emptyline);
            }

        }
        return {mannen, vrouwen};
    }
};


#endif //INC_09_2_STABLE_MARIAGE_VOORKEURENLEZER_H
