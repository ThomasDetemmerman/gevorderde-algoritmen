#include <cassert>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
//Deze header bevat de code voor de klassen:
//
//
//    Graaf<RichtType>: basisklasse met grafen zonder tak- of knoopdata
//    GraafMetTakdata<RichtType,TakDatatype>
//    GraafMetKnoopdata<RichtType,KnoopDatatype>
//    GraafMetKnoopEnTakdata<RichtType,KnoopDatatype,Takdatatype>
//
//
//Evenals de bijbehorende klasse van GraafExcepties.
/**********************************************************************

   Class: Graaf

   beschrijving: Graaf is de basisklasse voor een hi�rarchie van ijlegraafklassen.
                  Deze hi�rarchie zal klassen bevatten voor grafen met data behorend bij
                  knopen en/of takken. Deze data worden opgeslagen in vectoren,
                  zodat kan worden opgezocht op index.
        Ad knopen:      Deze zijn opeenvolgend genummerd. Knopen kunnen niet individueel verwijderd worden.
                        Wel kunnen alle knopen samen verwijderd worden (door wis()), en kunnen knopen
                        ��n voor ��n toegevoegd worden.
        Ad verbindingen: Deze kunnen naar hartenlust toegevoegd en verwijderd worden. Als een
                        verbinding wordt verwijderd, wordt het nummer op de stack van
                        vrijgekomenVerbindingsnummers gezet. Worden verbindingen toegevoegd,
                        dan worden eerst deze vrijgekomen nummers opnieuw gebruikt, alvorens nieuwe nummers
                        worden toegevoegd. hoogsteVerbindingsnummer kan nooit verkleinen, en de
                        vector met takdata zal altijd hoogsteVerbindingsnummer elementen bevatten.

        Tekenen met GraphViz: etiketten mogen, bij uitschrijven geen aanhalingsteken bevatten.
                        code is nog niet grondig getest.

***************************************************************************/
/**********************************************************************

   Class: GraafMetTakdata<RichtType, Takdata>

   beschrijving: Deelklasse van vorigen, met data bij elke tak.

***************************************************************************/

#ifndef __GRAAF_H
#define __GRAAF_H


#include <stdexcept>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <stack>
#include <exception>



enum RichtType { GERICHT, ONGERICHT };


class GraafExceptie : public std::logic_error {
public:
    GraafExceptie(const std::string &boodschap_) : std::logic_error(boodschap_) {}
};

std::ostream &operator<<(std::ostream &os, const GraafExceptie& exc){
    return os << exc.what();
}


template<RichtType RT>
class Graaf{
public:
    typedef std::map<int, int>  Burenlijst;      // beeldt knoopnummer (van buur) af op verbindingsnummer

    // Construeert een graaf met gegeven RichtType en aantal knopen (default 0), zonder verbindingen.
    Graaf(int n=0);
    // Geeft true indien het richttype GERICHT is, false indien het ONGERICHT is.
    bool isGericht() const;

    // Voegt een nieuwe 'lege' knoop toe, d.w.z. zonder verbindingen.
    // Geeft knoopnummer van toegevoegde knoop terug (begint bij 0).
    virtual int voegKnoopToe();

    // Voegt verbinding toe tussen knoopnummers 'van' en 'naar'.
    // Gooit GraafExceptie indien verbinding al bestaat of knoopnummers ongeldig zijn.
    // Geeft verbindingsnummer van toegevoegde verbinding terug (begint bij 0).
    // Bij ongerichte graaf wordt terugverbinding ook toegevoegd (met zelfde verbindingnummer!)
    virtual int voegVerbindingToe(int van, int naar);

    // Verwijdert de verbinding tussen knoopnummers 'van' en 'naar', incl. de terugverbinding indien ongericht.
    // Gooit GraafExceptie indien knoopnummers ongeldig zijn.
    // Geen exceptie indien de verbinding niet bestond.
    // Andere verbindingen worden niet 'hernummerd'. Er komen dus mogelijks "gaten" in de verbindingsnummers.
    virtual void verwijderVerbinding(int van, int naar);

    // Geeft het aantal knopen van de graaf.
    int aantalKnopen() const;

    // Geeft het aantal verbindingen van de graaf.
    // Bij ongerichte graaf worden verbindingen NIET dubbel geteld!
    int aantalVerbindingen() const;

    // Geeft het verbindingsnummer van de verbinding tussen 'van' en 'naar'.
    // Geeft -1 indien die verbinding niet bestaat.
    // Gooit een GraafExceptie indien knoopnummers ongeldig zijn.
    // Opgelet: performantie is O(log(v)) waarbij v aantal verbindingen vanuit 'van' is.
    int verbindingsnummer(int van, int naar) const;

    // Verwijdert alle knopen en verbindingen en herstart de verbindingsnummer
    virtual void wis();

