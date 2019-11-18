//
// Created by Thomas on 18/11/2019.
//

#ifndef INC_7_LABO___COMPRESSIE_ELIASCODE_H
#define INC_7_LABO___COMPRESSIE_ELIASCODE_H

#include <cmath>
#include "bincode.h"

int MAX_KEY_SIZE = 40;
/*
 * Used to encode the code.dat
 */

class EliasCode {

public:
    void encode(Bincode bincode, const char * fileName);
private:
    Varbitpatroon getPrefix(Varbitpatroon bincode);

};

void EliasCode::encode(const Bincode bincode, const char * fileName) {
    obitstream bitstream(fileName);

    for(int i=0; i < bincode.size; i++){
        Varbitpatroon code =  bincode.operator[](i);
        Varbitpatroon prefix = getPrefix(code);
        prefix.schrijf(bitstream);
        code.schrijf(bitstream);
    }
}

Varbitpatroon EliasCode::getPrefix(Varbitpatroon bincode) {
    Varbitpatroon vbp;
    int bincodelengte = bincode.geeflengte();
    int aantalNullenVoorEersteEen = floor(std::log2(bincodelengte));
    for(int i=0; i < aantalNullenVoorEersteEen; i++){
        vbp.voegToe(false);
    }
    int i = 0;
    while (bincodelengte> 0) {

        // storing remainder in binary array
        bool bit = ((bincodelengte % 2)==1);
        vbp.voegToe( bit);
        bincodelengte = bincodelengte / 2;
        i++;
    }
    return Varbitpatroon();
}



#endif //INC_7_LABO___COMPRESSIE_ELIASCODE_H
