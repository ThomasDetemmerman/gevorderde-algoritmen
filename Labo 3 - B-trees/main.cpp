#include <fstream>
#include <iostream>
#include <map>

#include "btree.h"
#include "schijf.h"
#include <string>
#define FILE "loremIpsum.txt"

using std::map;
using std::string;
using std::ifstream;

int main(){
     
    //Hoe weet je onderstaande? Dankzij de te kijken wat de constructor verwacht van Btree. Nl. Btree(Schijf<Knoop>& s):schijf(s)
    Schijf<Bknoop<string, int, 3>> C_disk;
    Btree<string, int, 3> bt(C_disk);


	ifstream infile;
	infile.open(FILE);

    int key=0;
    string input;
	while (infile >> input)
    {
        bt.voegToe(input,key);
        key++;
    }
    bt.memoryDump();
    bt.teken("test.dot");

    

    //Knoop<string, int, 3> result = bt.zoek("lorem");
    //cout << "key is: " << result->key << " and val is " << result->s;

}