    // Toegang tot de burenlijsten:
    const Burenlijst &operator[](int i) const { return burenlijsten[i]; }
    Burenlijst &operator[](int i)       { return burenlijsten[i]; }  // deze kan als lvalue gebruikt worden


    // Schrijft de gegevens van de volledige graaf naar outputstream os.
    virtual void schrijf(std::ostream &os) const;

    virtual void teken(const char* bestandsnaam) const;
    virtual std::string knooplabel(int i) const{
        std::ostringstream uit;
        uit<<i;
        return uit.str();
    };
    virtual std::string taklabel(int i) const{return "";};
    // Schrijft de gegevens van de knoop met knoopnummer k naar outputstream os.
    virtual void schrijfKnoop(std::ostream &os, int k) const;

    // Schrijft de gegevens van de verbinding met verbindingsnummer v naar outputstream os.
    virtual void schrijfVerbinding(std::ostream &os, int v) const;


protected:
    // hulpfuncties
    void controleerKnoopnummer(int k) const;   // throw indien k ongeldig
    void voegVerbindingToeInDatastructuur(int van, int naar,int verbindingsnummer);
    void verwijderVerbindingUitDatastructuur(int van, int naar);
protected:
    //datavelden
    std::vector<Burenlijst>  burenlijsten;
    int                 hoogsteVerbindingsnummer;
    RichtType           richttype;
    std::stack<int>     vrijgekomenVerbindingsnummers;
};


template<RichtType RT>
std::ostream &operator<<(std::ostream& os, const Graaf<RT>& g);


// --- implementatie ---


template<RichtType RT>
void Graaf<RT>::controleerKnoopnummer(int k) const{
    if (k<0 || (size_t)k>=burenlijsten.size()){
        std::ostringstream out;
        out << "Ongeldig knoopnummer "<<k
            <<". Moet >= 0 en < "<<burenlijsten.size()<<" zijn.";
        throw GraafExceptie(out.str());
    }
}

template<RichtType RT>
Graaf<RT>::Graaf(int n) : burenlijsten(n), hoogsteVerbindingsnummer(0){};


template<RichtType RT>
bool Graaf<RT>::isGericht() const { return true; }//voor gerichte graaf

template<>
bool Graaf<ONGERICHT>::isGericht() const { return false; }//voor ongerichte graaf


template<RichtType RT>
int Graaf<RT>::voegKnoopToe(){
    int n = burenlijsten.size();
    burenlijsten.resize(n+1); // default constructor voor nieuwe knoop wordt opgeroepen (hier lege map)
    return n;
}


template<RichtType RT>
int Graaf<RT>::voegVerbindingToe(int van, int naar){
    controleerKnoopnummer(van);
    controleerKnoopnummer(naar);

    if (burenlijsten[van].count(naar) > 0)
    {
        std::ostringstream out;
        out << "verbinding " << van << "-" << naar << " bestaat al";
        throw GraafExceptie(out.str());
    }
    else {
        int verbindingsnummer;
        if (!vrijgekomenVerbindingsnummers.empty()){
            verbindingsnummer=vrijgekomenVerbindingsnummers.top();
            vrijgekomenVerbindingsnummers.pop();
        }else
            verbindingsnummer=hoogsteVerbindingsnummer++;
        voegVerbindingToeInDatastructuur(van,naar,verbindingsnummer);
        return verbindingsnummer;
    }
}

template<RichtType RT>//voor gerichte graaf
void Graaf<RT>::voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer){
    burenlijsten[van][naar] = verbindingsnummer;
}

template<>
void Graaf<ONGERICHT>::voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer){
    burenlijsten[van][naar] = verbindingsnummer;
    burenlijsten[naar][van] = verbindingsnummer;
}

template<RichtType RT>
void Graaf<RT>::verwijderVerbinding(int van, int naar){
    controleerKnoopnummer(van);
    controleerKnoopnummer(naar);
    if (burenlijsten[van].find(naar)!=burenlijsten[van].end()){//verbinding bestaat
        vrijgekomenVerbindingsnummers.push(burenlijsten[van][naar]);
        verwijderVerbindingUitDatastructuur(van,naar);
    }
    // (geen exception indien verbinding niet bestaat)
}

template<RichtType RT>
void Graaf<RT>::verwijderVerbindingUitDatastructuur(int van, int naar){
    burenlijsten[van].erase(naar);
}

template<>
void Graaf<ONGERICHT>::verwijderVerbindingUitDatastructuur(int van, int naar){
    burenlijsten[van].erase(naar);
    burenlijsten[naar].erase(van);
}

template<RichtType RT>
int Graaf<RT>::aantalKnopen() const { return burenlijsten.size(); }

template<RichtType RT>
int Graaf<RT>::aantalVerbindingen() const {
    return hoogsteVerbindingsnummer-vrijgekomenVerbindingsnummers.size();
}

