#include "knoop.h"

using namespace std;

template <unsigned int k>
// KDTree is in feite gewoon een pointer naar een knoop.
// we maken gebruik van smartpointers (unique_ptr) want dit is best practise om memory leaks te voorkomen.
class KDTree : public unique_ptr<KDNode<k>>
{
public:
    using unique_ptr<KDNode<k>>::unique_ptr;
    KDTree(){};
    /* 
        Makes it possible to cast a unique_ptr<KDNode<k>> to a KDTree<k>
        zoals op de lijn met code  KDTree<k> newSubTree = make_unique<KDKnoop<k>>(point, ((dim%k)+1));
    */
    KDTree(unique_ptr<KDNode<k>> &&u) : unique_ptr<KDNode<k>>(move(u)){};

    void add(Point<k> &point)
    {
        //als er al een wortel bestaat
        if (*this)
        {
            KDTree<k> currentTree = this;
            KDKnoop<k> currentKnoop = 0;
            unsigned int currentDim;

            // zoeken naar waar de knoop hoort
            while (*current)
            {
                currentKnoop = currentTree->get();
                currentDim = currentKnoop.getDimension();
                if (point.coordinates[dim] < currentKnoop->punt.coordinates[dim])
                {
                    currentTree = currentKnoop->left;
                }
                else
                {
                    currentTree = currentKnoop->right;
                }
            }

            // de knoop nu ook toevoegen
            KDTree<k> newSubTree = make_unique<KDKnoop<k>>(point, ((dim % k) + 1));
        }
        else
        {
            // er is nog geen wortel
            KDTree<k> newTree = make_unique<KDKnoop<k>>(point, 0);
            (*this) = move(newTree);
        }
    }
};
