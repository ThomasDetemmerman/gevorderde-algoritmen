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
    int cnd=0;
    int pos = 1;
    failureTable[0]=0;
  while (pos < _naaldlengte){
      if(naald[pos] < naald[cnd]){
          failureTable[pos] = failureTable[cnd];
          pos++;
          cnd++;
      }
      else{
          failureTable[pos] = cnd;
          cnd = failureTable[cnd];

          while (cnd >= 0 && failureTable[pos] != failureTable[cnd]){
              cnd = failureTable[cnd];
          }
          pos++;
          cnd++;
      }
      failureTable[pos] = cnd;
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
    
