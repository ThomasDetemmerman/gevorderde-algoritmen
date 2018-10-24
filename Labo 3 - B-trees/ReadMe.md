## strikvraag

```c
template<class T,class D,unsigned int m>
int Knoop<T,D,m>::crashtest=0;
```
Dit is statische variabele die je instelt op een bepaalde waarde. Maar telkens dat de 
classe wordt geïnstantieerd wilt c++ daar een andere waarde aan geven maar dat kan niet
want het is statisch en zou dus maar 1 keer gedeclareerd worden.
C++ lost dit op door gewoon de functie buiten de classe te zetten.

Wat is de opgave: Knoop-klasse bestaat niet dus je lost dit op door er dit van te maken.

```c
template<class T,class D,unsigned int m>
int Bknoop<T,D,m>::crashtest=0;
    ^
```

## Aanpassing 2
Iets van dat je compilers tegenwoordig niet gebruikte code wegsmijten. De chrashtest mag niet weg voor enige reden dus voeg je volgende toe op lijn 19 van originele code
```c
 int crashtest = 0;
```

## VoegToe()
In mijn aanpak zal de Btree()::voegtoe() een een deelboom zoeken en desnoods toevoegen om vervolgens Bknoop::voegtoe() de effectieve waarde in de array toe te voegen.

## Iterator
We gaan een uitbreiding doen op de labo-oefening. Om de 20 meest gekozen woorden te vinden kunnen we best een iterator schrijven.
Hoe we dat moeten doen hebben we niet gezien en gaan we niet zien. Hier beetje help.

Gebruik van een iterator
```c++
for(Bree<>::iterator=bt.begin(); it != bt.end(); it++){
    ...*it...
    ...it->?...
}
```
Geneste klassen. Zo kan bree iterator aan alles van btree classe. I.p.v. die `friends` te gebruiken zoals Cnops kunnen we dus geneste klassen gebruiken. Daarom dat de oplossing van de assistent er anders uitziet als deze van cnops. Hij heeft deze classes uit elkaar gehaald.

operator++ zal moeilijkste zijn omdat je moet 'springen' in je boom van kind naar parent enzo. `Stack` is een mogelijk goede oplossing.
```c++
Template<...>
class Btree::iterator{
    iterator& operator++();
    T* operator*();
    bool operator==();
    bool operator!();
};
```
```c++
Template<...>
classBTree{
    class iterator;
    iterator begin();
    iterator end();
};
```

## usefull links
https://www.cs.usfca.edu/~galles/visualization/BTree.html
https://github.ugent.be/mvdpol/algo2/blob/master/BBoom/main.cpp


## BKnoop <class T, class D, unsigned int m>
```
m = 3
k = 2

T sleutel[m];
+-----------+-----------+-----------+-----------+
|  0     |     1     |     2     |           |
+-----------+-----------+-----------+-----------+

D data[m];
+-----------+-----------+-----------+-----------+
|  A        |   B       |   C       |   D       |
+-----------+-----------+-----------+-----------+


```

## Known issues
line 199 throws floating point exception