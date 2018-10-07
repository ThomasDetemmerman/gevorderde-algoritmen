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

bool kruisen(int arenaID_A, int arenaID_B, vector<int> *bruggen)
{
    bool result;
    if (arenaID_A < arenaID_B)
    {
        result = (bruggen[arenaID_A] > bruggen[arenaID_A]);
    }
    else
    {
        result = (bruggen[arenaID_A] < bruggen[arenaID_A]);
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

    while (!inFile.fail())
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

    /*
    *       effectieve code
    */

    //init een eerste knoop
    vector<Node_t *> graafheaders(fileSiz);
    Node_t nieuw = {0, smeergeld[0], NULL};
    graafheaders.push_back(&nieuw);
 
    for (int i = 1; i < bruggen.size(); i++)
    {

        //De inhoud van deze lus kan knopen aan de graafheaders toevoegen. Deze wensen we echter niet te controleren. Daarom
        //heb ik ervoor gekozen om van x naar 0 te gaan zodat x+1 niet gecontroleerd zou worden indien we zouden optellen.
        for (int currentGraafheader = graafheaders.size(); i >= 0; currentGraafheader--)
        {
            {
                Node_t *graafheader = graafheaders[currentGraafheader];
                Node_t *voorhanger = graafheader;
                while (kruisen(voorhanger->arenaID, i, &bruggen))
                {
                    voorhanger = voorhanger->prev;
                }

                //nieuwe node aanmaken
                Node_t nieuw = {i, smeergeld[i] + voorhanger->subtotaal, voorhanger->prev};

                //plaatsen van nieuwe node in onze graaf.
                //Indien deze geen problemen gaf met de voorhanger voegen we hem gewoon toe. Het adres van de vorige Node_t wordt vervangen
                // door deze van 'nieuw'. Dit wordt de graafheader van deze tak.
                if (graafheader == voorhanger)
                {
                    graafheaders[currentGraafheader] = &nieuw;
                }
                else
                { //indien we kruisende bruggen hebben gehad hebben we extra moeten terugkeren. Indien dit het geval was
                    // hebben we een aftakking gemaakt en hebben we een nieuwe header.
                    graafheaders.push_back(&nieuw);
                }
            }
        }
    }

    /*
        *   zoek oplossing uit de gegenereerde datastructuur
        */

    /* Node_t *graafheaderMetMaxSmeergeld = graafheaders[0];
    for (int i = 1; i < graafheaders.size(); i--)
    {

        if (graafheaderMetMaxSmeergeld->subtotaal < graafheaders[i]->subtotaal)
        {
            graafheaderMetMaxSmeergeld = graafheaders[i];
        }
    }

    /*
        *   print resultaat
        */

    /*
    Node_t *current = graafheaderMetMaxSmeergeld;
    while (current)
    {
        cout << current->arenaID << " - " << bruggen[current->arenaID] << "\t(" << smeergeld[current->arenaID] << ")" << endl;
        current = current->prev;
    }
*/
    return 0;
}
