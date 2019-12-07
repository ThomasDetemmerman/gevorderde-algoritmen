#include <iostream>
#include "KarpRabin.h"

int main() {
    KarpRabin kb("tom");
    queue<int> results = kb.zoek("tom was op weg om tom bonen te zien met zijn tomtom.");
    while(!results.empty()){
        std::cout << results.front() << std::endl;
        results.pop();
    }
    return 0;
}