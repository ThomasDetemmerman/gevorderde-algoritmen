#include "punt2.h"
#include <memory>

using namespace std;

// wrapper rond punt2.h voor extra attributen toe te voegen
class Knoop
{
    friend class Boom;

  public:
    punt2 punt;
    Boom linkerkind, rechterkind;
    Knoop();
    Knoop(punt2);
};

Knoop::Knoop(){};
Knoop::Knoop(punt2 p):punt(p){};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////         Boom           /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Boom : public unique_ptr<Knoop>
{

  public:
    bool isX;
    void voegtoe();
    // zoek heeft uitvoerparameter met al dan niet gevonden deelboom
    // bool = true indien gevonden
    bool zoekInCollectie(punt2,Boom);
    Knoop zoekDichtstePunt();

  private:
     void togglePlaats();
};

bool Boom::zoekInCollectie(punt2 p, Boom plaats){

    if(plaats.isX){
        if(plaats->punt.x < p.x){
            //plaats.get()->linkerkind enkel als je de pointer zelf nodig hebt
            plaats.togglePlaats();
            zoekInCollectie(p,plaats->linkerkind);
        }else{
             plaats.togglePlaats();
             zoekInCollectie(p,plaats->rechterkind);
        }
    }
    else{
        if(plaats->punt.y < p.y){
             plaats.togglePlaats();
            zoekInCollectie(p,plaats->linkerkind);
        }else{
             plaats.togglePlaats();
             zoekInCollectie(p,plaats->rechterkind);
        }
    }
};

void Boom::togglePlaats(){
    this->isX = !this->isX;
}
 