#include <bitset>
using std::bitset;
template <int T>
class DigitaleZoekboom;

template <int T>
class zoekKnoop
{
    friend class DigitaleZoekboom<T>;

public:
    zoekKnoop() {}
    zoekKnoop(std::unique_ptr<zoekKnoop<T>> && ptr) {
        *this = move(ptr);
    }
    zoekKnoop& operator=(std::unique_ptr<zoekKnoop<T>> && ptr) {
        *this = move(ptr);
    }

    zoekKnoop(bitset<T> patroon) : bitpatroon(patroon){};
    DigitaleZoekboom<T> &geefKind(bool links);
    std::bitset<8> bitpatroon;
    DigitaleZoekboom<T> links, rechts;
    bool isBlad(){
        return (links == nullptr && rechts == nullptr);
    }
};

template <int T>
DigitaleZoekboom<T> &zoekKnoop<T>::geefKind(bool linkerkind)
{
    if (linkerkind)
        return links;
    else
        return rechts;
};