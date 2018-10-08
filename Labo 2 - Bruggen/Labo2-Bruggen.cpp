// Labo2-Bruggen.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <vector>
#include <fstream>


using namespace::std;


int main()
{
	//inlezen
	ifstream infile;
	//infile.open("simpelBrug.dat");
	infile.open("versnick.dat");

	int aantalKnopen;
	infile >> aantalKnopen;
	
	vector<int> smeergeld(aantalKnopen);
	vector<int> winkelcentra(aantalKnopen);
	int teller = 0;
	int temp;
	for(int i=0;i<aantalKnopen;i++){
		infile >> smeergeld[teller];
		infile >> winkelcentra[teller];
		teller++;
	}


	//effectieve code
	
	vector<pair<int, int>> datastructuur; //totale winst, wel sub eronder zit
	//overloop alle arena's met matching winkelcentra
	for (int i = 0; i < winkelcentra.size(); i++) {
		//"Winkelcentra i toevoegen aan de deeloplossing"
		datastructuur.push_back(make_pair(smeergeld[i], -1));

		//Alle voorgaande suboplossingen controleren om toe te voegen
		int maxSmeergeldSubopl=-1;
		for (int j = i - 1; j >= 0; j--) {
			//degene zoeken met het hoogste smeergeld
			if (maxSmeergeldSubopl < datastructuur[j].first) {
				//deze mag hier niet staan omdat je dan een maxsmeergeldsubopl zou opslaan die eventueel niet combineerbaar is
				//maxSmeergeldSubopl = datastructuur[j].first;

				//De nieuw toegevoegde conflicteert niet met de vorige suboplossing (dus je mag deze toevoegen)
				if(winkelcentra[i] - winkelcentra[j] >= 0){
					datastructuur[i].first = smeergeld[i] + datastructuur[j].first;	//winst van het subtotaal opslaan
					datastructuur[i].second = j;						//opslaan dat de deeloplosing J onder de huidige deeloplossing zit

					//De tijdelijk grootste voorgaande oplossing opslaan die mogelijk is (nadruk op mogelijk!)
					maxSmeergeldSubopl = datastructuur[j].first;
				}
			}
		}
	}


	//De index van de grootste gaan zoeken
	int grootsteWinst=0;
	int index = -1;
	for (int i = 0; i < datastructuur.size(); i++) {
		if (datastructuur[i].first > grootsteWinst) {
			grootsteWinst = datastructuur[i].first;
			index = i;
		}
	}

	//Uitschrijven
	cout << "Totale winst = " << datastructuur[index].first << endl;
	cout << "Arena's die een brug krijgen: ";
	cout << index << " ";

	int nextInChain = datastructuur[index].second;
	while (nextInChain != -1) {
		cout << nextInChain << " ";
		nextInChain = datastructuur[nextInChain].second;
	}
}




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
