//
// Created by Thomas on 02/11/2019.
//

#ifndef INC_3_2_EXTENDIBLE_HASHING_DOCUMENT_H
#define INC_3_2_EXTENDIBLE_HASHING_DOCUMENT_H

#include <vector>

using std::vector;

template <class Data>
class Document : public vector<Data>{
private:
    const int maxAllowedSize = 5;
public:
    int k;
    Document(){};
    Document<Data>(int _k): k(_k){};
    Document& operator=(const Document & doc); //required by schijf.h
    bool voegToe(Data d);
    Data zoek(int s);
    bool isFull();
};


// todo: this should be sorted
template<class Data>
bool Document<Data>::voegToe(Data d) {
    bool succesvol = false;
    if(!isFull()){
        succesvol = true;
        this->emplace_back(d);
    }
    return succesvol;
}

template<class Data>
Document<Data>& Document<Data>::operator=(const Document<Data> &doc) {
    this->k = doc.k;
    vector<Data>::operator=(doc);
    return *this;
}

template<class Data>
Data Document<Data>::zoek(int s) {
    auto it = std::find(this->begin(), this->end(), s);
    if(it == this->end()){
        return -1;
    }
    return *it;
}

template<class Data>
bool Document<Data>::isFull() {
    return (this->size() >= maxAllowedSize);
}


#endif //INC_3_2_EXTENDIBLE_HASHING_DOCUMENT_H
