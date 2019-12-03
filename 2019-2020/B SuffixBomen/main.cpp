#include <iostream>
#include "Knoop.h"
#include "suffixboom.h"

int main() {
    suffixboom sb("blabla");
    sb.teken("output.dot");
    return 0;
}