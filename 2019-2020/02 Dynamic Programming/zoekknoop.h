template <class Sleutel, class Data>
class Zoekboom;

template <class Sleutel, class Data>
class zoekKnoop
{
    friend class Zoekboom<Sleutel, Data>;

public:
    zoekKnoop() : ouder(0) {}
    zoekKnoop(const Sleutel &sl, const Data &d) : sleutel{sl}, data(d), ouder(0){};
    zoekKnoop(Sleutel &&sl, Data &&d) : sleutel{move(sl)}, data(move(d)), ouder(0){};
    Zoekboom<Sleutel, Data> &geefKind(bool links);
    Sleutel sleutel;
    Data data;
    zoekKnoop<Sleutel, Data> *ouder;
    Zoekboom<Sleutel, Data> links, rechts;
};

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data> &zoekKnoop<Sleutel, Data>::geefKind(bool linkerkind)
{
    if (linkerkind)
        return links;
    else
        return rechts;
};