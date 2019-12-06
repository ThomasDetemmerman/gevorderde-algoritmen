#include <iostream>
#include "TernaireBoom.h"

int main() {
    TernaireBoom tb;
    tb.voegToe("alimentation");
    tb.voegToe("alightments");
    tb.voegToe("alimentary");
    tb.voegToe("aliment");
    tb.voegToe("atherosclerosis");
    tb.voegToe("atherosclerotic");
    tb.voegToe("alimenting");
    tb.voegToe("xiphisternum");
    tb.voegToe("xiphisterna");
    tb.voegToe("zoogeographical");
    tb.voegToe("zoomorphic");
    tb.teken("ternaireBoom.dot");
    return 0;
}