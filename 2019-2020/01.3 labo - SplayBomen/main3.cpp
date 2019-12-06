//
// Created by Thomas on 07/10/2019.
//


#include <iostream>
#include <fstream>
#include <string>
#include "splayboom.h"
#include <map>
#include <unistd.h>

using namespace std;

string cleanup(string *woord){
    string goodword;
    for(char x: (*woord)){
        if(isalpha(x)){
            goodword += tolower(x);
        }
    }
    return goodword;
};

int sum(vector<int> freq, int i, int j)
{
    int s = 0;
    for (int k = i; k <= j; k++)
        s += freq[k];
    return s;
}

int main(int argc, char *argv[]) {
    string FILENAME;

    if(argc == 2){
        FILENAME = argv[1];
    } else{
        FILENAME = "/Users/thomasdetemmerman/Documents/Gevorderde-Algoritmen/2019-2020/2_labo/Shakespeare_short.txt";
    }


    ifstream istrm(FILENAME);

    if (!istrm.is_open()) {
        cerr << "failed to open " << FILENAME << endl << "Try again by explicitly adding the file location as an argument.";

    }

    //build frequenty table
   // map<string, int> woorden;   //het is essentieel om gebruik te maken van een hash aangezien een array zorgt voor slechte tijdscomplexiteit. template is <woord, index> waarmij index verweist naar de possitie in de frequenty-array die de frequenty bevat van het woord
   vector<string> woorden;
    vector<int> frequency;
    int wordcount = 0;
    string woord;
    int roots[woorden.size()][woorden.size()];
    while (istrm >> woord)
    {
        wordcount++;
        woord = cleanup(&woord);
        auto it = find(woorden.begin(),woorden.end(), woord);
        if ( it == woorden.end() ) {
           frequency.push_back(1);
           woorden.push_back(woord);
        } else {
            int index = distance(woorden.begin(), it);
           frequency[index]++;
        }


    }

    //matrix om data in op te slaan. Dit voorkomt dat deelproblemen onnodig opnieuw berekend wordt
    vector<vector<int>>costmatrix(woorden.size());

    //we werken bottom up. We weten namelijk op voorhand ons kleinste deelprobleem. Deelbomen met maar 1 knoop.
    //deze krijgen als gewicht hun eigen frequentie.
    //dit slaan we op op de diagonaal
    for(int i=0; i<woorden.size(); i++){
        costmatrix[i].resize(woorden.size());
        costmatrix[i][i] = frequency[i];
    }


    //bereken deelproblemen die langer zijn. Hiervoor baseren we ons op het basisprobleem (waarden op de diagonaal)
    for(int lengte = 2; lengte < woorden.size(); lengte++){
        for(int leftIndex=0; leftIndex < woorden.size() - lengte +1; leftIndex++){
            int rightIndex = lengte+leftIndex-1;
             costmatrix[leftIndex][rightIndex]= INT_MAX;
            int tempresult;
            // Try making all keys in interval keys[i..j] as root
            for (int currentRootTry = leftIndex; currentRootTry <= rightIndex; currentRootTry++){
                int cost = 0;
                if(currentRootTry > leftIndex){
                    cost += costmatrix[leftIndex][currentRootTry-1];
                }
                if(currentRootTry < rightIndex){
                    cost += costmatrix[currentRootTry+1][rightIndex];
                }

                cost += sum(frequency, leftIndex, rightIndex);

                if(cost < costmatrix[leftIndex][rightIndex]){
                    //beter resultaat gevonden
                    costmatrix[leftIndex][rightIndex] = cost;
                    roots[leftIndex][rightIndex] = currentRootTry; //todo: the matrix to minimize the bin tree has been calculated. Now every time a node is elected as root, it should be saved.
                }
            }

        }

    }
    //todo: convert matrix to tree

   // int resultaat = costmatrix[0][wordcount-1];
    for(int i=0; i < woorden.size(); i++){
        for (int j = 0; j < woorden.size(); ++j) {
            std::cout<< roots[i][j] << " ";
        }
        cout << endl << flush;
    }
    Zoekboom<string, int> zkbm;
    cout<<endl;
    /*
        zkbm.voegtoe(woorden[index],frequency[index]);

    zkbm.teken("optizedTree.dot");
   // execl("./dot2png.sh", "./dot2png.sh", NULL);
*/

}


