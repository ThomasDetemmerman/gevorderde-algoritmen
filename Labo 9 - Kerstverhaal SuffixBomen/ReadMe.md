# SuffixBoom - Kerstmis editie
Requires C++17
## Status

- [x] Toevoegen van verhaal aan suffix boom.
      Opgelet: we voegen alle teksten toe als 1 grote string. Het idee hierachter is als volgt: als je een woord toevoegd vind je de langst gemeenschappelijke suffix binnen dat woord. Maar nu is dit suffix een volledige zin.
- [ ] LGD vinden.
- [ ] Extra: gelijkenisgraaf


## Parameters recursieve functie

Heeft een in en out parameter voor elke langere deelstring dat hij tegenkomt. De functie moet ook weten wat de gezochte teksten zijn.  

Bitpatroon `gezocht` mag een ptr zijn maar maakt niet echt uit.   

Sluitmap kan omgezet worden in een bitpatroon.  

```
geefRec(string & in, const Bitpatroon &, gezocht, const vector<> sluitmap &)
```

Voor een blad hebben we voldoende maar voor knoop onderweg kunnen we nog geen kind opvragen, we breiden dus uit.
We willen van een knoop zijn kinderen kunnen opvragen met para `knoopverzameling`.
```
geefRec(string & in, const Bitpatroon & gezocht, const vector<> sluitmap &, Bitpatroon & knoopversameling)
```