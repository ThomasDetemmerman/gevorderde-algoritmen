# Boyer-Moore

## status
- [x] Heurirstiek van het verkeerde karakter.
- [ ] Naïve methode.
- [ ] Vergelijken van aantal karaktervergelijkingen.
- [x] Heuristiek van het juiste suffix

## Implementatie details: verkeerde karakter
- Je houdt een MRP set bij die de grote is van uw alfabet. Op positie `char` wordt
de meest rechtse positie bijgehouden waar dat teken voorkomt in het patroon. Alle
andere waarden zijn -1. Characters zijn signed integers gaande van -128 tot 127, dus we doen elke char +128 en 
plaatsen ze in een vector van 255 plaatsen groot.

- Je doorloopt de tekst van rechts naar links maar tijdens het vergelijken loop je wel 
van links rechts naar links.

- Het aantal possities dat je moet opschuiven is als volgt te vinden
```cpp
naald.length()-1 - MRP[hooiberg[hooibergIndex] + 128]
````
dit werkt ook  indien het teken uit T niet in P voorkomt. Dan krijg je `p -1 - -1, waardoor 
we p opschuiven.
```cpp
aaaaaaaaaxaaaaaa -> aaaaaaaaaxaaaaaa 
      aaaa                    aaaa
```

## Implementatie details: juiste suffix

- Credits @ Martijn
- Deze oplossing is in O(p^2) en kan dus efficienter O(p)
- Deze oplossing kan nog beter door er een strong shift table van te maken
- aangezien dat een juist suffix altijd een possitieve verschuiving is moeten we niet meer controleren of het slechte karakter heuristiek een negatieve verschuiving heeft. We nemen toch max van beide
- Meer uitleg in de documentatie bij de code zelf.