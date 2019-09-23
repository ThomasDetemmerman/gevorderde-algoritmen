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
    /* 
    *  https://youtu.be/cH-5KcgUcOE?t=207 
    * 
    *  i = counter in text
    *  j = counter in failuretable/naald
    *  als match: i++ en j++
    *  als mismatch: probeer j te decrementeren; indien -1, incrementeeer i.
    * 
    */

    if(hooiberglengte == 0){
        return;
    }

    int indexHooiberg = 0;
    int indexNaald = 0;
    while (indexHooiberg < hooiberglengte)
    {
        // we blijven naald en hooiberg vergelijken totdat er een mismatch is.
        while (naald[indexNaald] == hooiberg[indexHooiberg])
        {
            indexNaald++;
            indexHooiberg++;
        };

        //indien de indexNaald de volledige failure tabel hebben doorlopen hebben we een match
        if (indexNaald == _naaldlengte)
        {
            //slaan dit op
            plaats.push(&(hooiberg[indexHooiberg - _naaldlengte]));
            // stel dat naald ABAB is. ALs we een match gevonden hebben in de tekst staat indexNaald op 4. 
            // We mogen de naald niet reseten maar op 2 zetten. De failuretable heeft deze info.
            // https://youtu.be/cH-5KcgUcOE?t=256
            indexNaald = failureTable[indexNaald-1 ];
        }
        // mismatch after j matches 
		else if (indexHooiberg < hooiberglengte && naald[indexNaald] != hooiberg[indexHooiberg]) { 
			// Do not match lps[0..lps[j-1]] characters, 
			// they will match anyway 
			if (indexNaald != 0) 
				indexNaald = failureTable[indexNaald - 1]; 
			else
				indexHooiberg++; 
		} 
    }
}
