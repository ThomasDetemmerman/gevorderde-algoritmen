#ifndef __PROBPROB_H
#define __PROBPROB_H
#include <iostream>
#include <fstream>
#include <stdio.h>
using std::ostream;
using std::cerr;
using std::cout;
#include <vector>
//#include <pair>
using std::vector;
using std::pair;
//
// Probabilistic Probing
//
//De abstracte klasse ProbProb zoekt een vector van knopen die een optimale oplossing
//biedt voor een gegeven probleem waarvoor de zoekruimte te groot is om ze
//met backtracking helemaal af te gaan. Optimaal betekent met de beste waarde voor een
//evaluatiefunctie f.
//
//Hiervoor onderzoekt ze de zoekruimte op probabilistische wijze gebruik makend van
//een heuristiek h.
//
//
//Een oplossing bestaat uit een reeks waarden W_0, W_1, ..., W_k (waarbij k niet
//vooraf gegeven hoeft te zijn) die aan bepaalde voorwaarden voldoet en waarvoor f kan
//berekend worden.
//
//Oplossingen worden probabilistisch opgebouwd, Gegeven een deeloplossing W_0, ... W_j
//wordt de verzameling V_j+1 van mogelijke waarden bepaald voor W_j+1. Tevens wordt de
//heuristische waarde h(W_0, ... W_j, v) berekend voor alle v in V_j+1. het algoritme
//kiest dan probabilistisch de volgende waarde W_j+1; de probabiliteit om v te kiezen
//is evenredig met h(W_0, ... W_j, v).
//
//Dit proces wordt n keren herhaald door de lidfunctie
//
//vector<Waarde> ProbProb<Waarde>::geefOptimum(int n)
//
//die de best gevonden oplossing teruggeeft.
//
//Er wordt van uitgegaan dat een deeloplossing die een oplossing is nooit kan
//uitgebreid worden tot een betere oplossing en dat een lege deeloplossing geen
//oplossing kan zijn. bijgevolg mag de code nooit f evalueren op een lege
//deeloplossing.

//DEBUG zorgt ervoor dat elke oplossing wordt uitgeschreven
//#define DEBUG


template <class Waarde>
class ProbProb{
public:
//indien geefOptimum er niet in slaagt ook maar 1 oplossing te vinden geeft ze een
//lege vector terug
vector<Waarde> geefOptimum();
//De functie geefProbOplossing probeert op probabilistische manier 1 oplossing te
//vinden door constructief steeds verder te zoeken. Merk op: het kan zijn dat ze op
//dood spoor belandt en een deeloplossing vindt die niet verder kan uitgebreid worden
//maar ook geen oplossing is. In dat geval geeft ze een lege vector terug.

    vector<Waarde> geefProbOplossing();
/**********************************************************************

De onderstaande functies moeten ingevuld worden voor specifieke problemen.


***************************************************************************/
    virtual bool isOplossing(const vector<Waarde>& deeloplossing)=0;
    virtual double f(const vector<Waarde>& oplossing)=0;
//gegeven een deeloplossing geeft deze functie een vector met mogelijke voortzettingen
//samen met hun heuristische waarde
    virtual vector<pair<Waarde,double> >
                            geefVoortzetting(const vector<Waarde>& deeloplossing)=0;
    //neem een individu en zoek van daaruit lokaal optimum
    virtual void verbeterLokaal(vector<Waarde>&)=0;
};

template <class Waarde>
vector<Waarde> ProbProb<Waarde>::geefProbOplossing(){
    vector<Waarde> deeloplossing;
    bool deeloplossingIsOplossing=false;
    vector<pair<Waarde,double> > voortzetting=geefVoortzetting(deeloplossing);
    while(!deeloplossingIsOplossing && voortzetting.size() > 0){
        double totaleHeuristiek=0;
        for(auto&& p: voortzetting)
            totaleHeuristiek+=p.second;
        double lot=rand()*(totaleHeuristiek/RAND_MAX);
        int i=0;
        while(i<voortzetting.size()-1 && lot > voortzetting[i].second){
            lot-=voortzetting[i].second;
            i++;
        };
        deeloplossing.push_back(voortzetting[i].first);
        deeloplossingIsOplossing=isOplossing(deeloplossing);
        if (!deeloplossingIsOplossing)
            voortzetting=geefVoortzetting(deeloplossing);
    };
    if (!deeloplossingIsOplossing)
        deeloplossing.clear();

    return deeloplossing;
};

template <class Waarde>
vector<Waarde> ProbProb<Waarde>::geefOptimum(){
    vector<Waarde> poging=geefProbOplossing();
#ifdef DEBUG
    for(auto&& w : poging)
        cout<<" "<<w;
    cout<<"\n";
#endif
    verbeterLokaal(poging);
    return poging;
};




#endif