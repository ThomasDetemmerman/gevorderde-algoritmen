//
// Created by Thomas on 02/11/2019.
//

#ifndef INC_3_2_EXTENDIBLE_HASHING_HASHTABEL_H
#define INC_3_2_EXTENDIBLE_HASHING_HASHTABEL_H

#include <math.h>
#include "schijf.h"
#include "document.h"



template <class Data>
class hashtabel{

private:
    Schijf<Document<Data>> diskmanager;
    const int MAX_KEY_SIZE = 32;
    int d = 0;
    vector<blokindex> blokindexen;
    vector<int> hash(Data data);
    int getPrefixFromBinNum(vector<int> key, int prefixLenght);
    void upgradeHashTabel();
    vector<int> dec2bin(Data n);
    void splitsDocument(blokindex bi);

public:
    hashtabel();

    Data zoek(int i);

    void voegtoe(int sleutel);
};

template<class Data>
hashtabel<Data>::hashtabel(){
    this->blokindexen.resize(pow(2,0)); //1 plaats dus.
    Document<Data> doc(d);
    blokindexen[0] = diskmanager.schrijf(doc);
}


template<class Data>
void hashtabel<Data>::voegtoe(int sleutel) {
    int index = getPrefixFromBinNum(hash(sleutel), d);
    Document<Data> doc;
    diskmanager.lees(doc, blokindexen[index]);
    std::cout << "adding " << sleutel << " at blockindex " << blokindexen[index] << std::endl;
    bool succesvol = doc.voegToe(sleutel);

    if(!succesvol){
        if(d == doc.k){

            upgradeHashTabel();

        }
        splitsDocument(blokindexen[index]);
        int result = doc.voegToe(sleutel);
        assert(!result);
    }
    diskmanager.herschrijf(doc, blokindexen[index]);

}

template<class Data>
Data hashtabel<Data>::zoek(int sleutel) {
    int index = getPrefixFromBinNum(hash(sleutel), d);
    Document<Data> doc;
    diskmanager.lees(doc, blokindexen[index]);
    std::cout << "searching " << sleutel << " at blockindex" << blokindexen[index] << std::endl;
    return doc.zoek(sleutel);
}


// this function is quick and dirty written. All values are stored and added again.
// However some values still belong to the old document and should therefore not be moved.
// also data should probably be moved instead of copied and cleared afterwards.
template<class Data>
void hashtabel<Data>::splitsDocument(blokindex bi) {
    Document<Data> doc;
    diskmanager.lees(doc, bi);
    doc.k++;
    vector<Data> datas;
    for (int i = 0; i < doc.size(); ++i) {
        datas.emplace_back(doc[i]);
    }
    doc.clear();
    //todo er is hier mogelijk een fout geprogrammeerd. De originele blockindex mag uiteraard behouden blijven. Nu krijgt ook deze een nieuwe partitie
    for (int j = 0; j < d; ++j) {
        if(blokindexen[j] == bi){
            Document<Data> doc(d);
            blokindexen[j] = diskmanager.schrijf(doc);
        }
    }
    diskmanager.herschrijf(doc, bi);
    for(Data d: datas){
        voegtoe(d);
    }

}



template<class Data>
void hashtabel<Data>::upgradeHashTabel() {
    std::cout << "upgrading hashtabel from " << pow(2,d) << " to " << pow(2,d+1) << std::endl;
    d++;
    blokindexen.resize(pow(2,d));
    for (int i = pow(2,d-1); i < pow(2,d); ++i) {
        //this new index was related to 'branchedOfIndex'
        int branchedOfIndex = getPrefixFromBinNum(dec2bin(i), d-1);
        blokindexen[i]= blokindexen[branchedOfIndex];
    }

}

//////////////////
// hulp functies //
//////////////////


/*
 *  just a bad hash function
 */
template<class Data>
vector<int> hashtabel<Data>::hash(Data data) {
    return dec2bin(data);
}

/*
 * convert decimal number to binary nummer represented in a vector
 */
template<class Data>
vector<int> hashtabel<Data>::dec2bin(Data n) {
    vector<int> binNum(MAX_KEY_SIZE);
    int i = 0;
    while (n > 0) {

        // storing remainder in binary array
        binNum[i] = n % 2;
        n = n / 2;
        i++;
    }

    //std::reverse(binNum.begin(),binNum.end());

    return binNum;
}

template<class Data>
int hashtabel<Data>::getPrefixFromBinNum(vector<int> key, int prefixLenght) {

    int decimalNumber = 0;
    if(prefixLenght > 0) {
        for (int j = 0; j < prefixLenght; ++j) {
            if (key[j] == 1) {
                decimalNumber += pow(2, j);
            }
        }
    }
    return decimalNumber;
}



#endif //INC_3_2_EXTENDIBLE_HASHING_HASHTABEL_H
