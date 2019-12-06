//
// Created by Thomas on 28/10/2019.
//

#ifndef INC_3_3_DYNAMIC_PROGRAMMING_DYNAMICTREE_H
#define INC_3_3_DYNAMIC_PROGRAMMING_DYNAMICTREE_H

#include <vector>
#include <iostream>
#include "zoekboom17.h"

using std::vector;

class DynamicTree {

private:


    int _size;
    vector<vector<int>> G;
    vector<vector<int>> M;
    vector<vector<int>> R;
    vector<int> _data;
    vector<int> _freq;

    int g(int i, int j);

    int z(int i, int j);

    void buildG();
    void printTree(int i, int j, Zoekboom<int, string> &zkbm);
    void buildM();
    void printMatrix(vector<vector<int>> mat);
    int calculateG(int lowerboundary, int upperboundary);

public:
    DynamicTree(vector<int> data, vector<int> frequency);
    void printWeights();
    void printRoots();


    void printTree();


};





DynamicTree::DynamicTree(vector<int> data, vector<int> frequency) :  _data(data), _freq(frequency) {
    assert(_data.size() == _freq.size());

    // aangezien lower boundaries niet inbegrepen zijn en bijvoorbeeld G(0,0) nul terug geeft en G(0,1) enkel gewicht van 1, begint onze array vanaf index 1
    _data.insert(_data.begin(), 0);
    _freq.insert(_freq.begin(), 0);
    _size = _data.size()+1;
    G.resize(_size);
    M.resize(_size);
    R.resize(_size);
    for (int i = 0; i < _size; i++) {
        M[i].resize(_size);
        G[i].resize(_size);
        R[i].resize(_size);
    }

    buildG();
    buildM();
};

void DynamicTree::buildG() {
    for (int diagonaal = 0; diagonaal < _size; ++diagonaal) {
        for (int upperboundary = diagonaal; upperboundary < (_size); ++upperboundary) {
            int lowerboundary = upperboundary - diagonaal;
            G[lowerboundary][upperboundary] = calculateG(lowerboundary, upperboundary);
        }
    }
}


/*
 *  preconditie: buildG moet uitgevoerd zijn geweest om de inhoud van G-matrix op te vullen
 *  lower boundary i is not included
 *  upper boundary j is included
 */
int DynamicTree::g(int i, int j) {
    i++;
    if (i > j) return 0;
    return G[i][j];
}

/***
 * z functie zoals beschreven op pagina 30
 * @param i as lowerboundary
 * @param j as upperboundary
 * @return
 */

int DynamicTree::z(int i, int j) {

    if ((i + 1) > j) return 0;
    if ((i + 1) == j) return _freq[i];

    int min = INT_MAX;
    int min_root_ID = -1;
    int currentZ;
    for (int w = i; w < j; ++w) { //w van wortel
        currentZ = z(i, w - 1) + z(w + 1, j) + g(i, j); //formule zie cursus p30
        if(min > currentZ){
            min = currentZ;
            min_root_ID = w;
        }

    }
    R[i][j] = min_root_ID ;
    return min;
};

/***
 *  Deze functie lijkt sterk op g(). Deze functie wordt echter gebruikt voor het opbouwen van G terwijl g() meer als een getter kan gezien worden van de reeds gebouwde g matrix.
 * @param lowerboundary, boundary included
 * @param upperboundary, boundary included
 * @return
 */
int DynamicTree::calculateG(int lowerboundary, int upperboundary) {
    int sum = 0;
    for (int i = lowerboundary; i <= upperboundary; i++) {
        sum += _freq[i];
    }
    return sum;
}

void DynamicTree::buildM() {
    for (int diagonaal = 0; diagonaal < _size; ++diagonaal) {

        for (int upperboundary = diagonaal; upperboundary < (_size); ++upperboundary) {

            int lowerboundary = upperboundary - diagonaal;
            M[lowerboundary][upperboundary] = z(lowerboundary, upperboundary);
        }
    }
}


void DynamicTree::printMatrix(vector<vector<int>> mat) {
    for (int i = 0; i < _size; ++i) {
        for (int j = 0; j < _size; ++j) {
            if (i > j) {
                std::cout << "  ";
            } else {
                std::cout << mat[i][j];
            }
            std::cout << "\t";
        }
        std::cout << std::endl;
    }

}

void DynamicTree::printWeights() {
    printMatrix(M);

}

void DynamicTree::printRoots() {
    printMatrix(R);
}

void DynamicTree::DynamicTree::printTree(){
    Zoekboom<int, string> zkbm;
    printTree(0, _size-1, zkbm);
    zkbm.teken("output.dot");
}
void DynamicTree::printTree(int i, int j,Zoekboom<int, string> &zkbm){

  int ID = R[i][j];
    if(ID == 0){
        return;
    }
  int sleutel = _data[ID];
  std::string data = "[" + std::to_string(i) + "," + std::to_string(j) + "]";
  std::cout << "ID: " << ID << " : " << data << std::endl;

  zkbm.voegtoe(sleutel, data);
    if(i < ID){
        printTree(i, ID, zkbm);
    }
    if(ID < j) {
        printTree(ID, j, zkbm);
    }





}

#endif //INC_3_3_DYNAMIC_PROGRAMMING_DYNAMICTREE_H
