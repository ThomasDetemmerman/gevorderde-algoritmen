//
// Created by Thomas on 24/10/2019.
//

#ifndef INC_5_HANDELSREIZIGERSPROBLEEM_HANDELSREIZIGERSPROBLEEM_H
#define INC_5_HANDELSREIZIGERSPROBLEEM_HANDELSREIZIGERSPROBLEEM_H

#include "graaf.h"
#include "component.h"
#include <set>
#include <unordered_map>

class Handelsreizigersprobleem {
private:

public:
    Handelsreizigersprobleem() {
    };



    void calculateePostOrder();


    void DEZrec(int startpunt, std::stack<int> &postordernummering, std::set<int> *visited, Graaf<GERICHT> &graaf);




    int vindVerantwoordelijkeVan(int lid, std::vector<component> &vector);



    std::set<int> convertStackToSet(std::stack<int> &stack);

    std::stack<int> calculateePostOrder(Graaf<GERICHT> &g, int size);

    std::vector<component> calculateComponents(Graaf<GERICHT> &graaf, std::stack<int> &stack);

    GraafMetKnoopdata<GERICHT, int> convertToComponentGrapth(Graaf<GERICHT> &graaf, std::vector<component> &vector );

    std::stack<int>  run(Graaf<GERICHT> &graaf, Graaf<GERICHT> &omgekeerdeGraaf);
};

std::stack<int>  Handelsreizigersprobleem::run(Graaf<GERICHT> &graaf, Graaf<GERICHT> &omgekeerdeGraaf) {
    //stap 1: bereken postordering met dez
    std::stack<int> postorder = calculateePostOrder(graaf, 5000);

    //stap 2: pas dez toe op elke postorder nummer dat nog niet vistided is
    std::vector<component> bos = calculateComponents(omgekeerdeGraaf, postorder);

    // stap 3: stel componentengraaf op
    GraafMetKnoopdata<GERICHT, int> componentengraaf =  convertToComponentGrapth(graaf, bos);


    //stap 4: topologisch sorteren
    //todo: mooier geprogrammeerd met een graaf.size() functie.


    std::stack<int> verkoopvolgordeknoopID = calculateePostOrder(componentengraaf, bos.size());
    std::stack<int> verkoopvolgorde;
    while(!verkoopvolgordeknoopID.empty()){
        int knoopID = verkoopvolgordeknoopID.top();
        verkoopvolgordeknoopID.pop();

        int familieID = *(componentengraaf.geefKnoopdata(knoopID));
        verkoopvolgorde.push(familieID);

    }
    return verkoopvolgorde;


}

std::stack<int> Handelsreizigersprobleem::calculateePostOrder(Graaf<GERICHT> &g,int size) {
    std::set<int> visited;
    std::stack<int> tmp;
    for(int i=0; i < size; i++){
        if(visited.find(i) == visited.end()){
            DEZrec(i, tmp, &visited, g);
        }

    }

    return tmp;
}

std::vector<component> Handelsreizigersprobleem::calculateComponents(Graaf<GERICHT> &omgekeerdegraaf, std::stack<int> &postorder) {
    std::set<int> visited;
    std::vector<component> bos;

    while (!postorder.empty()) {
        std::stack<int> tmpboom;
        int startpunt = postorder.top();
        postorder.pop();
        if (visited.find(startpunt) == visited.end()) {
            DEZrec(startpunt, tmpboom, &visited, omgekeerdegraaf);
            //waarom een stack als je hem toch converteert naar een set? DEZ wordt ook in stap1 aangeroepen en deze verwacht een stack.
            bos.push_back(component(convertStackToSet(tmpboom)));
        }

    }
    return bos;
}

void Handelsreizigersprobleem::DEZrec(int startpunt, std::stack<int> &postnummering, std::set<int> *visited,
                                      Graaf<GERICHT> &graaf) {

    // buren is een map:  beeldt knoopnummer (van buur) af op verbindingsnummer
    auto buren = graaf.operator[](startpunt);

    for (auto const&[knp, numverb]: buren) {
        //als de nog niet bezocht is geweest, recursief herhalen
        if (visited->find(knp) == visited->end()) {
            //std::cout << " adding " << knp << "to vistid list ";
            visited->insert(knp);
            // if(visited->find(knp) == visited->end()) std::cout << "adding failed" << std::endl << std::flush;
            //else std::cout << "adding OK" << std::endl << std::flush;
            // std::cout << "\tadding: " << knp << std::endl;
            DEZrec(knp, postnummering, visited, graaf);
        }

    }
    //als alle buren bezocht zijn push je uzelf op de stack
    postnummering.push(startpunt);
}

