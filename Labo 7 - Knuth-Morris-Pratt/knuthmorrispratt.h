#define DEBUG 0
#include <queue>
#include <iostream>

using namespace std;

typedef unsigned int uint;
typedef unsigned char uchar;
class KnuthMorrisPratt
{
  public:
    KnuthMorrisPratt(const uchar *, uint);
    void zoek(std::queue<const uchar *> &plaats, const uchar *hooiberg, uint hooiberglengte);
    void printfailureTable() const;

  private:
    const uchar *naald;
    uint _naaldlengte;
    int *failureTable;
};

KnuthMorrisPratt::KnuthMorrisPratt(const uchar *_naald, uint __naaldlengte) : _naaldlengte(__naaldlengte), naald(_naald)
{
    /*
    *   Deze failuretabel wordt opgemaakt op basis van de prefix methode en geniet nog geen KMP optimalisatie
    *   Opmerking: volgende site maakt ook geen gebruik van deze optimalisatie: https://people.ok.ubc.ca/ylucet/DS/KnuthMorrisPratt.html 
    *
    *   Explenation: https://www.youtube.com/watch?v=KG44VoDtsAA
    */
    this->failureTable = new int[__naaldlengte];
    int j = 0;
    int i = 1;
    failureTable[0] = 0;

    while (i < _naaldlengte)
    {
#if DEBUG
        cout << i << " and " << _naaldlengte << endl
             << flush;
#endif
        if (naald[i] == naald[j])
        {
            failureTable[i] = (j + 1);
            i++;
            j++;
#if DEBUG
            cout << "\t" << naald[i] << "==" << naald[j] << endl
                 << flush;
            cout << "\t pushback(" << j + 1 << ")" << endl
                 << flush;
#endif
        }
        else
        {
            // YT part with decrementing J:
            // 1 - https://youtu.be/KG44VoDtsAA?t=138
            // 2 - https://youtu.be/KG44VoDtsAA?t=336

            // terugspringen zolang 1) naald[i] != naald[j]
            //                      2)        j != 0

            while (j > 0 && naald[i] != naald[j])
            {
#if DEBUG
                cout << "\t" << j << "   " << naald[i] << " <> " << naald[j] << endl
                     << flush;
#endif
                j = failureTable[j - 1];
            }
#if DEBUG
            cout << "\t" << j << "   " << naald[i] << " <> " << naald[j] << endl
                 << flush;
#endif
            // verwerken van j als we klaar zijn met het terugspringen wat gebeurde in while loop.
            if (naald[i] == naald[j])
            {
#if DEBUG
                cout << "\t pushback(" << j + 1 << ")" << endl
                     << flush;
#endif
                // beide waarden zijn gelijk. We doen dus hetzelfde als anders.
                failureTable[i] = (j + 1);
            }
            else
            {
#if DEBUG
                cout << "\t pushback(" << 0 << ")" << endl
                     << flush;
#endif
                // indien beiden niet hetzelfde zijn bevindt j zich op de start.
                failureTable[i] = 0;
            }
            i++;
            j++;
        }
    }
}

void KnuthMorrisPratt::printfailureTable() const
{
    for (int i = 0; i < _naaldlengte; i++)
    {
        cout << "\t" << naald[i];
    }
    cout << "\n";
    for (int i = 0; i < _naaldlengte; i++)
    {
        cout << "\t" << this->failureTable[i];
    }
    cout << endl;
};

void KnuthMorrisPratt::zoek(std::queue<const uchar *> &plaats, const uchar *hooiberg, uint hooiberglengte)
{
    /* i = counter in text
    *  j = counter in failuretable/naald
    *  als match: i++ en j++
    *  als mismatch: probeer j te decrementeren; indien -1, incrementeeer i.
    * 
    */

    int indexHooiberg = 0;
    int indexNaald = -1;
    while (indexHooiberg <= hooiberglengte)
    {
        while (naald[indexNaald + 1] == hooiberg[indexHooiberg])
        {
            indexNaald++;
            indexHooiberg++;
        };

        //indien de indexNaald de volledige failure tabel hebben doorlopen hebben we een match

        if (indexNaald == _naaldlengte - 1)
        {
            //slaan dit op
            plaats.push(&(hooiberg[indexHooiberg - _naaldlengte]));
            //reset indexNaald counter
            indexNaald = -1;
        }
        else
        {
            if (indexNaald == 0)
            {
                indexHooiberg++;
            }
            else
            {
                indexNaald = failureTable[indexNaald];
            }
        }
    }
}
