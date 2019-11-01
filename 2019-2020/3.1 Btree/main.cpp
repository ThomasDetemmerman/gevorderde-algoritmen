//
// Created by Thomas on 11/10/2019.
//

#include "Btree.h"

int main(){
    Btree<int,char> bt(5);
    bt.voegToe(1,'a');
    bt.voegToe(2,'a');
    bt.voegToe(6,'a');
    bt.voegToe(7,'a');
    bt.voegToe(5,'a');
    bt.voegToe(4,'a');
    bt.toString();


    Schijf<string> disk;
    blokindex a = disk.schrijf("test");
    string b;
    disk.lees(b,a);
    cout << b;
}