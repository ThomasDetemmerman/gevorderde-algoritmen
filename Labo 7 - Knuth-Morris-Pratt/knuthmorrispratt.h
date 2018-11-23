#define DEBUG 0
#include <queue>
#include <iostream>

using namespace std;

typedef unsigned int uint;
typedef unsigned char uchar;
class KnuthMorrisPratt{
public:
    KnuthMorrisPratt(const uchar* naald,uint _naaldlengte);
    void zoek(std::queue<const uchar*>& plaats, const uchar* hooiberg, uint hooiberglengte);
    void printfailureTable() const;
private:
    const uchar* naald;
    uint _naaldlengte;
    int * failureTable;

    
};


KnuthMorrisPratt::KnuthMorrisPratt(const uchar* naald,uint _naaldlengte){
    /*
    *   Deze failuretabel wordt opgemaakt op basis van de prefix methode en geniet nog geen KMP optimalisatie
    *   Opmerking: volgende site maakt ook geen gebruik van deze optimalisatie: https://people.ok.ubc.ca/ylucet/DS/KnuthMorrisPratt.html 
    */
    this->failureTable = new int[_naaldlengte];
    this->naald = naald;
    this->_naaldlengte = _naaldlengte;
    int c=0;
    failureTable[0]=0;
    for(int gnd=1; gnd < _naaldlengte; gnd++){
       if(naald[gnd] != naald[c]){
#if DEBUG         
           cout << naald[gnd] << " != " <<  naald[c] << "-> 0" << endl;
#endif
           failureTable[gnd]=0;
           c=0;
       }else{
           c++;
           failureTable[gnd] = c;
#if DEBUG 
           cout << naald[gnd] << " == " <<  naald[c] << "-> " << c << endl;
#endif
       }
    }

};

void KnuthMorrisPratt::printfailureTable() const{
    for(int i=0; i < _naaldlengte; i++){
        cout << " " << naald[i];
    }
    cout << "\n";
        for(int i=0; i < _naaldlengte; i++){
        cout << " " << this->failureTable[i];
    }
};


void KnuthMorrisPratt::zoek(std::queue<const uchar*>& plaats,const uchar* hooiberg, uint hooiberglengte){
    int s = 0;
    int t = 0;


}
    
