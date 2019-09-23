# Shift-AND methode
[Extra uitleg en visualisatie](http://alvaro-videla.com/2014/01/shift-and-visualization.html)
## build bit mask. 

Voor het patroon "ABD" is dit dan 
```
A: 100000000 
B: 010000000 
C: 000000000 
D: 001000000 
E: 000000000 
... 
```

Merk op:  
- In dit geval is de maximum patroonlengte "000000000".length() 
- De tabel geeft een rij voor elk mogelijk teken. Of deze nu in p zit of niet. 
- A,B,C worden niet effectief opgeslagen. Ze worden gerepresenteerd door hun numerieke representatie. 

 

## searching for the pattern 
### Code
Documentatie en commentaar bevindt zich in het programma zelf (shiftand.h)
. Hoe het algoritme werkt zie log (hieronder).

```cpp
void Shiftand::zoek(std::queue<const uchar*>& resultaten, const uchar* hooiberg, uint hooiberg_lengte)
{
   
    Bitpatroon huidig;

    for (int i = 0; i < hooiberg_lengte; i++)
    {   
       
        huidig = huidig.shiftrechts(1);
        huidig |= Bitpatroon::eenbit(0);
        huidig &= letterpatroon[hooiberg[i]];
           
        if (huidig.en(Bitpatroon::eenbit(naaldlengte - 1)))
        {
            resultaten.push(hooiberg + i);
        }

    } 
}
```
### log 
Hoe de code werkt wordt uitgelegt a.d.h.v. de log. Deze spreekt voor zichzelf.
```
huidig:                                        00000000000000000000000000000000
huidig.shiftrechts(1):                         00000000000000000000000000000000
huidig |= Bitpatroon::eenbit(0):               10000000000000000000000000000000
letterpatroon[hooiberg[i]]:                    10010000000000000000000000000000
huidig &= letterpatroon[hooiberg[i]]:          10000000000000000000000000000000

checking result
huidig:                                        10000000000000000000000000000000
Bitpatroon::eenbit(naaldlengte - 1)            00010000000000000000000000000000
huidig.en(Bitpatroon::eenbit(naaldlengte - 1))  0       this doesn't equal. No matches

huidig:                                        10000000000000000000000000000000
huidig.shiftrechts(1):                         01000000000000000000000000000000
huidig |= Bitpatroon::eenbit(0):               11000000000000000000000000000000
letterpatroon[hooiberg[i]]:                    01100000000000000000000000000000
huidig &= letterpatroon[hooiberg[i]]:          01000000000000000000000000000000

checking result
huidig:                                        01000000000000000000000000000000
Bitpatroon::eenbit(naaldlengte - 1)            00010000000000000000000000000000
huidig.en(Bitpatroon::eenbit(naaldlengte - 1)) 0       this doesn't equal. No matches

huidig:                                        01000000000000000000000000000000
huidig.shiftrechts(1):                         00100000000000000000000000000000
huidig |= Bitpatroon::eenbit(0):               10100000000000000000000000000000
letterpatroon[hooiberg[i]]:                    01100000000000000000000000000000
huidig &= letterpatroon[hooiberg[i]]:          00100000000000000000000000000000

checking result
huidig:                                        00100000000000000000000000000000
Bitpatroon::eenbit(naaldlengte - 1)            00010000000000000000000000000000
huidig.en(Bitpatroon::eenbit(naaldlengte - 1)) 0       this doesn't equal. No matches

huidig:                                        00100000000000000000000000000000
huidig.shiftrechts(1):                         00010000000000000000000000000000
huidig |= Bitpatroon::eenbit(0):               10010000000000000000000000000000
letterpatroon[hooiberg[i]]:                    10010000000000000000000000000000
huidig &= letterpatroon[hooiberg[i]]:          10010000000000000000000000000000

checking result
huidig:                                        10010000000000000000000000000000
Bitpatroon::eenbit(naaldlengte - 1)            00010000000000000000000000000000
huidig.en(Bitpatroon::eenbit(naaldlengte - 1)) 1       this equals

result found:abba
```

