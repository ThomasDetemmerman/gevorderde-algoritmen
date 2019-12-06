#include <iostream>
#include "graaf.h"
using std::vector;

void zoekScharnierpunten(Graaf<GERICHT> &graaf, int i);
void zoekScharnierpuntenRec(Graaf <GERICHT> &graaf, int i, int hoofdwortel, vector<int> &laagsteTeBereiken,vector<int> &preOrderNummering, vector<bool> &ontdekt, vector<int> &parent);
void controleerWortelOpScharnierpunten(Graaf<GERICHT> &graaf, int wortel);

int main() {
    Graaf <GERICHT> graaf(6);
    // compontent A
    std::cout << "preconditie: verbonden graaf" << std::endl;
    graaf.voegVerbindingToe(0, 1);
    graaf.voegVerbindingToe(1,2);
    graaf.voegVerbindingToe(2,3);
    graaf.voegVerbindingToe(3,4);
    graaf.voegVerbindingToe(4,5);
    graaf.voegVerbindingToe(5,3);


    graaf.teken("output.dot");

    zoekScharnierpunten(graaf, 6);

    return 0;
}

void zoekScharnierpunten(Graaf <GERICHT> &graaf, int size){
    vector<bool> ontdekt(size, false);
    vector<int> parent(size, -1);
    vector<int> laagsteTeBereiken(size, -1);
    //volgens de theorie moet je eerst preorder nummeren en vervolgens bruggen zoeken. Deze twee kunnen echter wel gelijktijd.
    vector<int> preOrderNummering(size, -1);
    int num = 0;
    controleerWortelOpScharnierpunten(graaf, 0);
    zoekScharnierpuntenRec(graaf, 0, 0, laagsteTeBereiken, preOrderNummering, ontdekt, parent);



}

void controleerWortelOpScharnierpunten(Graaf<GERICHT> &graaf, int i){
    if(graaf.operator[](i).size() > 1){
        std::cout << i << " is een scharnierpunt want hij heeft meerdere kinderen." << std::endl;
    } else {
        std::cout << i << " is geen scharnierpunt want hij heeft maar 1 kind." << std::endl;
    }
};


void zoekScharnierpuntenRec(Graaf <GERICHT> &graaf, int i, int hoofdwortel, vector<int> &laagsteTeBereiken,vector<int> &preOrderNummering, vector<bool> &ontdekt, vector<int> &parent){
    ontdekt[i] = true;

    preOrderNummering[i] = (i+1);   // je preorder nummer opslaan.
    laagsteTeBereiken[i] = (i+1);   // je kan altijd jezelf bereiken.

    // we gaan nu aan elk van onze buren (kinderen) vragen of zij toevallig een betere knoop kunnen bereiken.
    for(auto const&[buurID, numverb]: graaf.operator[](i)){

        // als het nog niet ontdekt is gaan we dit recursief herhalen
        if(!ontdekt[buurID]){
            parent[buurID] = i;
            zoekScharnierpuntenRec(graaf, buurID, hoofdwortel, laagsteTeBereiken, preOrderNummering, ontdekt, parent);

            // We hebben het kind net de opdracht gegeven te berekenen welke knoop hij kan bereiken, we gaan nu zien of deze beter is als wat we al hebben
            //dit kind kan een lager genummerde knoop bereiken.
             //als dat het geval is, is dat goed want dan kan jij die ook bereiken. We updaten onze waarde.
            if(laagsteTeBereiken[buurID] < laagsteTeBereiken[i]){
                laagsteTeBereiken[i] = laagsteTeBereiken[buurID];
            } else {
                if(laagsteTeBereiken[buurID] >= laagsteTeBereiken[i]){
                    if(i != hoofdwortel) { //de hoofdwortel moet gecheckt worden met controleerWortelOpScharnierpunten()
                        std::cout << i << " is een scharnierpunt voor " << buurID  << " (en zijn kinderen)" << std::endl;
                    }

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


