#include <iostream>
#include "BinQueue.h"

int main() {
   BinQueue<int,char> bq(10);
    for (int i = 0; i < 30; ++i) {
        bq.voegToe(i, 'a'+i);
    }


    bq.teken();
    bq.toString();

    char min = bq.top();
    while (min != -1){
        std::cout << min << " - "  << std::flush;
        min = bq.top();

    }

    return 0;
}