GraafMetKnoopdata<GERICHT, int> Handelsreizigersprobleem::convertToComponentGrapth(Graaf<GERICHT> &graaf, std::vector<component> &vector) {
    std::cout <<  "go catch some coffee ☕" << std::endl;
    GraafMetKnoopdata<GERICHT, int> componentengraaf;
    //mapt huisID (dus nummer in rss.txt) op knoopID. Dit is nodig omdat de niet alle huisID's meer gebruikt worden en dat zou te veel onnodig geheugen alloceren
    std::unordered_map<int, int> umap;
    for (int i = 0; i < vector.size(); i++) {
        std::set<int>::iterator compontent_it = vector[i].begin();
        int knoopIDCurrent, knoopIDBuur;
        int verantwoordelijkeHuidige = vector[i].getVerantwoordelijke();
        //indien verantwoordelijke van huidig compontent nog niet bestaat voegen we hem toe
        if (umap.find(verantwoordelijkeHuidige) == umap.end()) {
            knoopIDCurrent = componentengraaf.voegKnoopToe(verantwoordelijkeHuidige);
            umap[verantwoordelijkeHuidige] = knoopIDCurrent;
        } else {
            knoopIDCurrent = umap[verantwoordelijkeHuidige];
        }

        std::cout << "boom " << i << "/" << vector.size()<< " (verantwoordelijke: " << verantwoordelijkeHuidige <<  " ) \t-> " << vector[i].size()<< " members" << std::endl << std::flush;


        // voor elk knoop in compontent
        while (compontent_it != vector[i].end()) {
            //voor elke buur van die knoop in dat compontent
            for (auto buur: graaf.operator[](*compontent_it)) {
                //indien de buur van de knoop niet verbonden is met een knoop uit het zelfde component hebben we een brug gevonden.
                //deze brug zal toegevoegd moeten worden tussen de verantwoordelijken van beide sets
                if (!vector[i].contains(buur.first)) {
                    int verantwoordelijkeVanBuur = vindVerantwoordelijkeVan(buur.first, vector);
                    //indien deze buur nog niet in de graaf zit voegen we deze toe
                    if (umap.find(verantwoordelijkeVanBuur) == umap.end()) {

                        knoopIDBuur = componentengraaf.voegKnoopToe(verantwoordelijkeVanBuur);
                        umap[verantwoordelijkeVanBuur] = knoopIDBuur;
                    } else {
                        knoopIDBuur = umap[verantwoordelijkeVanBuur];
                    }
                    // ofwel zat hij er al in ofwel hebben we hem net toegevoegd. Wat het ook is, nu voegen we de verbinding toe.
                    try {
                        componentengraaf.voegVerbindingToe(knoopIDCurrent, knoopIDBuur);
                        //std::cout << "  └ verbinding toegevoegd van " << knoopIDCurrent << " -> " << knoopIDBuur << std::endl;
                    } catch (...) {
                        // verbinding bestaat al. Je zou er op kunnen chechen en indien het niet bestaat voeg je het toe
                        // of je maakt het je gemakkelijk en je catcht de error
                    }
                }
            }
            compontent_it++;
        }
    }
    std::cout << "compontentgraaf gegenereerd in dot file" << std::endl;
    componentengraaf.teken("output2.dot");
    return componentengraaf;

}

int Handelsreizigersprobleem::vindVerantwoordelijkeVan(int lid, std::vector<component> &vector) {
    for (component c: vector) {
        if (c.contains(lid)) {
            return c.getVerantwoordelijke();
        }
    }
    throw ("verantwoordelijke niet gevonden");
}

std::set<int> Handelsreizigersprobleem::convertStackToSet(std::stack<int> &stack) {
    std::set<int> tmp;
    while (!stack.empty()) {
        tmp.insert(stack.top());
        stack.pop();
    }
    return tmp;
}




#endif //INC_5_HANDELSREIZIGERSPROBLEEM_HANDELSREIZIGERSPROBLEEM_H
