//
// Created by Thomas on 07/10/2019.
//


#include <iostream>
#include <fstream>
#include <string>
#include "splayboom.h"

#include <unistd.h>
using namespace std;

string cleanup(string *woord){

    string goodword;
    for(char x: (*woord)){
        if(isalpha(x)){
            goodword += tolower(x);
        }
    }

   // *woord = goodword;    //todo: why does this not work
    return goodword;
}

int main(int argc, char *argv[]) {
    string FILENAME;
    int wordcount;
    if(argc == 2){
        FILENAME = argv[1];
    } else{
        FILENAME = "/Users/thomasdetemmerman/Documents/Gevorderde-Algoritmen/2019-2020/2_labo/AChristmasCarol.txt";
    }


    ifstream istrm(FILENAME);

    if (!istrm.is_open()) {
        cerr << "failed to open " << FILENAME << endl << "Try again by explicitly adding the file location as an argument.";

    }

    Splayboom<string, int> splayboom;
    string woord;
    int countersplayboom = 0;
    zoekKnoop<string, int> *zoekknoop;
    Zoekboom<string, int> *ouder;
    while (istrm >> woord)
    {
        wordcount++;
        woord = cleanup(&woord);
        countersplayboom += splayboom.zoek(woord,zoekknoop, ouder);
        if(splayboom && splayboom->sleutel == woord){
            splayboom->data++;
        } else {

            splayboom.voegtoe(woord,1);
        }


    }
    cout << "Splayboom: aantal zoeken per woord: " << (int)countersplayboom/wordcount << endl;
    /***********************************/
    istrm.close(); //reset file
    istrm.open(FILENAME);

    Zoekboom<string, int> zoekboom;
    zoekKnoop<string, int> *ouder2;
    Zoekboom<string, int> *plaats;
    int counterzkbm = 0;
    while (istrm >> woord)
    {
        woord = cleanup(&woord);
        counterzkbm += zoekboom.zoek(woord,ouder2,plaats);
        if(*plaats){
            (*plaats)->data++;
        } else {
            zoekboom.voegtoe(woord,1);
        }


    }
    cout << "Zoekboom: aantal zoeken per woord: " << (int)counterzkbm/wordcount << endl;
    /***********************************/

    splayboom.zoek("WORKS",zoekknoop, ouder);
    cout<< "Het woord " << splayboom->sleutel << "  heeft " << splayboom->data << " hits" <<endl;
    //splayboom.teken("Shakespeare.dot");
   // execl("./dot2png.sh", "./dot2png.sh", NULL);
}


