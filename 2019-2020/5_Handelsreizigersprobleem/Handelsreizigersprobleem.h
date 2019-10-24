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
    Graaf<GERICHT> graaf;
    Graaf<GERICHT> omgekeerdeGraaf;
    std::stack<int> postorder;//todo: not hardcode
    int size;
public:
    Handelsreizigersprobleem(Graaf<GERICHT> &g, Graaf<GERICHT> &gr) : graaf(g), omgekeerdeGraaf(gr) {
        postorder.push(3);
    }

    void run();

    void stap1();


    void DEZrec(int startpunt, std::stack<int> &postordernummering, std::set<int> *visited, Graaf<GERICHT> &graaf);


    void stap3(std::vector<component> vector);

    int vindVerantwoordelijkeVan(int lid, std::vector<component> vector);

    std::vector<component> stap2();

    std::set<int> convertStackToSet(std::stack<int> &stack);
};

void Handelsreizigersprobleem::run() {
    //stap 1: bereken postordering met dez
    stap1();

    //stap 2: pas dez toe op elke postorder nummer dat nog niet vistided is
    std::vector<component> bos = stap2();

    // stap 3: stel componentengraaf op
    stap3(bos);
}

void Handelsreizigersprobleem::stap1() {
    int asociaalGezin = 774;
    std::set<int> visited;
    std::stack<int> tmp;
    DEZrec(asociaalGezin, tmp, &visited, this->graaf);
    this->postorder = tmp;
}

std::vector<component> Handelsreizigersprobleem::stap2() {
    std::cout << std::endl << "stap 2" << std::endl;
    std::set<int> visited;
    std::vector<component> bos;

    while (!postorder.empty()) {
        std::stack<int> tmpboom;
        int startpunt = postorder.top();
        postorder.pop();
        if (visited.find(startpunt) == visited.end()) {
            DEZrec(startpunt, tmpboom, &visited, omgekeerdeGraaf);
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

void Handelsreizigersprobleem::stap3(std::vector<component> vector) {
    std::cout << "stap 3" << std::endl << "go catch some coffee ☕" << std::endl;
    GraafMetKnoopdata<GERICHT, int> componentengraaf;
    //mapt huisID (dus nummer in rss.txt) op knoopID. Dit is nodig omdat de niet alle huisID's meer gebruikt worden en dat zou te veel onnodig geheugen alloceren
    std::unordered_map<int, int> umap;
    for (int i = 0; i < vector.size(); i++) {
        std::cout << "boom " << i << "/" << vector.size() << " -> " << vector[i].size() <<  " members" << std::endl << std::flush;
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
                        std::cout << "  └ verbinding toegevoegd van " << knoopIDCurrent << " -> " << knoopIDBuur <<std::endl;
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
}

int Handelsreizigersprobleem::vindVerantwoordelijkeVan(int lid, std::vector<component> vector) {
    for (component c: vector) {
        if (c.contains(lid)) {
            c.getVerantwoordelijke();
        }
    }
    return -1;
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
