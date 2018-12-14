#ifndef __STROOMNET_H
#define __STROOMNET_H
#include "graaf.h"
#include <cassert>
#include <iostream>
#include <functional>
using std::function;
using std::ostream;
using std::vector;

template <class T>
class Stroomnetwerk;

template <class T>
class Pad : public std::vector<int>
{
  public:
    T geefCapaciteit() const
    {
        return capaciteit;
    }
    void zetCapaciteit(const T &_capaciteit)
    {
        capaciteit = _capaciteit;
    }
    friend ostream &operator<<(ostream &os, const Pad &p)
    {
        os << "Capaciteit= " << p.capaciteit << " :: ";
        if (p.size() > 0)
        {
            os << p[0];
        }
        for (int i = 1; i < p.size(); i++)
        {
            os << "->" << p[i];
        }
        os << "\n";
    }

  protected:
    T capaciteit;
};
/**********************************************************************

   Class:VergrotendPad
   
   beschrijving: Methodeklasse die, gegeven een stroomnetwerk,
                 een vergrotend pad teruggeeft.
                 Vermits ze hoort bij Ford-Fulkerson zal een object van deze klasse
                 elke keer een ander pad geven als het stroomnetwerk opgegeven
                 in de constructor verandert.

   
***************************************************************************/
template <class T>
class Vergrotendpadzoeker
{
  public:
    Vergrotendpadzoeker(const Stroomnetwerk<T> &stroomnetwerk) : q(stroomnetwerk), v(stroomnetwerk.van), v2(stroomnetwerk.naar),
                                                                 l(q.aantalKnopen()), m(q.aantalKnopen(), false){};
    Pad<T> geefVergrotendPad();

  protected:
    virtual void foo(int t, int x, Pad<T> &p);
    const Stroomnetwerk<T> &q;
    vector<int> l;
    vector<bool> m;
    int v, v2;

  private:
    void voegStroomToe(int van, int naar, int stroom);
};

template <class T>
Pad<T> Vergrotendpadzoeker<T>::geefVergrotendPad()
{
    for (int i = 0; i < m.size(); i++)
    {
        m[i] = false;
    }
    Pad<T> p;
    assert(v != v2);
    foo(v, 0, p);
    assert(p.size() != 1);
    if (p.size() > 1)
    {
        T g = *q.geefTakdata(p[0], p[1]);
        for (int i = 2; i < p.size(); i++)
        {
            T ychg = *q.geefTakdata(p[i - 1], p[i]);
            if (ychg < g)
                g = ychg;
        }
        p.zetCapaciteit(g);
    }
    return p;
}

template <class T>
void Vergrotendpadzoeker<T>::foo(int t, int x, Pad<T> &p)
{
    m[t] = true;
    const typename GraafMetTakdata<GERICHT, T>::Burenlijst &a = q[t];
    int ychx = x + 1;
    for (typename GraafMetTakdata<GERICHT, T>::Burenlijst::const_iterator it = a.begin();
         it != a.end(); it++)
    {
        int u = it->first;
        if (*q.geefTakdata(t, u) > 0)
        {
            if (it->first == v2 && ychx + 1 > p.size())
            {
                l[v2] = t;
                p.resize(ychx + 1);
                int ychf = v2;
                int i = ychx;
                while (ychf != v)
                {
                    p[i--] = ychf;
                    ychf = l[ychf];
                }
                p[0] = ychf;
            }
            else if (u != v2 && !m[u])
            {
                l[u] = t;
                foo(u, ychx, p);
            }
        }
    }
}

/**********************************************************************

   Class: Stroomnetwerk
   
   beschrijving: Een stroomnetwerk gaat uit van een gewogen gerichte graaf
                 die in de constructor wordt opgegeven
   
***************************************************************************/

