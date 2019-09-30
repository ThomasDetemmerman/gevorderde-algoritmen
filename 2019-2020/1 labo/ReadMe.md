# Zoekbomen

- [x] constructoren  
  moeilijk hierbij was een pointer naar zoekknoop om te zetten naar een zoekboom. Dit gebeurt als volgt:
```cpp
  Zoekboom(unique_ptr<zoekKnoop<Sleutel, Data>> &&k) : unique_ptr<zoekKnoop<Sleutel, Data>>(move(k)){}; //move constructor
```
- [x] geefDiepte
- [x] rotatiefunctie
- [x] maakOnevenwichtig
  - [x] werkt op lege boom
- [ ] maakEvenwichtig
  - [ ] werkt op lege boom
  - [ ] zie intranet

# lambda functies
```cpp
void Zoekboom<Sleutel, Data>::inorder(std::function<void(const zoekKnoop<Sleutel, Data> &)> bezoek) const
```

Een lambda functie kan je meegeven als volgt.

```cpp
bool Zoekboom<Sleutel, Data>::repOK() const{


    const Sleutel * vorige = nullptr; // we gebruiken een pointer zodat het ook voor Strings werkt
    bool boomIsOk = true;
    // Wil je iets gebruiken in uw lamda dat je van buiteaf nodig hebt? dan moet je deze definieren in [ ]. Dit heten captures.
    inorder([&vorige, &boomIsOk](const zoekKnoop<Sleutel, Data> &b){
        // Sectie 1: Is in order
        if(vorige && boomIsOk){
            boomIsOk = (b.sleutel >= *vorige);
        }
        vorige = &b.sleutel;

        // Sectie 2: parent pointers controleren
        if(b.links && boomIsOk){
            boomIsOk = b.links->ouder == &b;
        }
        if(b.rechts && boomIsOk){
            boomIsOk = b.rechts->ouder == &b;
        }
    });
    return boomIsOk;
}
```
Wil je iets gebruiken in uw lamda dat je van buiteaf nodig hebt? dan moet je deze definieren in [ ]. Dit heten captures.

Ook voor de controle van parentpointers moeten we alle knopen overlopen. We kunnen dus nog eens inorder oproepen. Twee keer inorder overlopen zou 2n zijn. 
Daarom gaan we 'parentpointer' en 'inorder' gelijktijdig overlopen.
   

