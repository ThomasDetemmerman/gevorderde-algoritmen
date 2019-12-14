#include <iostream>
#include "DigitaleZoekBoom.h"

int main() {
    DigitaleZoekboom<8> DST;
    DST.voegToe("00000000");
    DST.voegToe("00010000");
    DST.voegToe("11000100");
    DST.voegToe("10000000");
    DST.voegToe("00000001");
    DST.voegToe("01111000");

    DST.teken("output.dot");
    return 0;
}