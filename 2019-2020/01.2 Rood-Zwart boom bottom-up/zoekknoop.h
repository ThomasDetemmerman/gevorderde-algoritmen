#include "kleur.h"

template <class Sleutel, class Data>
class Zoekboom;

template <class Sleutel, class Data>
class zoekKnoop
{
    friend class Zoekboom<Sleutel, Data>;

public:
    // Uit de theorie weten we dat als we een nieuwe knoop toevoegen deze rood is omdat we dan de zwarte hoogte
    // niet verstoren en deze repareren is moeilijker. Daarom zal de default contstructor de knoop rood kleuren.
    zoekKnoop() : ouder(0), kleur(Kleur::Rood) {}
    zoekKnoop(const Sleutel &sl, const Data &d) : sleutel{sl}, data(d), ouder(0){};   //copy constructor
    zoekKnoop(Sleutel &&sl, Data &&d) : sleutel{move(sl)}, data(move(d)), ouder(0){}; // move constructor
    Zoekboom<Sleutel, Data> &geefKind(bool links);
    Sleutel sleutel;
    Data data;
    zoekKnoop<Sleutel, Data> *ouder;
    Zoekboom<Sleutel, Data> links, rechts;
    Kleur kleur;
};

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data> &zoekKnoop<Sleutel, Data>::geefKind(bool linkerkind)
{
    if (linkerkind)
        return links;
    else
        return rechts;
};