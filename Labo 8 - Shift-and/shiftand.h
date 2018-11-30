#include "bitpatroon.h"
#include <queue>
typedef unsigned int uint;
typedef unsigned char uchar;
class Shiftand
{
  public:
    //naaldlengte moet kleiner dan patroonlengte zijn,
    //anders wordt een exceptie opgegooid.
    Shiftand(const uchar *naald, uint _naaldlengte);
    void zoek(std::queue<const uchar *> &plaats,
              const uchar *hooiberg, uint hooiberglengte);

  private:
    //Bitpatroon letterpatroon[256];
    Bitpatroon letterpatroon;
    uint naaldlengte;
};

Shiftand::Shiftand(const uchar *naald, uint _naaldlengte) : naaldlengte(_naaldlengte)
{
    Bitpatroon a((uint)naald);
    letterpatroon = a;
};

void Shiftand::zoek(std::queue<const uchar *> &plaats, const uchar *hooiberg, uint hooiberglengte)
{
    // Tekst
    Bitpatroon T((uint)hooiberg);
    //Rj+1[0]

    Bitpatroon R;
    int j = 0;
    if (letterpatroon.getBit(0) == T.getBit(j + 1))
    {
        //waar als P[0] = T[j + 1]
        // waar = 1;
        R.shiftlinks(1);
    }
    else
    {
        //niet waar als P[0] =/= T[j + 1]
        R.shiftlinks(1);
    }

    for (int i = 0; i < naaldlengte; i++)
    {
        //waar als Rj [i − 1] waar is en P[i] = T[j + 1]
        if (letterpatroon.getBit(i) == T.getBit(j + 1))
        {
            R.shiftlinks(1);
        }
        else
        {
            R.shiftlinks(1);
        }
    }
}
