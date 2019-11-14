#ifndef __BINAIRETRIE_H__
#define __BINAIRETRIE_H__
#include "bincode.h"
#include <functional>
#include <memory>
#include <sstream>

using std::cerr;
using std::make_unique;
/**********************************************************************

   Class: BinaireTrie, met hulpklassen van knopen
   
   beschrijving: Huffmantries, geschikt voor byte-alfabet
   letters zijn 0-255, plus afsluitingsteken 256.
   Een lege Huffmantrie is zinloos, ook als deeltrie
   Alleen bij opbouw zijn er zo'n tries mogelijk.
   Vandaar dat de parameterloze constructor protected is
   
***************************************************************************/
using std::binary_function;
using std::cout;
class Trieknoop;

typedef std::unique_ptr<Trieknoop> upKnoop;


class BinaireTrie: public upKnoop{
friend class TrieNietblad;
public:
    BinaireTrie(const Bincode&);

    BinaireTrie& operator=(upKnoop&& a){
        upKnoop::operator=(move(a));
        return *this;
    };
    BinaireTrie& operator=(BinaireTrie&& a){
        upKnoop::operator=(move(a));
        return *this;
    };
    //BinaireTrie(const BinaireTrie &) = default;
    BinaireTrie(BinaireTrie &&) = default;
    BinaireTrie():upKnoop(){}

    //teken
    void teken(const char *bestandsnaam);
private:
    string tekenrec(ostream &uit, int &knoopteller, int diepte);

protected:;
    BinaireTrie( upKnoop&& a):upKnoop(move(a)){};
//hulpfunctie voor constructor uitgaande van Bincode
    void voegToe(const Varbitpatroon& vb,int letter);


};

class Trieknoop{
public:
    virtual bool isBlad()=0;
    virtual string tekenrec(ostream &uit, int &knoopteller, int diepte) = 0;
};

class TrieBlad:public Trieknoop{
public:
    TrieBlad(int letter):letter{letter}{};
    virtual bool isBlad(){ return true;}
    int letter;

virtual string tekenrec(ostream &uit, int &knoopteller, int diepte);
};

class TrieNietblad:public Trieknoop{
public:
    TrieNietblad(){};
    TrieNietblad(BinaireTrie&& l,BinaireTrie&&r):links(move(l)),rechts(move (r)){};
    virtual bool isBlad(){ return false;}
    virtual string tekenrec(ostream &uit, int &knoopteller, int diepte);
    BinaireTrie& geefKind(bool rechterkind){
        return (rechterkind?
                    rechts:
                    links);
    };
    BinaireTrie links, rechts;
};

BinaireTrie::BinaireTrie(const Bincode& bc) {
    for(int i=0; i < bc.size; i++){
        this->voegToe(bc.operator[](i), i);
    }
};
void BinaireTrie::voegToe(const Varbitpatroon &vb, int letter) {
    std::cout << std::endl << (char)letter << " :";
    int diepte = 0;
    upKnoop *current = &(*this);
    while((current) != nullptr && (*current) != nullptr && diepte < vb.geeflengte()){
        std::cout << vb.geefbit(diepte) << std::flush;
        BinaireTrie *bt = &(static_cast<TrieNietblad *>(current->get())->geefKind(vb.geefbit(diepte)));
        current = move(static_cast<upKnoop*>(bt));
        diepte++;
    }
    while(diepte < vb.geeflengte()){
        std::cout << vb.geefbit(diepte) << std::flush;
        upKnoop nieuw = make_unique<TrieNietblad>();
        *current = move(nieuw);
        BinaireTrie *bt = &(static_cast<TrieNietblad *>(current->get())->geefKind(vb.geefbit(diepte)));
        current = move(static_cast<upKnoop*>(bt));

        diepte++;
       // BinaireTrie *bt = &(static_cast<TrieNietblad *>(current->get())->geefKind(vb.geefbit(diepte)));
        //current = static_cast<upKnoop*>(bt);
    }
    std::cout << vb.geefbit(diepte);
    upKnoop nieuw = make_unique<TrieBlad>(letter);
    *current = move(nieuw);

    //*current = move(make_unique<TrieBlad>(letter));



}

/************ teken ***********/


void BinaireTrie::teken(const char *bestandsnaam) {
    std::ofstream uit(bestandsnaam);

        assert(uit);
        int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
        uit << "digraph {\n";
        this->tekenrec(uit,knoopteller,0);

        uit << "}";
}

string BinaireTrie::tekenrec(ostream &uit, int &knoopteller, int diepte) {
    std::ostringstream wortelstring;
    if(!(*this)->isBlad()){
        wortelstring << static_cast<TrieNietblad*>(this->get())->tekenrec(uit, knoopteller, diepte);
    } else {
        wortelstring << static_cast<TrieBlad*>(this->get())->tekenrec(uit, knoopteller, diepte);
    }
    return wortelstring.str();
}

string TrieNietblad::tekenrec(ostream &uit, int &knoopteller, int diepte) {
    std::ostringstream wortelstring;
    if (this == nullptr)
    {
        wortelstring << "null";
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        wortelstring << '"' << "diepte: " << diepte  << '"';
        uit << wortelstring.str() << "[label=\"" <<"diepte: " << diepte << "\"]";
        uit << ";\n";

        string linkskind = this->geefKind(true).tekenrec(uit, knoopteller, diepte+1);
        string rechtskind = this->geefKind(true).tekenrec(uit, knoopteller, diepte+1);
        uit << wortelstring.str() << " -> " << linkskind << ";\n";
        uit << wortelstring.str() << " -> " << rechtskind << ";\n";
    };
    return wortelstring.str();

}
 string TrieBlad::tekenrec(ostream &uit, int &knoopteller, int diepte) {
    std::ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';

    uit << wortelstring.str() << "[label=\"" <<  letter << "\"]";

    return wortelstring.str();
}

#endif
