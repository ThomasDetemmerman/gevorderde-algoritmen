#include "bitpatroon.h"
#include <queue>
typedef unsigned int uint;
typedef unsigned char uchar;
class Shiftand{
public:
//naaldlengte moet kleiner dan patroonlengte zijn,
//anders wordt een exceptie opgegooid.
    Shiftand(const uchar* naald,uint _naaldlengte);
    void zoek(std::queue<const uchar*>& plaats,
                     const uchar* hooiberg, uint hooiberglengte);
private:
    Bitpatroon letterpatroon[256];
    uint naaldlengte;
};
    
