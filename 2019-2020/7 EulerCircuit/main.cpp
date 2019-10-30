#include <iostream>
#include "graaf.h"
using std::vector;

void zoekScharnierpunten(Graaf<GERICHT> &graaf, int i);
void zoekScharnierpuntenRec(Graaf <GERICHT> &graaf, int i, vector<int> &laagsteTeBereiken,vector<int> &preOrderNummering, vector<bool> &ontdekt, vector<int> &parent);


int main() {
    Graaf <GERICHT> graaf(7);
    // compontent A
    graaf.voegVerbindingToe(1,2);
    graaf.voegVerbindingToe(2,1);
    graaf.voegVerbindingToe(1,3);
    graaf.voegVerbindingToe(3,2);

    // compontent B
    graaf.voegVerbindingToe(4,5);
    graaf.voegVerbindingToe(5,4);
    graaf.voegVerbindingToe(4,6);
    graaf.voegVerbindingToe(6,4);

    //brug 1
    graaf.voegVerbindingToe(1,6);

    //brug 2
    graaf.voegVerbindingToe(0,5);

    graaf.teken("output.dot");

    zoekScharnierpunten(graaf, 7);

    return 0;
}

void zoekScharnierpunten(Graaf <GERICHT> &graaf, int size){
    vector<bool> ontdekt(size, false);
    vector<int> parent(size, -1);
    vector<int> laagsteTeBereiken(size, -1);
    //volgens de theorie moet je eerst preorder nummeren en vervolgens bruggen zoeken. Deze twee kunnen echter wel gelijktijd.
    vector<int> preOrderNummering(size, -1);
    int num = 0;
    for(int i=0; i < size; i++){
        zoekScharnierpuntenRec(graaf, i, laagsteTeBereiken, preOrderNummering, ontdekt, parent);

    }

}

void zoekScharnierpuntenRec(Graaf <GERICHT> &graaf, int i, vector<int> &laagsteTeBereiken,vector<int> &preOrderNummering, vector<bool> &ontdekt, vector<int> &parent){
    ontdekt[i] = true;
    i++;
    preOrderNummering[i] = i;   // je preorder nummer opslaan.
    laagsteTeBereiken[i] = i;   // je kan altijd jezelf bereiken.

    // we gaan nu aan elk van onze buren (kinderen) vragen of zij toevallig een betere knoop kunnen bereiken.
    for(auto const&[buurID, numverb]: graaf.operator[](i)){

        // als het nog niet ontdekt is gaan we dit recursief herhalen
        if(!ontdekt[buurID]){
            parent[buurID] = i;
            zoekScharnierpuntenRec(graaf, buurID, laagsteTeBereiken, preOrderNummering, ontdekt, parent);

            // We hebben het kind net de opdracht gegeven te berekenen welke knoop hij kan bereiken, we gaan nu zien of deze beter is als wat we al hebben
            //dit kind kan een lager genummerde knoop bereiken.
             //als dat het geval is, is dat goed want dan kan jij die ook bereiken. We updaten onze waarde.
            if(laagsteTeBereiken[buurID] < laagsteTeBereiken[i]){
                laagsteTeBereiken[i] = laagsteTeBereiken[buurID];
            } else {
                if(laagsteTeBereiken[buurID] >= laagsteTeBereiken[i]){
                    std::cout << i << " is een scharnierpunt voor " << buurID << std::endl;
                }

            }
        }
        else {
         // als hij wel al ontdekt is hebben we misschien een terugverbinding
         // als dat het geval is:  is het een terugverbinding. Dit is goed nieuws want dan kunnen we via dit kind een lager genummerde knoop bereiken. (dus een knoop hoger in de boom)

            if(buurID != parent[i]){
               if(preOrderNummering[buurID] < laagsteTeBereiken[i]){
                   laagsteTeBereiken[i] = preOrderNummering[buurID];
               }
            }

        }
    }
}


