# Splaybomen

Een splayboom in een uitgebreidere implementatie van zoekbomen. Splayboom zal dus van zoekbomen overerven.


# Considerations

## Zoeken

``Splayboom<Sleutel, Data>::zoek(...) `` zal gewoon ``Zoekboom<Sleutel,Data>::zoek(..)`` implementeren gevolgd door een ``splayoperatie(...)``

## Splay

Onderstaande private huklp functies zijn verschoven geweest naar de grote functie ``play()``. Splay verwacht namelijk
een zoekboom als parameter. Zig, Zigzig en zigzag zijn echter operaties op een splayboom. Converteren
tussen beide bomen was te veel werk en lukte niet. Vandaar deze oplossing.

Indien er toch met hulpfuncties zig, zigzig en zigzag gewerkt wenst te worden kan je de knoop meegeven als parametern.
```cpp

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::zig(bool naarRechts)
{
    (*this).roteer(naarRechts);
};

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::zigzag(bool naarRechts)
{
    //(*this)->geefKind(naarRechts); // dit kan enkel indien je zig zou programmeren in de zoekboom zelf
    std::cout << (*this)->sleutel;
    (*this)->geefKind(naarRechts).roteer(!naarRechts);
    std::cout << (*this)->geefKind(naarRechts)->sleutel;
    zig(naarRechts);
};

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::zigzig(bool naarRechts)
{
    //Zoekboom<Sleutel, Data> *p = &((*this)->geefKind(naarRechts));//betekenis P zie cursus pagina 18

    zig(naarRechts);
    zig(naarRechts);
}

```

