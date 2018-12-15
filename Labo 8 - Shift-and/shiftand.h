#include "bitpatroon.h"
#include <queue>
#include <sstream>
#include <iomanip>
#include <iostream>

typedef unsigned int uint;
typedef unsigned char uchar;
class Shiftand{
public:
//naaldlengte moet kleiner dan patroonlengte zijn,
//anders wordt een exceptie opgegooid.
    Shiftand(const uchar* naald,uint _naaldlengte);
    void zoek(std::queue<const uchar*>& plaats, const uchar* hooiberg, uint hooiberglengte);
    std::string to_string() const;

private:
    Bitpatroon letterpatroon[256];
    const uchar* naald;
    uint naaldlengte;
};

Shiftand::Shiftand(const uchar* naald, uint naald_lengte) : naald{naald}, naaldlengte{naald_lengte}
{
    if (naald_lengte > patroonlengte)
    {
        throw "De naald lengte is te groot";
    }

    /*
     stel ons woord is "aaaab". en i is 0
     naald[i] = heeft dus b terug.
     b komt overeen met een nummer hier (hypothetisch) 1.
     in onze vector letterpatroon wordt dus op letterpatroon[naald[i]] vertaald naar letterpatroon[1].
     Op deze positie bevindt zich een bitpatroon bestaande uit nulletjes. 
     
     eenbit(i=1) doet achter de schermen: patroonlengte-1-i. Als gevolg wordt dat hier 10000 (zie intranet).
    
     Nu gaan we de |-operator toepassen
                000000
                100000
                -------
                100000

    */
    for (int i = 0; i < naald_lengte; i++)
    {
        std::cout << naald[i];
        letterpatroon[naald[i]] |= Bitpatroon::eenbit(i);
        
    }
}

void Shiftand::zoek(std::queue<const uchar*>& resultaten, const uchar* hooiberg, uint hooiberg_lengte)
{
   
    //http://alvaro-videla.com/2014/01/shift-and-visualization.html
    Bitpatroon huidig;

    for (int i = 0; i < hooiberg_lengte; i++)
    {   
        // 000001
        huidig = huidig.shiftrechts(1);

        // 00001
        // 10000
        // ------
        // 10001
        huidig |= Bitpatroon::eenbit(0);

        huidig &= letterpatroon[hooiberg[i]];

            // huidig == 00001
        if (huidig.en(Bitpatroon::eenbit(naaldlengte - 1)))
        {
            resultaten.push(hooiberg + i);
        }
    }

   
}


// code assistent
std::string Shiftand::to_string() const
{
    const int LOWER_READABLE_BOUND = 32;
    const int UPPER_READABLE_BOUND = 126;

    std::stringstream out;

    out << "Naald: ";
    for (int i = 0; i < naaldlengte; i++)
    {
        out << naald[i];
    }
    out << std::endl << "Naaldlengte:" << naaldlengte << std::endl << std::endl;

    out << std::setw(6) << "";
    for (int i = 0; i < naaldlengte; i++)
    {
        out << static_cast<char>(naald[i]);
    }
    out << std::endl;

    for (int i = LOWER_READABLE_BOUND; i <= UPPER_READABLE_BOUND; i++)
    {
        out << std::setw(6) << std::left << static_cast<char>(i) << letterpatroon[i] << std::endl;
    }

    return out.str();
}