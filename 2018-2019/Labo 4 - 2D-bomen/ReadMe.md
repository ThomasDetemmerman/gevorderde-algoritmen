# Kd bomen
Er is ook nog een readme in folder OLD2

## stap 1
Maak klasse voor de kleinste bouwsteen van het algoritme. Namelijk een punt. deze klasse is gegeven.

## stap 2
De grotere bouwsteen zijn knopen.  
__vereisten:__
- [x] verwijzing naar links
- [x] verwijzing naar rechts
- [x] sleutel (coördinaten)
- [x] dimensie (kdboom test per niveau op exact 1 dimensie)
- [ ] data (niet nodig voor deze labo)

## stap 3
Nu moeten we een klasse KDTree schrijven die deze KDKnopen kan verzamelen tot een boom en hier bewerkingen op kan doen.  
__functies:__
- [x] toevoegen
- [x] zoeken
- [ ] vindtInDeBuurt


# To Do
lijn 40 & 41: 
waarom
```
KDTree<k>* currentTree = this;
```
en niet 
```
KDTree<k> currentTree = this;
```