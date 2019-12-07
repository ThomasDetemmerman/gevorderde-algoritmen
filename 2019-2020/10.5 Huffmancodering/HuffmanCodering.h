//
// Created by Thomas on 07/12/2019.
//

#ifndef INC_10_5_HUFFMANCODERING_HUFFMANCODERING_H
#define INC_10_5_HUFFMANCODERING_HUFFMANCODERING_H

#include <map>
#include <string>
#include <vector>
#include <stack>
#include <fstream>

#include "Knoop.h"
#include "Blad.h"
#include "NietBlad.h"

using std::map;
using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;

typedef unique_ptr<Knoop> Knoopptr;

class HuffmanCodering {
private:
    void selectionSort(vector<int> &freqTable, vector<Knoopptr> &bos);
    Knoopptr root;

public:
    HuffmanCodering(const string & sampledata);
    void teken(const char *bestandsnaam);

    //todo
    void encode(const string & data);
    void decode(const string & data);
};

HuffmanCodering::HuffmanCodering(const string &sampledata) {
    map<char,int> index;
    vector<Knoopptr> patriciaBos;
    vector<int> freqTable;

    for(char c: sampledata){
        if(index.find(c) == index.end()){
            //Knoopptr nieuw = ;
            patriciaBos.push_back(move(make_unique<Blad>(c)));
            freqTable.push_back(1);
            index[c] = freqTable.size()-1;
        } else {
            freqTable[index[c]]++;
        }
    }

    while(patriciaBos.size() > 1){
        selectionSort(freqTable, patriciaBos);
        Knoopptr nieuwBos = make_unique<NietBlad>();
        static_cast<NietBlad*>(nieuwBos.get())->l = move(patriciaBos[0]);
        static_cast<NietBlad*>(nieuwBos.get())->r = move(patriciaBos[1]);
        patriciaBos[1] = move(nieuwBos);
        freqTable[1] += freqTable[0];
        freqTable.erase(freqTable.begin(), freqTable.begin()+1);
        patriciaBos.erase(patriciaBos.begin(), patriciaBos.begin()+1);
    }

    root = move(patriciaBos[0]);
}



void HuffmanCodering::selectionSort(vector<int> &freqTable, vector<Knoopptr> &bos) {
    int i = 0;
    int n = freqTable.size();
    int minIndex, j;
    while(i < n-1){
        minIndex = i;
        j = i+1;
        while(j < n){
            if(freqTable[j] < freqTable[minIndex]){
                minIndex = j;
            }
            j++;
        }
        swap(freqTable[minIndex], freqTable[i]);
        swap(bos[minIndex], bos[i]);
        i++;
    }
}


void HuffmanCodering::teken(const char *bestandsnaam) {
    std::ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\n";
    (*root).tekenrec(uit, knoopteller);
    uit << "}";
}


#endif //INC_10_5_HUFFMANCODERING_HUFFMANCODERING_H
