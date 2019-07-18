#include <stdlib.h>
#include <stdio.h>

using namespace std;

template <unsigned int k>
class KDTree;

// k staat voor het aantal dimenties.
template <unsigned int k>
class KDKnoop {
    private:
        KDTree<k> links;
        KDTree<k> rechts;
        Point<k> punt;
        unsigned int depth; //om te weten op welke dimensie we moeten testen.
        //Data data; //we moeten geen extra data opslaan bij het punt voor deze opgave

    public:
        friend class KDTree<k>; //kdtree moet aan private attributen kunnen

        /* constructor */
        KDKnoop(Point<k> &p, unsigned int d) : punt(p), depth(d) {}; // een constructor om de knoop aan te maken met een initilizer list
        unsigned int getDimension();


};

template <unsigned int k>
unsigned int KDKnoop<k>::getDimension(){
    return this->depth % k;
};