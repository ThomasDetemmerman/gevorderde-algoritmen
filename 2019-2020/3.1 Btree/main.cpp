//
// Created by Thomas on 11/10/2019.
//

#include "Bknoop.h"

int main(){
    Bknoop<int,char> bt(5);
    bt.voegToe(1,'a');
    bt.voegToe(2,'a');
    bt.voegToe(6,'a');
    bt.voegToe(7,'a');
    bt.voegToe(5,'a');
    bt.voegToe(4,'a');
    //bt.toString();
}