# Galadiner (Ford Fulkerson)

 - De `+=operator()` en `-=operator()` moesten aagevuld worden `stroomnet.h`
 - Ondanks dat deze klasse de oplossing is van Cnops, zit er een fout in (met return types).

## Werking

De `-=operator()` wordt opgeroepen in de context van `restnetwerk-=vergrotendpad;`
Hierbij moeten we dus het vergrotendpad verwijderen uit het restnetwerk en terugverbindingen aanmaken met terug stroom. Tussen twee knopen kunnen dus twee verbindingen voorkomen.

- heentak.cap = nieuwecap
  - verwijder heentak
  - voeg terugtak toe
- heentak.cap =/= nieuwecap
  - update: heentak.cap -= nieuwecap
  - update terugtak of voeg toe 

De `+=operator()`  wordt opgeroepen in de context van `oplossing+=vergrotendpad;`. We moeten dus deze  oplossing gewoon toevoegen aan de oplossingsgraaf. Eventuele verbindingen die al bestaan in omgekeerde richting moeten ongedaan gemaakt worden of gereduceerd. Tussen twee kopen komt maar 1 verbinding voor.

- terugtak bestaat && nieuwecapaciteit > oudecapaciteit 
  - verwijder terugtak
  - nieuwe heentak met cap = (nieuwecap - oudecap)
- terugtak bestaat  && nieuwecap < oudecap
  - update: oudecap -= nieuwecap
- terugtak bestaat && nieuwecap = oudecap
  - verwijder terugtak
- terugtak bestaat niet
  - voeg heentak toe
