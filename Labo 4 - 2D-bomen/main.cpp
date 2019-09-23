#include "point.h"
#include "KDTree.h"
#include <fstream>

using namespace std;

//const unsigned int AANTAL_DIM = 2;
#define AANTAL_DIM 2
const char* OUT_FILE = "boom.dot";
const string COORDINATES_IN_FILE = "puntenverz.txt";


int main(int argc, char** argv) {
    ifstream infile(COORDINATES_IN_FILE);

    if(infile.is_open()){
        KDTree<AANTAL_DIM> boom;
        int x,y;
        while(infile >> x >> y){
            int coo[AANTAL_DIM] = { x,y};   //array maken
            Point<AANTAL_DIM> p(coo);       //punt aanmaken met als value de array
            boom.add2(p);                    //punt toevoegen
        }
        boom.draw(OUT_FILE);
    }

};