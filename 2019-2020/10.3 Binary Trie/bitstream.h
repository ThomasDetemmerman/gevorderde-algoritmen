#ifndef __BITSTREAM
#define __BITSTREAM
#include <fstream>
using std::ofstream;
using std::ifstream;
typedef unsigned int uint;

class ibitstream:private ifstream{
    public:
        ibitstream();
        ibitstream(const char* naam);                                  
        void open(const char* c);                                                               ;
        void close();                                         
        bool fail();                                               
        bool leesbit();                                                                                                                                                                      
    private:
        char inchar;
        uint bittel;//aantal verwerkte bits van laatst ingelezen byte
};

class obitstream:private ofstream{
    public:
        obitstream();
        obitstream(const char* naam);                                  
        void open(const char* c);                                                                                      ;
        void close();                                                                                                 
        bool fail();                                               
        //Opletten: het werkelijke uitschrijven gbeurt in groepen van acht
        //gebruik flush om overgebleven bits uit te schrijven.
        //na een flush mag schrijfbit niet meer gebruikt worden!
        void schrijfbit(bool bit);
        void flush();
    private:
        char uitchar;
        uint bittel;
};

ibitstream::ibitstream():ifstream(){}

ibitstream::ibitstream(const char* naam){
    open(naam);
}

void ibitstream::open(const char* c){
    ifstream::open(c,std::ios::binary);
    bittel=8;
}

void ibitstream::close(){
    ifstream::close();
}

bool ibitstream::fail(){
    return ifstream::fail();
}

bool ibitstream::leesbit(){
    if (bittel==8){
        inchar=get();
        bittel=0;
    }
    return (inchar & (char(1)<<(7-bittel++)))!=0;
}


obitstream::obitstream():ofstream(){}

obitstream::obitstream(const char* naam){
    open(naam);
}

void obitstream::open(const char* c){
    ofstream::open(c,std::ios::binary);
    bittel=0;
    uitchar=0;
}

void obitstream::close(){
    if (bittel>0)
        put(uitchar);
    ofstream::close();
}

bool obitstream::fail(){
    return ofstream::fail();
}


void obitstream::schrijfbit(bool bit){
    ++bittel;
    if (bit)
        uitchar|=(char(1)<<(8-bittel));
    if (bittel==8){
        put(uitchar);
        bittel=0;
        uitchar=0;
    }
}
void obitstream::flush(){
    if (bittel!=0)
        put(uitchar);
}

#endif
    
