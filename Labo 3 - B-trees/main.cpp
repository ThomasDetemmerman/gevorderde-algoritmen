#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include "schijf.h"
#include "btree.h"

#define FILE "loremIpsum.txt"
#define M 4

using namespace std;

int main(){
     
    //Hoe weet je onderstaande? Dankzij de te kijken wat de constructor verwacht van Btree. Nl. Btree(Schijf<Knoop>& s):schijf(s)
    Schijf<Bknoop<string, int, M>> C_disk =  Schijf<Bknoop<string, int, M>>();
    Btree<string, int, M> bt(C_disk);


	ifstream infile;
	infile.open(FILE);

    int key=0;
    string input;
	while (infile >> input)
    {
        bt.voegToe(input,key);
        key++;
    }
    

    

    //Knoop<string, int, 3> result = bt.zoek("lorem");
    //cout << "key is: " << result->key << " and val is " << result->s;

}


