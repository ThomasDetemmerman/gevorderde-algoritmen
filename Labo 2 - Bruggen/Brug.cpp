#define DEBUG 1
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

using namespace std;

struct Node_t
{
	int arenaID;
	int subtotaal;
	Node_t *prev;
};

bool kruisen(int arenaID_A, int arenaID_B, vector<int> bruggen)
{
#ifdef DEBUG
	cout << arenaID_A << " <> " << arenaID_B << endl;
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
	return result;
};

int main()
{
	ifstream inFile;

	int fileSiz, teller = 0;

	inFile.open("versnick.dat");

	inFile >> fileSiz;

	//beter als matrix aangezien het anders een heel ijle matrix zou zijn.
	vector<int> bruggen(fileSiz);
	vector<int> smeergeld(fileSiz);

	while (!inFile.fail() && teller < fileSiz)
	{ //alternatief: herhaal fileSiz keer.
		inFile >> smeergeld[teller];
		inFile >> bruggen[teller];
		teller++; //regelnummer
	}
	inFile.close();
	/*for(int i=0; i<bruggen.size(); i++){
		cout << i << " - " << bruggen[i] << "\t(" << smeergeld[i] << ")" << endl;
	}
	*/



	/****************************************
	*       effectieve code
	*****************************************/

	//init een eerste brug
	vector<Node_t **> leaves;
	struct Node_t* nodeptr = (struct Node_t*)malloc(sizeof(struct Node_t));
	nodeptr->arenaID = 0;
	nodeptr->subtotaal = smeergeld[0];
	nodeptr->prev = NULL;
	leaves.push_back(&nodeptr);

	//voeg andere bruggen toe
	for (int arenaID = 1; arenaID < bruggen.size(); arenaID++)
	{
#ifdef DEBUG
		cout << "------- Processing Arena " << arenaID << endl;
#endif 

		//De inhoud van deze lus kan knopen aan de leaves toevoegen. Deze wensen we echter niet te controleren. Daarom
		//heb ik ervoor gekozen om van x naar 0 te gaan zodat x+1 niet gecontroleerd zou worden indien we zouden optellen.
		for (int currentLeaf = leaves.size() - 1; currentLeaf >= 0; currentLeaf--)
		{
			cout << "-- aantal leaves " << leaves.size() << endl;

			Node_t *leave = *(leaves[currentLeaf]);
			Node_t *voorhanger = leave;

			//zolang we kruisende bruggen hebben gaan stijgen we in de boom
			while (voorhanger != 0 && kruisen(voorhanger->arenaID, arenaID, bruggen))
			{
				voorhanger = voorhanger->prev;
			}

			//nieuwe node aanmaken
			//Node_t nieuw = { arenaID, smeergeld[arenaID] + voorhanger->subtotaal, voorhanger }; //Werkt niet! Er zal maar 1 instantie aangemaakt worden en deze wordt voordurend overschreven.
			struct Node_t* nodeptr = (struct Node_t*)malloc(sizeof(struct Node_t));
			nodeptr->arenaID = arenaID;
			int tmp = 0;
			if (voorhanger) {
				 tmp = voorhanger->subtotaal;
			}
			nodeptr->subtotaal = smeergeld[arenaID] + tmp;
			nodeptr->prev = voorhanger;
#ifdef DEBUG
		//	cout << "nieuwe knoop {" << arenaID << " , " << voorhanger->subtotaal << " , " << voorhanger->prev << " }\n";
#endif 

			//plaatsen van nieuwe node in onze graaf.
			//Indien deze geen problemen gaf met de voorhanger voegen we hem gewoon toe. Het adres van de vorige Node_t wordt vervangen
			// door deze van 'nieuw'. Dit wordt de leave van deze tak.

			if (leave == voorhanger)
			{
#ifdef DEBUG
				cout << "updating leaf " << leaves[currentLeaf];    //flush to prevent buffering
#endif
				leaves[currentLeaf] = &nodeptr;
#ifdef DEBUG
				cout << "-> " << leaves[currentLeaf] << endl;    //flush to prevent buffering
#endif
			}
			else
			{ //indien we kruisende bruggen hebben gehad hebben we extra moeten terugkeren. Indien dit het geval was
				// hebben we een aftakking gemaakt en hebben we een nieuwe header.
#ifdef DEBUG
				cout << "new leaf\n" << std::flush;
#endif
				//leaves.push_back(&nieuw);
			}


		}
	}

	/*
	 *   zoek oplossing uit de gegenereerde datastructuur
	 */

		 Node_t *leaveMetMaxSmeergeld = *leaves[0];
		 for (int i = 1; i < leaves.size(); i--)
		 {

			 if (leaveMetMaxSmeergeld->subtotaal < (*leaves[i])->subtotaal)
			 {
				 leaveMetMaxSmeergeld = *leaves[i];
			 }
		 }

	 /*
	 *   print resultaat
	 */

	 Node_t *current = leaveMetMaxSmeergeld;
	 cout << "smeergeld: " << leaveMetMaxSmeergeld->subtotaal << endl << "Bruggen die gebouwd moeten worden" << endl;
	 while (current)
	 {
		 cout << current->arenaID << " - " << bruggen[current->arenaID] << "\t(" << smeergeld[current->arenaID] << ")" << endl;
		 current = current->prev;
	 }

	return 1;
}
