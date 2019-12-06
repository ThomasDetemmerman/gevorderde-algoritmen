//
// Created by Thomas on 14/11/2019.
//

#ifndef INC_7_LABO___COMPRESSIE_DECODERINGSTRIE_H
#define INC_7_LABO___COMPRESSIE_DECODERINGSTRIE_H


#include <string>
#include "bitstream.h"
#include "bincode.h"
#include "binairetrie.h"
#include "EliasCode.h"
#include <iostream>

using namespace std;

class decoderingstrie {
private:
    Bincode bincode;
    BinaireTrie decodetrie;
public:
    decoderingstrie(const char *fileName);
    void decode(const char *fileName, const char *output);
    void teken(const char* bestandsnaam);
    void encodeAsEliasGammaCode(const char * filename);
};


decoderingstrie::decoderingstrie(const char *fileName): bincode(fileName), decodetrie(bincode){};

void decoderingstrie::decode(const char *codedFile, const char *output){

    std::ofstream outfile (output);

    ibitstream bitstream(codedFile);
    BinaireTrie* current = &decodetrie;
    TrieBlad* tb = static_cast<TrieBlad*>(current->get());
    while(!bitstream.fail()){
        tb = static_cast<TrieBlad*>(current->get());
        if(tb->isBlad()){

            int letter = tb->letter;
           // std::cout << "found blad at adress " << *current  << " with value " << letter << std::endl;
            //reset
            current = &decodetrie;
            outfile << (char)letter;
        } else {
            bool currentBit = bitstream.leesbit();
            TrieNietblad* tnb = static_cast<TrieNietblad*>(current->get());
            //std::cout << "naar " << ((currentBit)? "rechts":"links" )<< std::endl;
            current =  &(tnb->geefKind(currentBit));
        }
    }

    outfile.close();
    bitstream.close();

}

void decoderingstrie::teken(const char* bestandsnaam) {
    decodetrie.teken(bestandsnaam);
}

void decoderingstrie::encodeAsEliasGammaCode(const char *filename) {
    EliasCode ec;
    ec.encode(bincode, filename);
}

#endif //INC_7_LABO___COMPRESSIE_DECODERINGSTRIE_H
