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
