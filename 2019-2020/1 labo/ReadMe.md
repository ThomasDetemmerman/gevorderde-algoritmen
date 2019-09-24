# Zoekbomen

- [x] constructoren  
  moeilijk hierbij was een pointer naar zoekknoop om te zetten naar een zoekboom. Dit gebeurt als volgt:
  ```cpp
  Zoekboom(unique_ptr<zoekKnoop<Sleutel, Data>> &&k) : unique_ptr<zoekKnoop<Sleutel, Data>>(move(k)){}; //move constructor
- [x] geefDiepte
- [x] rotatiefunctie
- [x] maakOnevenwichtig
  - [x] werkt op lege boom
- [ ] maakEvenwichtig
  - [ ] werkt op lege boom
  - [ ] zie intranet
