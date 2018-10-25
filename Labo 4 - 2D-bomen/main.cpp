#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

using namespace std;

int main()
{
	ifstream inFile;



	inFile.open("puntenverz.txt");

	
    int teller=0;
    int x, y;

	while (!inFile.fail())
	{ 

		inFile >> x;
		inFile >>y;
		teller++; //regelnummer
        cout << teller << " " << x << " " << y << endl;
	}
	inFile.close();
	

}

