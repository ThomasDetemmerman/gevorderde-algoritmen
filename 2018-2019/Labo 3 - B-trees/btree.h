#ifndef BTREE_H
#define BTREE_H

#include "schijf.h"
#include <stack>
#include <iostream>

using std::stack;
using std::cerr;
using std::cout;
using std::endl;
using std::move;
using std::ostream;
using std::string;

// Dit bestand bevat code en hoofdingen van ...
template <class T, class D, unsigned int m>
class Btree;
template <class T, class D, unsigned int m>
class Bknoop;

// Betekenis m: zie cursus

template <class T, class D, unsigned int m>
class Bknoop
{
    friend class Btree<T, D, m>;

  public:
    Bknoop<T, D, m>()
    {
        int crashtest = 0;
        // temp is nodig om te verhinderen dat de compiler dit negeert
        int temp = (4 / (-4 + crashtest));
        crashtest++;
    };

    const Bknoop<T, D, m> &operator=(const Bknoop<T, D, m> &b)
    {
        k = b.k;
        isblad = b.isblad;

        for (unsigned int i = 0; i < k; i++)
        {
            sleutel[i] = b.sleutel[i];
            data[i] = b.data[i];
        }

        if (!isblad)
        {
            for (unsigned int i = 0; i <= k; i++)
            {
                index[i] = b.index[i];
            }
        }

        return *this;
    }

    T sleutel[m];
    D data[m];
    blokindex index[m + 1];
    unsigned int k;
    bool isblad;

  private:
    static int crashtest;
    void addKeyAndValue(T &sleutel, D &data);
    void addChildren(T& sleutel, blokindex& lChild, blokindex& rChild);

    void toString();

    string dotstring() const;
};

template <class T, class D, unsigned int m>
int Bknoop<T, D, m>::crashtest = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////         BTREE           /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T, class D, unsigned int m>
class Btree
{
    typedef Bknoop<T, D, m> Knoop;

  public:
    Btree(Schijf<Knoop> &s) : schijf(s)
    {
        wortel.k = 0;
        wortel.isblad = true;
        wortelindex = schijf.schrijf(wortel);
    };
    void voegToe(T sleutel, D data);
    bool zoek(T &sleutel, Bknoop<T, D, m> *zoekKnoop, blokindex &parent);


    void toString();

