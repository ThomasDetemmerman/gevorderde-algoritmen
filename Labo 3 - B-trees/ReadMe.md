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
```