template<RichtType RT>
int Graaf<RT>::verbindingsnummer(int van, int naar) const{
    controleerKnoopnummer(van);
    controleerKnoopnummer(naar);
    Burenlijst::const_iterator verbindingit = burenlijsten[van].find(naar);

    if (verbindingit == burenlijsten[van].end())
        return -1;
    else
        return (*verbindingit).second;
}

template<RichtType RT>
void Graaf<RT>::wis(){
    burenlijsten.clear();
    hoogsteVerbindingsnummer = 0;
    while (!vrijgekomenVerbindingsnummers.empty())
        vrijgekomenVerbindingsnummers.pop();
}

template<RichtType RT>
void Graaf<RT>::schrijf(std::ostream &os) const{
    os << "Graaf: " << aantalKnopen() << " knopen en "
       << aantalVerbindingen() << " verbindingen:" << std::endl;
    for (int k=0; k<aantalKnopen(); k++)
        schrijfKnoop(os, k);
}

template<RichtType RT>
void Graaf<RT>::teken(const char* bestandsnaam) const{
    std::ofstream uit(bestandsnaam);
    assert(uit);
    std::string pijl;
    if (isGericht()){
        uit<<"digraph {\n";
        pijl="->";
    }
    else{
        uit<<"graph {\n";
        pijl="--";
    };
    for (int k=0; k<aantalKnopen(); k++){
        if (burenlijsten[k].empty())
            uit<<knooplabel(k)<<";\n";
        else{
            for (auto& [buur,tak]: this->burenlijsten[k]){
//C++-14-versie:
//        for (auto& p: this->burenlijsten[k]){
//              auto& buur=p.first;
//              auto& tak=p.second;
                if (isGericht() || buur >= k)
                    uit<<knooplabel(k)<<" "<<pijl<<" "
                       <<knooplabel(buur)<<taklabel(tak)<<";\n";
            };
        };
    };
    uit<<"}";
    uit.close();
};


template<RichtType RT>
void Graaf<RT>::schrijfKnoop(std::ostream &os, int k) const{
    os<<"Burenlijst "<<k<<"::\n";
    for (auto& [buur,tak]: this->burenlijsten[k]){
//C++-14-versie:
//        for (auto& p: this->burenlijsten[k]){
//              auto& buur=p.first;
//              auto& tak=p.second;
        os << "  ->" << buur;
        schrijfVerbinding(os, tak);
    }
}

template<RichtType RT>
void Graaf<RT>::schrijfVerbinding(std::ostream &os, int v) const{
    os << " via verbinding nr. " << v << std::endl;
}

template<RichtType RT>
std::ostream &operator<<(std::ostream &os, const Graaf<RT> &g){
    g.schrijf(os);
    return os;
}

template<RichtType RT,class Takdata>
class GraafMetTakdata: public virtual Graaf<RT>{
public:
    GraafMetTakdata(int n=0):Graaf<RT>(n){};
    //Noot: toevoegfunctie zonder takdata op te geven kan alleen gebruikt als de klasse
    //      Takdata een defaultconstructor heeft.
    virtual int voegVerbindingToe(int van, int naar);
    virtual int voegVerbindingToe(int van, int naar, const Takdata&);
    //Noot: verwijderVerbinding wordt ongewijzigd overgenomen van Graaf!

    //TakData vrijgeven (eventueel voor wijziging). Geeft nullpointer als tak niet bestaat
    //Noot: pointers teruggegeven door geefTakdata worden ongeldig
    //door toevoegen van een tak.
    const Takdata* geefTakdata(int van,int naar) const;
    Takdata* geefTakdata(int van,int naar)      ;
    virtual std::string taklabel(int i) const{
        std::ostringstream uit;
        uit<<"[label=\""<<takdatavector[i]<<"\"]";
        return uit.str();
    };

    virtual void wis();


    // Schrijft de gegevens van de verbinding met verbindingsnummer v naar outputstream os.
    virtual void schrijfVerbinding(std::ostream &os, int v) const;

protected:
    std::vector<Takdata> takdatavector;
};

template<RichtType RT,class Takdata>
int GraafMetTakdata<RT,Takdata>::voegVerbindingToe(int van, int naar){
    return this->voegVerbindingToe(van,naar,Takdata());
}


template<RichtType RT,class Takdata>
int GraafMetTakdata<RT,Takdata>::voegVerbindingToe(int van, int naar, const Takdata& td){
    bool isnieuwtaknummer=this->vrijgekomenVerbindingsnummers.empty();
    int taknummer=Graaf<RT>::voegVerbindingToe(van,naar);
    if (isnieuwtaknummer)
        takdatavector.push_back(td);
    else
        takdatavector[taknummer]=td;
    return taknummer;
}



