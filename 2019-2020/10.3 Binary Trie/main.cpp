#include <iostream>
#include "BinaryTrie.h"

int main() {
    BinaryTrie bt;
    bt.voegToe("a");
    bt.voegToe("b");
    bt.voegToe("q");
    bt.voegToe("c");
    bt.teken("output.dot");
    return 0;
}