#include <queue>
typedef unsigned int uint;
typedef unsigned char uchar;
class KnuthMorrisPratt{
public:
    KnuthMorrisPratt(const uchar* naald,uint _naaldlengte);
    void zoek(std::queue<const uchar*>& plaats,
                     const uchar* hooiberg, uint hooiberglengte);
};
    
