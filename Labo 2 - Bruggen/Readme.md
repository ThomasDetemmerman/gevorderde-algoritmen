# structuur *.dat file

```
3
1 1
1 5
2 3
```
-   3 => aantal records die het bestand bevat
-   lijnnummer => ID arena
-   eerste kolom => smeergeld
-   tweede kolom => ID winkelcentrum

# Toevoegen van bruggen

- Je begint met de uiterst linkse arena en zijn bijbehorende brug. De verzameling is leeg en dus geen conflicten.
- Je wilt brug n toevoegen
    - n kruist met n-1
        - controleer voor n-2
    - n kruist niet met n-1
        - gewoon toevoegen.

*Belangrijke eigenschap*: 
Indien brug n niet kruist met n-1 zal deze ook niet kruisen met alle andere bruggen waarmee n-1 ook niet kruist!


# Implementatie

Ik maak gebruik van twee datastructuren. 
-  Een boom zoals deze wel vaker wordt gebruikt bij dynamisch programmeren. De oplossingsverzameling is dan het pad naar de leave met de grootste hoeveelheid smeergeld. 
    -  elke knoop wijst naar zijn parent.
    -  Theoretisch gezien kan er gebruik gemaakt worden van een viruele root. In deze oplossing wordt er gebruik gemaakt van een bos.
-  Een vector die alle leaves van de boom bijhoud om gemakkelijk toegang te krijgen tot deze knopen.
    -  Indien een leaf een nieuw kind krijgt wordt deze vector geupdate
    -  Indien een knoop een tweede kind krijgt krijgen we een extra leaf en voegen we deze toe aan de vector


