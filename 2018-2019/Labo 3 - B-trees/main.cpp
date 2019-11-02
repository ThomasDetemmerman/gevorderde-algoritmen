#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include "schijf.h"
#include "btree.h"

#define FILE "../loremIpsum.txt"
#define M 4

using namespace std;
int main1();
int main2();

int main(){
    main2();
    //main1();
}

int main2(){
    Schijf<Bknoop<int, char, M>> C_disk =  Schijf<Bknoop<int, char, M>>();
    Btree<int, char, M> bt(C_disk);
    for (int i = 0; i < 40; ++i) {
        bt.voegToe(i, i+'a');

    }
    bt.teken("output.dot");

    std::cout << "tekening gemaakt in output.dot"<< std::endl;

}

int main1(){
     
    //Hoe weet je onderstaande? Dankzij de te kijken wat de constructor verwacht van Btree. Nl. Btree(Schijf<Knoop>& s):schijf(s)
    Schijf<Bknoop<string, int, M>> C_disk =  Schijf<Bknoop<string, int, M>>();
    Btree<string, int, M> bt(C_disk);


	ifstream infile;
	infile.open(FILE);

    int key=0;
    string input;
	while (infile >> input)
    {
	    std::cout << "voeg toe:" << input << std::endl;
        bt.voegToe(input,key);
        //indien xdot loopt kan je live aanpassingen volgen
        bt.teken("output.dot");
        key++;
    }

    bt.teken("output.dot");
    std::cout << "tekening gemaakt in output.dot"<< std::endl;
    

    //Knoop<string, int, 3> result = bt.zoek("lorem");
    //cout << "key is: " << result->key << " and val is " << result->s;

}


