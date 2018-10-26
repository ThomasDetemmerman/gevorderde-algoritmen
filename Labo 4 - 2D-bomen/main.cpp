#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "kdboom.h"

using namespace std;

int main()
{
	ifstream inFile;

	Boom2D boom;

	inFile.open("nieuw");

	
    int teller=0;
    int x, y;

	while (!inFile.fail() && ! inFile.eof())
	{ 

		inFile >> x;
		inFile >>y;
		teller++; //regelnummer
		boom.voegtoe(punt2(x,y));
        //cout << teller << " " << x << " " << y << endl;
	}
	inFile.close();
	
	boom.teken("boom.dot");

}