template <class T> //T = takdata
class Stroomnetwerk : public GraafMetTakdata<GERICHT, T>
{
  public:
    //leeg netwerk; alleen aantal knopen en van en naar gegeven.
    Stroomnetwerk(int grootte, int _van, int _naar) : Graaf<GERICHT>(grootte), GraafMetTakdata<GERICHT, T>(grootte), van(_van), naar(_naar){};
    //Copyconstructor. Let op: Graaf<GERICHT>(gr) moet toegevoegd,
    //anders roept de copyconstructor van GraafMetTakdata de defaultconstructor
    //van Graaf op en krijgen we een lege graaf.
    Stroomnetwerk(const GraafMetTakdata<GERICHT, T> &gr, int _van, int _naar) : Graaf<GERICHT>(gr), GraafMetTakdata<GERICHT, T>(gr), van(_van), naar(_naar){};
    Stroomnetwerk(const Stroomnetwerk<T> &gr) : Graaf<GERICHT>(gr), GraafMetTakdata<GERICHT, T>(gr), van(gr.van), naar(gr.naar){};
    Stroomnetwerk(Stroomnetwerk<T> &&gr) : Stroomnetwerk(0, 0, 0)
    {
        swap(this->burenlijsten, gr.burenlijsten);
        swap(this->vrijgekomenVerbindingsnummers, gr.vrijgekomenVerbindingsnummers);
        std::swap(this->hoogsteVerbindingsnummer, gr.hoogsteVerbindingsnummer);
        swap(this->takdatavector, gr.takdatavector);
        std::swap(this->van, gr.van);
        std::swap(this->naar, gr.naar);
    };

    Stroomnetwerk<T> geefStroom()
    {
        Stroomnetwerk<T> oplossing(this->aantalKnopen(), van, naar);
        Stroomnetwerk<T> restnetwerk(*this);
        Vergrotendpadzoeker<T> vg(restnetwerk);
        Pad<T> vergrotendpad = vg.geefVergrotendPad();
        while (vergrotendpad.size() != 0)
        {   
            //uitleg martijn (assistent)
            // += en -= hebben niets met elkaar te maken. Het een is voor het stroomnetwerk (enkel aanpassen takdata), het
             // ander voor het restnetwerk (aanpassen takdata en verbindingen). Je kan eventueel ook andere functienamen
            // gebruiken zoals updateStroomnetwerk en updateRestnetwerk
            //restnetwerk -= vergrotendpad;
            //oplossing += vergrotendpad;
            updateRestnetwerk(restnetwerk, vergrotendpad);
            updateStroomnetwerk(*this, vergrotendpad);

            vergrotendpad = vg.geefVergrotendPad();
        }
        return oplossing;
    }

    void vergrootTak(int start, int eind, T delta)
    {
        int taknr = this->verbindingsnummer(start, eind);
        if (taknr == -1)
            taknr = this->voegVerbindingToe(start, eind, delta);
        else
            this->takdatavector[taknr] += delta;
    }
    T geefCapaciteit()
    {
        T som = 0;
        for (typename GraafMetTakdata<GERICHT, T>::Burenlijst::const_iterator it = this->burenlijsten[van].begin();
             it != this->burenlijsten[van].end(); it++)
            som += this->takdatavector[it->second];
        return som;
    }

    int van, naar;

