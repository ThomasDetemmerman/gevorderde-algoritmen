#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>

class DynamicProgramming {

private:

public:

    void vulMatrix(int e, int v, std::vector<std::vector<int>>& eggdrops) {

        // Randgevallen
        for (int i = 0; i < v; i++) eggdrops[1][i] = i;
        for (int i = 0; i < v; i++) eggdrops[0][i] = 0;
        for (int i = 0; i < e; i++) eggdrops[i][1] = 1;
        for (int i = 0; i < e; i++) eggdrops[i][0] = 0;

        // Matrix opvullen
        // Start bij 2 eieren want 0 en 1 hebben we al opgevuld
        for (int i = 2; i < e; i++) {
            for (int j = 2; j < v; j++) {

                std::cout << i << " - " << j << std::endl;

                // Bij aanvang is het minimum oneindig groot
                int minimum = std::numeric_limits<int>::max();


                // Nu overlopen we alle mogelijke startpunten en
                // zoeken we het startpunt met de kleinste waarde
                for (int x = 0; x <= j; x++) {

                    // ALs het ei breekt, neem je de waarde uit de matrix bij 1 ei minder
                    // aangezien je al 1x geworpen hebt op punt x moet je x-1 nemen
                    // zodat x + (x-1) = x
                    int eiBreekt = eggdrops[i-1][x-1];



                    // Als het ei niet breekt, neem je de waarde uit de matrix bij hetzelfde aantal eieren
                    // Maar je weet dat het ei niet breekt op de eerste x verdiepingen dus kijk je bij het
                    // v-x
                    int eiBreektNiet = eggdrops[i][j-x];


                    // De waarde die gekozen wordt voor deze x is het maximum van eiBreekt en eiBreekt niet
                    // We hadden al 1x geworpen om te zien of het ei breekt op x of niet
                    // dus moeten we de waarde + 1 doen
                    int gekozenWaarde = std::max(eiBreekt, eiBreektNiet) + 1;


                    std::cout << "gekozen: " << eiBreekt << std::endl;

                    // Check of gekozenWaarde het nieuwe minimum is
                    minimum = std::min(minimum, gekozenWaarde);

                }

                // Vul het gevonden minimum in
                eggdrops[i][j] = minimum;

            }
        }
    }

    void printMatrix(const int e, const int v, std::vector<std::vector<int>> eggdrops) {
        for (int i = 1; i < e; i++) {
            for (int j = 1; j < v; j++) {
                std::cout << eggdrops[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    }

};

int main() {

    int eieren = 36;
    int verdiepingen = 20;

    std::vector<std::vector<int>> matrix(eieren);
    for (int i = 0; i < eieren; i++) {
        matrix[i] = std::vector<int>(verdiepingen);
    }

    std::cout << "Vector aangemaakt" << std::endl;

    DynamicProgramming dp;

    dp.vulMatrix(eieren, verdiepingen, matrix);

    std::cout << "Klaar met vullen van matrix 2" << std::endl;

    dp.printMatrix(eieren, verdiepingen, matrix);

    return 0;
}
