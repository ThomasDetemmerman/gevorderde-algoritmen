# Splaybomen

Een splayboom in een uitgebreidere implementatie van zoekbomen. Splayboom zal dus van zoekbomen overerven.

## main1
Bevat een eenvoudige binaire boom voor test doeleinden. Is gebruikt om splayboom.h te implementeren en te testen.

### Voorbeeld: zoek(20)
Input:
![Input image](https://github.ugent.be/thdtemme/Gevorderde-Algoritmen/blob/master/2019-2020/2_labo/1_input.dot.png)
Output:
![Output image](https://github.ugent.be/thdtemme/Gevorderde-Algoritmen/blob/master/2019-2020/2_labo/1_output2.dot.png)

## main2
Leest Shakespeare of AChristmasCarol in.

## main3
Leest een document in in twee tabellen en probeert met dynamisch programmeren een optimale boom te bekomen. 
Dit is niet afgewerkt. https://www.geeksforgeeks.org/optimal-binary-search-tree-dp-24/

# Considerations bij main1

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

