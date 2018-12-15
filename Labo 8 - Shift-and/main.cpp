#include "shiftand.h"
#include <string>
#include <iostream>

using std::cout;
using std::string;

int main(){

    string naald = "tomthomastom";

    Shiftand shiftand((const uchar*)naald.c_str(),naald.size());
    cout << shiftand.to_string();
}