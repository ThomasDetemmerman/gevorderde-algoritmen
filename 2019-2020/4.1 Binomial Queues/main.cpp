#include <iostream>
#include "BinQueue.h"

int main() {
    BinQueue<int,char> bq(10);
    bq.voegToe(1, 'a');
    bq.voegToe(2, 'a');
    return 0;
}