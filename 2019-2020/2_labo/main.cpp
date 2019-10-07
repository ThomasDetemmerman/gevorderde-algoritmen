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
            goodword += x;
        }
    }

   // *woord = goodword;    //todo: why does this not work
    return goodword;
}

int main(int argc, char *argv[]) {
    string FILENAME;

    if(argc == 3){
        FILENAME = argv[1];
    } else{
        FILENAME = "/Users/thomasdetemmerman/Documents/Gevorderde-Algoritmen/2019-2020/2_labo/Shakespeare_short.txt";
    }


    ifstream istrm(FILENAME);

    if (!istrm.is_open()) {
        cerr << "failed to open " << FILENAME << endl << "Try again by explicitly adding the file location as an argument.";

    }

    Splayboom<string, int> splayboom;
    string woord;
    zoekKnoop<string, int> *zoekknoop;
    Zoekboom<string, int> *ouder;
    while (istrm >> woord)
    {
        woord = cleanup(&woord);
        //cout << "processing: " << woord << endl;
        splayboom.zoek(woord,zoekknoop, ouder);

        if(splayboom && splayboom->sleutel == woord){

            splayboom->data++;
        } else {

            splayboom.voegtoe(woord,1);
        }
    }
    splayboom.zoek("WORKS",zoekknoop, ouder);
    cout<< "Het woord " << splayboom->sleutel << "  heeft " << splayboom->data << " hits" <<endl;
    splayboom.teken("Shakespeare.dot");
    execl("./dot2png.sh", "./dot2png.sh", NULL);
}