template<RichtType RT,class Takdata>
const Takdata* GraafMetTakdata<RT,Takdata>::geefTakdata(int van,int naar) const{
    int taknummer=this->verbindingsnummer(van,naar);
    if (taknummer!=-1)
        return &takdatavector[taknummer];
    else
        return 0;
}

template<RichtType RT,class Takdata>
Takdata* GraafMetTakdata<RT,Takdata>::geefTakdata(int van,int naar){
    int taknummer=this->verbindingsnummer(van,naar);
    if (taknummer!=-1)
        return &takdatavector[taknummer];
    else
        return 0;
}


template<RichtType RT,class Takdata>
void GraafMetTakdata<RT,Takdata>::wis(){
    Graaf<RT>::wis();
    takdatavector.clear();
}

template<RichtType RT,class Takdata>
void  GraafMetTakdata<RT,Takdata>::schrijfVerbinding(std::ostream &os, int v) const{
    os << " via verbinding nr. " << v <<" (Data: "<<takdatavector[v]<<")"<< std::endl;
}


template<RichtType RT, class Knoopdata>
class GraafMetKnoopdata:public virtual Graaf<RT>{
public:
    // Construeert een graaf met gegeven RichtType, en lijst van Knoopdata;
    template<class InputIterator>
    GraafMetKnoopdata(InputIterator start,InputIterator eind);
    GraafMetKnoopdata():Graaf<RT>(){};

    virtual int voegKnoopToe();
    virtual int voegKnoopToe(const Knoopdata&);

    const Knoopdata* geefKnoopdata(int knoopnr) const;
    Knoopdata* geefKnoopdata(int knoopnr)      ;
    virtual std::string knooplabel(int i) const{
        std::ostringstream uit;
        uit<<'"'<<i<<":"<<knoopdatavector[i]<<'"';
        return uit.str();
    };

    virtual void wis();
    virtual void schrijfKnoop(std::ostream &os, int k) const;

protected:
    //datavelden
    std::vector<Knoopdata>  knoopdatavector;
};
template<RichtType RT, class Knoopdata>
template<class InputIterator>
GraafMetKnoopdata<RT,Knoopdata>::GraafMetKnoopdata(InputIterator start,InputIterator eind)
        :Graaf<RT>(0){
    for(;start!=eind;start++)
        voegKnoopToe(*start);
}

template<RichtType RT, class Knoopdata>
int GraafMetKnoopdata<RT,Knoopdata>::voegKnoopToe(){
    return this->voegKnoopToe(Knoopdata());
}

template<RichtType RT, class Knoopdata>
int GraafMetKnoopdata<RT,Knoopdata>::voegKnoopToe(const Knoopdata& kd){
    int ret=Graaf<RT>::voegKnoopToe();
    knoopdatavector.push_back(kd);
    return ret;
}

template<RichtType RT,class Knoopdata>
const Knoopdata* GraafMetKnoopdata<RT,Knoopdata>::geefKnoopdata(int knoopnummer) const{
    this->controleerKnoopnummer(knoopnummer);
    return &knoopdatavector[knoopnummer];
}

template<RichtType RT,class Knoopdata>
Knoopdata* GraafMetKnoopdata<RT,Knoopdata>::geefKnoopdata(int knoopnummer){
    this->controleerKnoopnummer(knoopnummer);
    return &knoopdatavector[knoopnummer];
}


template<RichtType RT,class Knoopdata>
void GraafMetKnoopdata<RT,Knoopdata>::wis(){
    Graaf<RT>::wis();
    knoopdatavector.clear();
}


template<RichtType RT, class Knoopdata>
void GraafMetKnoopdata<RT,Knoopdata>::schrijfKnoop(std::ostream &os, int k) const{
    os << "knoop " << k <<"(Data: "<<knoopdatavector[k]<< "):" << std::endl;
    for (auto& [buur,tak]: this->burenlijsten[k]){
//C++-14-versie:
//        for (auto& p: this->burenlijsten[k]){
//              auto& buur=p.first;
//              auto& tak=p.second;
        os << "  ->" << buur;
        this->schrijfVerbinding(os, tak);
    }
}


template<RichtType RT, class Knoopdata, class Takdata>
class GraafMetKnoopEnTakdata:public GraafMetKnoopdata<RT,Knoopdata>,
                             public GraafMetTakdata<RT, Takdata>{
public:
    template<class InputIterator>
    GraafMetKnoopEnTakdata(InputIterator start,InputIterator eind):
            GraafMetKnoopdata<RT,Knoopdata>(start,eind){};
    GraafMetKnoopEnTakdata():
            GraafMetKnoopdata<RT,Knoopdata>(){};
    virtual void wis(){
        GraafMetKnoopdata<RT,Knoopdata>::wis();
        this->takdatavector.clear();
    }
};

#endif // __GRAAF_H
