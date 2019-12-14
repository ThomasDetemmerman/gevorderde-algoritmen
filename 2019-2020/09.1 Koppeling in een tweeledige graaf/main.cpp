#include <iostream>
#include <vector>
#include "graaf.h"
#include <string>
#include "stroomnet.h"

using namespace std;

int main() {

    // gegeven
    vector<string> taakverdeling = { "1:987" , "2:567", "3:567", "4:9"}; //user 1 kan taken 9,8 en 7 uitvoeren. User 2 kan 5,6 en 7 uitvoeren.
    int size = 9; //gaande van 1-9

    //definieer producent en consument
    int producentID = 0;
    int consumentID = size+1;
    int aantaldagen = 1;
    int aantalWerknemersPerTaak = 1;

    //bouwen van bipartite graaf
    GraafMetTakdata <GERICHT,int> gr(size+2);
    for(string taak: taakverdeling){
        int userID = ((taak[0])- '0');
        gr.voegVerbindingToe(producentID, userID, aantaldagen);
        for(int i=2; i < taak.size(); i++){
            int taakID = ((taak[i])- '0');
            try {
                gr.voegVerbindingToe(userID, taakID, 1);
                gr.voegVerbindingToe(taakID, consumentID, aantalWerknemersPerTaak);
            }catch(GraafExceptie e){}

        }
    }
    
    //toon resultaat
    gr.teken("graaf.dot");
    Stroomnetwerk<int> sn(gr,0,size+1);
    sn.geefStroom().teken("sn.dot");
    return 0;
}
