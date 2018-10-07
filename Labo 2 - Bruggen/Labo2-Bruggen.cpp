#define DEBUG 1
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Node_t
{
	int arenaID;
	int subtotaal;
	Node_t *prev;
};
void dump(Node_t* nieuw) {
	cout << &nieuw << "{" << nieuw->arenaID << " , " << nieuw->subtotaal << " , " << nieuw->prev << " }";
}

void dump(vector<Node_t *> tree) {	//to do: reference
	for (int i = 0; i < tree.size(); i++)
	{
		Node_t* a = tree[i];
		while (a != NULL) {
			dump(tree[i]);
			cout << " > ";
			a = a->prev;

		}
		cout << endl;
	}
}



bool kruisen(int arenaID_A, int arenaID_B, vector<int> bruggen)
{
#ifdef DEBUG
	cout << arenaID_A << "(" << bruggen[arenaID_A] << ")"<< " <> " << arenaID_B << "(" << bruggen[arenaID_B] << ")";
#endif
	bool result;
	if (arenaID_A < arenaID_B)
	{
		result = (bruggen[arenaID_A] > bruggen[arenaID_B]);
	}
	else
	{
		result = (bruggen[arenaID_A] < bruggen[arenaID_B]);
	}
	cout << (result ? "(kruisen)" : "(kruisen niet)") << endl;
	return result;
};

int main()
{
	ifstream inFile;

	int fileSiz, teller = 0;

	inFile.open("Text.txt");
	if (!inFile.is_open()) {
		
		std::perror("Unable to open file");
	}
	

	inFile >> fileSiz;

	//beter als matrix aangezien het anders een heel ijle matrix zou zijn.
	vector<int> smeergeld(fileSiz);
	vector<int> bruggen(fileSiz);
	

	while (!inFile.fail())
	{ //alternatief: herhaal fileSiz keer.
		inFile >> smeergeld[teller];
		inFile >> bruggen[teller];
		teller++; //regelnummer
	}
	inFile.close();

	for(int i=0; i<bruggen.size(); i++){
		cout << i << " - " << bruggen[i] << "\t(" << smeergeld[i] << ")" << endl;
	}
	

	/*
	*       effectieve code
	*/

	//init een eerste brug
	vector<Node_t *> leaves;
	Node_t nieuw = { 0, smeergeld[0], NULL };
	leaves.push_back(&nieuw);

	//voeg andere bruggen toe
	for (int arenaID_c = 1; arenaID_c < bruggen.size(); arenaID_c++)
	{
#ifdef DEBUG
		cout << "------- Processing Arena " << arenaID_c << endl;
#endif 

		//De inhoud van deze lus kan knopen aan de leaves toevoegen. Deze wensen we echter niet te controleren. Daarom
		//heb ik ervoor gekozen om van x naar 0 te gaan zodat x+1 niet gecontroleerd zou worden indien we zouden optellen.
		for (int currentLeaf = leaves.size() - 1; currentLeaf >= 0; currentLeaf--)
		{
			
			dump(leaves);
			cout << "-- aantal leaves " << leaves.size() << endl;

			
			Node_t *voorhanger = leaves[currentLeaf];

			//zolang we kruisende bruggen hebben gaan stijgen we in de boom
			while (voorhanger != 0 && kruisen(voorhanger->arenaID, arenaID_c, bruggen))
			{
#ifdef DEBUG
				cout << "voorhanger ";
				dump(voorhanger);
				cout << " wordt ";
				dump(voorhanger->prev);
#endif
				voorhanger = voorhanger->prev;
			}

			//nieuwe node aanmaken
#ifdef DEBUG
			cout << "voorhanger is ";
			dump(voorhanger);
			cout << endl;
#endif
			
			Node_t nieuw = Node_t();
			nieuw.arenaID = arenaID_c;
			nieuw.subtotaal = smeergeld[arenaID_c] + voorhanger->subtotaal;
			nieuw.prev =voorhanger;
		
#ifdef DEBUG
			cout << "new node: ";
			dump(&nieuw);
			cout << endl;
			dump(leaves);
#endif 

			//plaatsen van nieuwe node in onze graaf.
			//Indien deze geen problemen gaf met de voorhanger voegen we hem gewoon toe. Het adres van de vorige Node_t wordt vervangen
			// door deze van 'nieuw'. Dit wordt de leave van deze tak.

			if (leaves[arenaID_c] == voorhanger)
			{
#ifdef DEBUG
				cout << "updating leaf " << leaves[currentLeaf]->arenaID;    //flush to prevent buffering
#endif
				leaves[currentLeaf] = &nieuw;
#ifdef DEBUG
				cout << "-> " << leaves[currentLeaf]->arenaID << endl;    //flush to prevent buffering
#endif
			}
			else
			{ //indien we kruisende bruggen hebben gehad hebben we extra moeten terugkeren. Indien dit het geval was
				// hebben we een aftakking gemaakt en hebben we een nieuwe header.
#ifdef DEBUG
				cout << "new leaf\n" << std::flush;
#endif
				leaves.push_back(&nieuw);
			}


		}
	}

	/*
	 *   zoek oplossing uit de gegenereerde datastructuur
	 */
	 /*
		 Node_t *leaveMetMaxSmeergeld = leaves[0];
		 for (int i = 1; i < leaves.size(); i--)
		 {

			 if (leaveMetMaxSmeergeld->subtotaal < leaves[i]->subtotaal)
			 {
				 leaveMetMaxSmeergeld = leaves[i];
			 }
		 }
	 */
	 /*
	 *   print resultaat
	 */

	 /*Node_t *current = leaveMetMaxSmeergeld;
	 while (current)
	 {
		 cout << current->arenaID << " - " << bruggen[current->arenaID] << "\t(" << smeergeld[current->arenaID] << ")" << endl;
		 current = current->prev;
	 }*/

	return 0;
}