    void teken(const char *bestandsnaam) const;

private:
    Schijf<Knoop> &schijf;
    Knoop wortel;
    blokindex wortelindex;
    void splits(Knoop& huidig, stack<blokindex>& parents);

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////         Btree  : functies         ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
*   To Do: parameters referenties
*/
template <class T, class D, unsigned int m>
void Btree<T, D, m>::voegToe(T sleutel, D data)
{
    // Zoeken naar knoop waar de sleutel onder kan
    Knoop huidig = wortel;
    blokindex huidigeIndex = wortelindex;
    // top element is de huidige locatie op schijf
    stack<blokindex> parents; 
    parents.push(huidigeIndex);
    std::cout << "---ADDING " << sleutel << ", " << data << "!" << std::endl;
    // toevoegen van data gebeurd altijd in bladeren.
    while (!huidig.isblad)
    {
        int i = huidig.k - 1;
        std::cout << "i was " << i << ", with " << huidig.k << " keys in it ";

        while (i >= 0 && huidig.sleutel[i] > sleutel)
        {
            i--;
        }
        i++;

        int tmp;
        //std::cin >> tmp; //purpose?
        huidigeIndex = huidig.index[i];
        parents.push(huidigeIndex);
        // lees zal de eerste parameter overschrijven met de nieuwe Bknoop.
        schijf.lees(huidig, huidigeIndex);
    }
    // als we onze bknoop (een blad) gevonden hebben waar we de data verwachten
    // dan gaan we de sleutel en data hieraan toevoegen.
    huidig.addKeyAndValue(sleutel, data);
    // Commit changes
    schijf.herschrijf(huidig, huidigeIndex);
    // Update root in mem
    if (huidigeIndex == wortelindex)
    {
        wortel = huidig;
    }

    // Controle vol zitten
    // De Bknoop is vol. Nu is dat nog geen probleem maar als we de volgende keer zouden toevoegen wel.
    // Daarom gaan we dit nu al oplossen zodat k kleiner wordt.
    while (huidig.k == m)
    {
        std::cout << "splitsen" << std::endl;
        // als ons blad vol zit moeten we een sleutel met zijn data wegwerken.
        splits(huidig, parents);
    }
    std::cout << "done adding!" << std::endl;
};

template<class T, class D, unsigned int m>
void Btree<T, D, m>::splits(Knoop& huidig, stack<blokindex>& parents) {
    /*
            ▭ ▭ ▭                      ▭ ▭ ▭ ▭
                 \           ==>            | \
                  ▭▭▭▭▭                    ▭▭   ▭▭▭
                    ^                            ^
                 splits                         broer

    */
	// Knoop splitsen
	Knoop broer;
    // helft van de data kopiëren
    int midden = (int) m/2;
	for (int start = midden + 1; start < m; start++) {
		broer.sleutel[start - midden - 1] = huidig.sleutel[start];
		broer.data[start - midden - 1] = huidig.data[start];
	}
	broer.k = midden - 1;
	huidig.k = m - midden;
	broer.isblad = huidig.isblad;

    // we hebben alle data groter dan midden in de broer gezet
    // alle data kleiner dan midden laten staan.
    // midden zelf moet nu naar de parent. 
	T sleutel = huidig.sleutel[midden];
	D data = huidig.data[midden];
	
    // parent vinden we terug dankzij de stack
	blokindex lChild = parents.top(); 
    parents.pop();
	blokindex rChild = schijf.schrijf(broer);
	
	// Updaten van de originele huidige node
	schijf.herschrijf(huidig, lChild);
	
	if (parents.empty()) {
		huidig = *(new Knoop()); // Hergebruik huidige als nieuwe root
		huidig.isblad = false;
		huidig.addKeyAndValue(sleutel, data);
		huidig.addChildren(sleutel, lChild, rChild);
		wortelindex = schijf.schrijf(huidig);
		wortel = huidig;
	} else {
		blokindex huidigeIndex = parents.top(); parents.pop();
		schijf.lees(huidig, huidigeIndex);
		huidig.addKeyAndValue(sleutel, data);
		huidig.addChildren(sleutel, lChild, rChild);
		schijf.herschrijf(huidig, huidigeIndex);
		if (huidigeIndex == wortelindex) {
			wortel = huidig;
		}
	}
}

template<class T, class D, unsigned int m>
void Bknoop<T, D, m>::addChildren(T& sleutel, blokindex& lChild, blokindex& rChild) {
	int i = 0;
    // zoek de index van de sleutel
	while (this->sleutel[i] != sleutel && i < k) {
		i++;
	}
	index[i] = lChild;
	index[i+1] = rChild;
}

/*
*   zal op de gepaste plaats in de Bknoop waarvan de methode wordt opgeroepen de sleutel en data toevoegen.
*/
template <class T, class D, unsigned int m>
void Bknoop<T, D, m>::addKeyAndValue(T &sleutel, D &data)
{
    // k bevat de volgende positie in de rij waar de nieuwe data dient te komen
    this->sleutel[k] = sleutel;
    this->data[k] = data;
    // k uiteraard updaten voor later gebruik.
    this->k++;

    // Binnen een Bknoop is alle data gesorteerd.
    // Hieronder Insertionsort wat ideaal is voor sorteren van kleine datasets.
    // zie Algo I: Rangschikken door tussenvoegen
    for (int i = 1; i < k; i++)
    {   T hs; D hk; // geen idee waarom dit niet op een lijn kan met twee onderstaande lijnen.
        hs = move(this->sleutel[i]);
        hk = move(this->data[i]);
        int j = i - 1;
        while (j >= 0 && this->sleutel[j] > hs)
        {
            this->sleutel[j + 1] = move(this->sleutel[j]);
            this->data[j + 1] = move(this->data[j]);
            j--;
        }
        this->sleutel[j + 1] = move(hs);
        this->data[j+1] = move(hk);
    }
}

/*
*   is eerder bool exists()
*   return true als het bestaat, anders false.
*/
template <class T, class D, unsigned int m>
bool Btree<T, D, m>::zoek(T &sleutel, Bknoop<T, D, m> *zoekKnoop, blokindex &parentIndex)
{
    cout << "Zoek: " << sleutel << endl;
    int i = 0;
    // i < zoekKnoop->k                  //zolang er waarden zijn in de knoop
    // zoekKnoop->sleutel[i] < sleutel   //zolang de sleutel kleiner is als wat we zoeken
    while (i < zoekKnoop->k && zoekKnoop->sleutel[i] < sleutel)
    {
        i++;
    }
    // als i niet het einde van de rij bereikt heeft (anders error omdat i niet bestaat?)
    // en de sleutel wel gevonden hebben
    if (i < m && zoekKnoop->sleutel[i] == sleutel)
    {
        return true;
    }
    // indien we in een blad zitten kunnen we niet verder doen met de recursie.
    // een oplossing was niet gevonden.
    if (zoekKnoop->isblad)
    {
        return false;
    }
    // volgende data opvragen
    // schrijf.lees zal *zoekKnoop overschrijven met zoekKnoop->index[i]
    schijf.lees(*zoekKnoop, zoekKnoop->index[i]);
    // recursief verder zoeken.
    return zoek(sleutel, zoekKnoop, zoekKnoop->index[i]);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////        tekenfuncties             ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class T, class D, unsigned int m>
void Btree<T, D, m>::toString(){
    wortel.toString();
}

template <class T, class D, unsigned int m>
void Bknoop<T, D, m>::toString() {
    std::cout << "\t";
    for (int i = 0; i < m; ++i) {
        if(i < k) {
            std::cout << sleutel[i] << "\t";
        }
        else {
            std::cout << "[]\t";
        }
    }
    std::cout << std::endl;

    std::cout << "\t";
    for (int i = 0; i < m; ++i) {
        if(i < k) {
            std::cout << data[i] << "\t";
        }
        else {
            std::cout << "[]\t";
        }
    }
    std::cout << std::endl;
    std::cout << " ";
    for (int i = 0; i < m; ++i) {
        if(i < k) {
            std::cout << index[i] << "\t";
        }
        else {
            std::cout << "[]\t";
        }
    }
}

template <class T, class D, unsigned int m>
void Btree<T, D, m>::teken(const char * bestandsnaam) const{
    std::ofstream uit(bestandsnaam);
    //    ostream& uit=cerr;
    assert(uit);
    uit<<"digraph {\n";
    //    uit<<wortel.aantalSleutels<<"\n";
    //    uit<<wortel.isBlad<<"\n";
    //noot: beetje ineffici"ent: knopen worden twee keer ingelezen
    std::queue<std::pair<blokindex,string>> BEq;//queue voor breedte eerst;
    if (!wortel.isblad)
        BEq.emplace(wortelindex,wortel.dotstring());
    uit<<"\""<<wortel.dotstring()<<"\" [shape=rectangle] [label=\""<<wortel.dotstring()<<"\"];\n";
    while (!BEq.empty()){
        Knoop ouder;
        schijf.lees(ouder,BEq.front().first);
        string& ouderstring=BEq.front().second;
        for (int i=0; i<=ouder.k; i++ ){
            Knoop kind;
            schijf.lees(kind,ouder.index[i]);
            string kindstring=kind.dotstring();
            uit<<"\""<<kindstring<<"\" [shape=rectangle] [label=\""<<kindstring<<"\"];\n";
            uit<<"\""<<ouderstring<<"\" -> \""<<kindstring<<"\";\n";
            if (!kind.isblad){
                BEq.emplace(ouder.index[i],kindstring);
            };
        }
        BEq.pop();
    };

    uit<<"}\n";
}

template<class T, class D,unsigned int m>
string Bknoop<T,D,m>::dotstring() const{
    std::ostringstream os;
//noot: voor debugdoelen is het vaak handig de indexen van de kinderen uit te schrijven
//vervang dan volgende lijn door
//        if (!isBlad){
    if (false){
        os<<"niet-blad: ";
        os<<" ("<<index[0]<<")";
        for (int i=1; i<=k; i++ ){
            os<<sleutel[i]<<"("<<index[i]<<") ";
        }
    }
    else{
        os<<"  ";
        for (int i=0; i<k; i++ ){
            os<<sleutel[i]<<"  ";
        }
    }
    return os.str();
}

#endif
