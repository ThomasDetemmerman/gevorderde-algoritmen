#include <iostream>
#include "Graaf.h"

using std::vector;

bool containsNegativeLoop(std::vector<int> vector);
vector<int> bellmanFord( GraafMetTakdata <GERICHT,int> graaf, int SIZE);

void dijkstra(GraafMetTakdata<GERICHT, int> takdata);

int main() {
    const int SIZE = 5;

    //gegeven
    GraafMetTakdata <GERICHT,int> graaf(SIZE+1);
    graaf.voegVerbindingToe(0,1, 9);
    graaf.voegVerbindingToe(0,2,-1);
    graaf.voegVerbindingToe(1,3,-3);
    graaf.voegVerbindingToe(2,3,2);
    graaf.voegVerbindingToe(3,4,-10);



    //add a startnode with weight zero to all other nodes
    for(int i=0; i < SIZE; i++){
        graaf.voegVerbindingToe(SIZE, i,0);
    }

    //apply bellmanFord
    vector<int> weights = bellmanFord(graaf,SIZE+1);

    //reweight graph
    //Indien dijkstra ook de gewichten zal moeten teruggeven ipv enkel het kortste pad, dan mogen we niet de orignele verbindingen aanpassen maar moeten we een kopie nemen van de graaf.
    for(int i=0; i < SIZE; i++){ //we moeten maar tot en met SIZE-1 gaan want SIZE zelf is onze start node die we nodig hadden voor het herwegen van de graaf
        for(auto const&[knp, numverb]: graaf.operator[](i)){
           *graaf.geefTakdata(i,knp) = *graaf.geefTakdata(i,knp) + weights[i] - weights[knp];
        }
    }

    //now we can apply dijkstra
    dijkstra(graaf);



    return 0;
}

void dijkstra(GraafMetTakdata<GERICHT, int> takdata) {
    //todo
}

//Bellman ford is also implementened in 07.1 with more tests and documentation.
vector<int> bellmanFord( GraafMetTakdata <GERICHT,int> graaf, int SIZE){
    std::vector<int> predepredecessors(SIZE,-1);
    std::vector<int> weightToReachThisVertex(SIZE,INT_MAX);
    std::deque<int> order;
    std::vector<int> negativeLoopCounter(SIZE,0);

    //kick off with zero
    order.push_back(0);
    weightToReachThisVertex[0] = 0;

    while(!order.empty() && !containsNegativeLoop(negativeLoopCounter)){
        int start = order.front();
        negativeLoopCounter[start]++;
        order.pop_front();
        auto neighbours = graaf.operator[](start);
        for(auto const&[knp, numverb]:  neighbours ){
            int prevWeight = weightToReachThisVertex[knp];
            int newWeight = weightToReachThisVertex[start] + *(graaf.geefTakdata(start,knp));
            if(prevWeight > newWeight){
                weightToReachThisVertex[knp] = newWeight;
                predepredecessors[knp] = start;
                if(prevWeight == -1){
                    order.push_back(knp);
                } else {
                    order.push_front(knp);
                }
            }
        }
    }

   return weightToReachThisVertex;
}
bool containsNegativeLoop(std::vector<int> vector) {
    bool containsLoop = false;
    int current = 0;
    while(!containsLoop && current < vector.size()){
        if(vector[current] > vector.size()){
            containsLoop = true;
        }
        current++;
    }
    return containsLoop;
}
