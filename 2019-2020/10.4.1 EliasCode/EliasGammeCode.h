//
// Created by Thomas on 11/12/2019.
//

#ifndef INC_10_4_1_ELIASCODE_ELIASGAMMECODE_H
#define INC_10_4_1_ELIASCODE_ELIASGAMMECODE_H

#include <string>
#include <vector>
#include <cmath>
#include <fstream>

#include "EliasCode.h"


using std::string;
using std::bitset;
using std::vector;
using std::merge;
using std::ofstream;
using std::ifstream;

class EliasGammeCode : public EliasCode{
public:
    virtual void encode(const char *bestandsnaam, const char *outname);

    virtual void decode(const char *bestandsnaam, const char *outname);

private:
    vector<bool> int2Bin(int val);

    void gammaEncodeByte(vector<bool> &data);

    void writeToFile(vector<bool> vector, const char *string1);

    int bin2int(vector<bool> bin);
};

void EliasGammeCode::encode(const char *bestandsnaam, const char *outname) {
    vector<bool> result;
    std::ifstream ifs(bestandsnaam);
    if (!ifs.is_open()) {
        std::cout << "Error opening file";
    }

    int ch;
    while (ifs >> ch) {
        vector<bool> bin = int2Bin(ch);
        gammaEncodeByte(bin);
        merge(result.begin(), result.end(), bin.begin(), bin.end(), std::back_inserter(result));
    }
    ifs.close();
    writeToFile(result, outname);

}

vector<bool> EliasGammeCode::int2Bin(int val) {
    vector<bool> bin;
    for (int i = 0; val > 0; i++) {
        (val % 2) ? bin.push_back(true) : bin.push_back(false);
        val /= 2;
    }
    return bin;
}

int EliasGammeCode::bin2int(vector<bool> bin) {
    int result;
    for (int i = 0; i < bin.size(); i++) {
        if (bin[i]) {
            result += pow(2, i);
        }
    }

}

void EliasGammeCode::gammaEncodeByte(vector<bool> &data) {
    vector<bool> header(data.size() - 1, false);
    vector<bool> dst;
    merge(header.begin(), header.end(), data.begin(), data.end(), std::back_inserter(dst));
    data = move(dst);
}

void EliasGammeCode::writeToFile(vector<bool> vector, const char *output) {
    ofstream of(output);
    for (bool bit: vector) {
        of << (bit ? "1" : "0");
    }
    of.close();
}

void EliasGammeCode::decode(const char *bestandsnaam, const char *outname) {
    ifstream ib(bestandsnaam);
    int result;
    int zeroCounter;
    bool readingDataSection = false;
    vector<bool> data;
    char i;
    while (ib >> i) {
        bool currentBit = ('0'==i)? false : true;
        if (readingDataSection) {
            zeroCounter--;
            data.push_back(currentBit);
            if (zeroCounter == 0) {
                readingDataSection = false;
                result = bin2int(data);
                data.clear();
            }
        } else {
            if (currentBit == true) {
                readingDataSection = true;
                //since there will be one zero less, we dont de-increment the zero counter
                data.push_back(currentBit);
            } else {
                zeroCounter++;
            }
        }
    }
    ib.close();

    ofstream of(outname);
    if (!of.is_open()) {
        std::cout << "Error opening file";
    }
    of << result;
    of.close();
}


#endif //INC_10_4_1_ELIASCODE_ELIASGAMMECODE_H