  protected:
    virtual std::string knooplabel(int i) const
    {
        std::ostringstream uit;
        if (i == van)
            uit << '"' << i << ":P\"";
        else if (i == naar)
            uit << '"' << i << ":V\"";
        else
            uit << i;
        return uit.str();
    };

///////////// eigen

///////////// alternatief (mijn voorkeur)

template <class T>
void Stroomnetwerk<T>::updateStroomnetwerk(Stroomnetwerk<T>& stroomnetwerk, const Pad<T>& vergrotendpad)
{
    if (vergrotendpad.empty())
    {
        return;
    }

    for (int i = 1; i < vergrotendpad.size(); i++)
    {
        // vergrotend pad is een vector met alle knoop ID's in volgorde zoals deze in onze graaf doorlopen dient te worden.
        // verbinding 0-1; daarna 1-2...
        int van = vergrotendpad[i - 1];
        int naar = vergrotendpad[i];

        // elke pad heeft een capaciteit. Als deze bijvoorbeeld y is dan moet elke huidige capaciteit van elke verbinding op dat pad met y verhogen.
        T toe_te_voegen_stroom = vergrotendpad.geef_capaciteit();

        // Indien er een heen verbinden en een terugverbinding is gaat onze voorkeur (blijkbaar) altijd naar de terug verbinding
        // maar als deze er niet is kunneen we de gewone heenverbinding gewoon updaten.
        if (this->verbindingsnummer(naar, van) == -1)
        {
            stroomnetwerk.voegStroomToe(van, naar, toe_te_voegen_stroom);
        }
        else // er is dus wel een terugverbinding
        {
            // wat is de capaciteit van deze terugstroom.
            T* terugstroom = stroomnetwerk.geefTakdata(naar, van);

            // Uit de theorie weten we dat als we een pad in tegenstelde zin doorlopen we de nieuwe capaciteit moeten aftrekken van de oude. (Normaal tellen we deze erbij op)
            // Dit kan uiteraard enkel als de terugstroom groot genoeg is. We willen geen negatieve capaciteit
            if (*terugstroom >= toe_te_voegen_stroom)
            {
                *terugstroom -= toe_te_voegen_stroom;
            }
            // indien blijkt dat de terugstroom niet groot genoeg is om ervan afgetrokken te worden, dan:
            // probeer zoveel mogelijk via de terugstroom te laten gaan.
            // en het resterende via de heenstroom.
            // vb. toe_te_voegen_stroom = 10
            //     terugstroom = 4
            //     nieuwe terugstroom zou -6 zijn. Dus wat doen we:
            //     terugstroom = 0
            //     heenstroom = 6
            else
            {
                toe_te_voegen_stroom -= *terugstroom;
                *terugstroom = 0;
                stroomnetwerk.voegStroomToe(van, naar, toe_te_voegen_stroom);
            }
        }
    }
}


template <class T>
void Stroomnetwerk<T>::voegStroomToe(int van, int naar, int stroom)
{
    if (this->verbindingsnummer(van, naar) == -1)
    {
        this->voegVerbindingToe(van, naar, stroom);
    }
    else
    {
        *(this->geefTakdata(van, naar)) += stroom;
    }
}


//////////// Oplossing conform Cnobs, niet nagekeken.
/*
    Stroomnetwerk &operator-=(Pad<T> &p)
    {
        for (int i = 1; i < p.size(); i++)
        {
            int van = p[i - 1];
            int naar = p[i];
            int *td = this->geefTakdata(van, naar);
            *td -= p.geefCapaciteit();
            if (this->verbindingsnummer(naar, van) == -1)
            {
                this->voegVerbindingToe(naar, van, 0);
            }
            td = this->geefTakdata(naar, van);
            *td += p.geefCapaciteit();
        }
        return *this;
    }

    Stroomnetwerk &operator+=(Pad<T> &p)
    {
        for (int i = 1; i < p.size(); i++)
        {
            int van = p[i - 1];
            int naar = p[i];
            if (this->verbindingsnummer(van, naar) == -1)
            {
                this->voegVerbindingToe(van, naar, 0);
            }
            int *td = this->geefTakdata(van, naar);
            *td += p.geefCapaciteit();
            if (this->verbindingsnummer(naar, van) == -1)
            {
                this->voegVerbindingToe(naar, van, 0);
            }
            td = this->geefTakdata(naar, van);
            *td -= p.geefCapaciteit();
            if (*td <= 0)
            {
                // todo
                //verwijderVerbindingUitDatastructuur(naar, van);
            }
        }
        return *this;
    }
*/



};
//////////// einde eigen

#endif
