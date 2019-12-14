#include <iostream>
#include "Graaf.h"

bool containsNegativeLoop(std::vector<int> vector);
void bellmanFord( GraafMetTakdata <GERICHT,int> graaf, int SIZE);

int main() {
    const int SIZE = 5;

    //correcte graaf
    GraafMetTakdata <GERICHT,int> graaf(SIZE);
    graaf.voegVerbindingToe(0,1, 9);
    graaf.voegVerbindingToe(0,2,-1);
    graaf.voegVerbindingToe(1,3,-3);
    graaf.voegVerbindingToe(2,3,2);
    graaf.voegVerbindingToe(3,4,-10);
    graaf.teken("graaf.dot");

    //graaf met negatieve lus
    GraafMetTakdata <GERICHT,int> graafWithNegLoop(SIZE);
    graafWithNegLoop.voegVerbindingToe(0,1, 9);
    graafWithNegLoop.voegVerbindingToe(0,2,-1);
    graafWithNegLoop.voegVerbindingToe(1,3,-3);
    graafWithNegLoop.voegVerbindingToe(3,0,-17);
    graafWithNegLoop.voegVerbindingToe(2,3,2);
    graafWithNegLoop.voegVerbindingToe(3,4,-10);
    graafWithNegLoop.teken("graafNegLoop.dot");

    std::cout << "Correcte graaf:" << std::endl;
    bellmanFord(graaf,SIZE);

    std::cout << "Incorrecte graaf:" << std::endl;
    bellmanFord(graafWithNegLoop,SIZE);

    return 0;
}

void bellmanFord( GraafMetTakdata <GERICHT,int> graaf, int SIZE){
    std::vector<int> predepredecessors(SIZE,-1);
    std::vector<int> weightToReachThisVertex(SIZE,INT_MAX);
    std::deque<int> order;
    std::vector<int> negativeLoopCounter(SIZE,0);

    //kick off with zero
    order.push_back(0);
    weightToReachThisVertex[0] = 0;

    bool wijzigingenInVorigeIteratie = true;
    int aantalIteraties = 0;
    int V = SIZE

    while(wijzigingenInVorigeIteratie && aantalIteraties < V-1){
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

    //show results
    if(containsNegativeLoop(negativeLoopCounter)){
        std::cout << "could not solve this graph since it contains negative loops" << std::endl;
    } else {
        for(int i = 0; i < SIZE; i++){
            std::cout << "Vertex " << i << " can be reached via " << predepredecessors[i] << " with a wait of " << weightToReachThisVertex[i] << std::endl;
        }

        //print shortest path
        std::stack<int> shortesPath;
        int next = SIZE-1;
        while(next != -1){
            shortesPath.push(next);
            next = predepredecessors[next];
        }
        std::cout << std::endl << "Shortest path: ";
        while(!shortesPath.empty()){
            std::cout << shortesPath.top() << " -> ";
            shortesPath.pop();
        }
        std::cout << "end" << std::endl << std::endl;
    }



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
