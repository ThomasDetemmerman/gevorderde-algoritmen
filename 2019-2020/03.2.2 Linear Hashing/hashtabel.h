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
    int d = 1;
    int p = 0;
    const blokindex restPagina = 500;
    vector<int> hash(Data data);
    blokindex getBlokIndex(vector<int> key, int prefixLenght);
    void upgradeHashTabel();
    vector<int> dec2bin(Data n);

public:
    hashtabel();

    Data zoek(int i);

    void voegtoe(int sleutel);


};

template<class Data>
hashtabel<Data>::hashtabel(){
    Document<Data> doc(d);
    diskmanager.herschrijf(doc, 0);
    diskmanager.herschrijf(doc, 1);
    diskmanager.herschrijf(doc, restPagina);
}


template<class Data>
void hashtabel<Data>::voegtoe(int sleutel) {
    int index = getBlokIndex(hash(sleutel), d);
    Document<Data> doc;
    diskmanager.lees(doc, index);
    std::cout << "adding " << sleutel << " at blockindex " << index << std::endl;
    if(doc.isFull()){
        upgradeHashTabel();
    }
    if(doc.isFull()) {
        Document<Data> restdoc;
        diskmanager.lees(restdoc, restPagina);
        int restPageSuccesive = restdoc.voegToe(sleutel);
        if(!restPageSuccesive) {
            std::cout << "Could not add " << sleutel << " to restpage because it was full. Expending to a second restpage isn't implemented";
            exit(-1);
        }
        diskmanager.herschrijf(restdoc, restPagina);
    } else {
        doc.voegToe(sleutel);
        diskmanager.herschrijf(doc, index);
    }
}

// todo: fallback to restpage if not found
template<class Data>
Data hashtabel<Data>::zoek(int sleutel) {
    blokindex index = getBlokIndex(hash(sleutel), d);
    Document<Data> doc;
    diskmanager.lees(doc, index);
    std::cout << "searching " << sleutel << " at blockindex" << index << std::endl;
    int result = doc.zoek(sleutel);
    //if result was not found, try restpage
    if(result == -1){
        std::cout << "searching " << sleutel << " in restpage" << std::endl;
        diskmanager.lees(doc, restPagina);
        int result = doc.zoek(sleutel);
    }
   return result;

}


template<class Data>
void hashtabel<Data>::upgradeHashTabel() {
    p++;
    if(p == pow(2,d)){
        p=0;
        d++;
    }

    blokindex newPage = p + pow(2,d);

    Document <Data> ouddoc;
    Document <Data> newdoc;
    diskmanager.lees(ouddoc,p);

    //move data from splittet page
    for (int j = 0; j < ouddoc.size(); ++j) {
        if(p != getBlokIndex(hash(ouddoc.at(j)), d+1)){
            newdoc.voegToe(ouddoc.at(j));
            ouddoc.erase(ouddoc.begin()+j, ouddoc.begin()+j+1); //bovengrens is niet inbegrepen
        }
    }

    //move data from restpage
    Document <Data> restdoc;
    diskmanager.lees(restdoc,restPagina);
    for (int j = 0; j < restdoc.size(); ++j) {
        if(p != getBlokIndex(hash(restdoc.at(j)), d+1)){
            newdoc.voegToe(restdoc.at(j));
            restdoc.erase(restdoc.begin()+j, restdoc.begin()+j+1); //bovengrens is niet inbegrepen
        }
    }

    diskmanager.herschrijf(ouddoc, p);
    diskmanager.herschrijf(newdoc, newPage);

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
blokindex hashtabel<Data>::getBlokIndex(vector<int> key, int prefixLenght) {

    blokindex decimalNumber = 0;
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
