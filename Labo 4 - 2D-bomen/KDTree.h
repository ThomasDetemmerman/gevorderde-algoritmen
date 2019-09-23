#include "knoop.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include <cassert>
#include <functional>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

template <unsigned int k>
// KDTree is in feite gewoon een pointer naar een knoop.
// we maken gebruik van smartpointers (unique_ptr) want dit is best practise om memory leaks te voorkomen.
class KDTree : public unique_ptr<KDKnoop<k>>
{
public:
    using unique_ptr<KDKnoop<k>>::unique_ptr;
    KDTree(){};
    /* 
        Makes it possible to cast a unique_ptr<KDNode<k>> to a KDTree<k>
        zoals op de lijn met code  KDTree<k> newSubTree = make_unique<KDKnoop<k>>(point, ((dim%k)+1));
    */
    KDTree(unique_ptr<KDKnoop<k>> &&u) : unique_ptr<KDKnoop<k>>(move(u)){};
    
    void add(Point<k> &point);
    void draw(const char *filename);

private:
    string drawRecursive(ostream &out, int &counter);
};

template <unsigned int k>
void KDTree<k>::add(Point<k> &point)
{
    //als er al een wortel bestaat
    if (*this)
    {
        KDTree<k>* currentTree = this;
        KDKnoop<k>* currentKnoop = 0;
        unsigned int currentDim;

        // zoeken naar waar de knoop hoort
        while (*currentTree)
        {
            currentKnoop = currentTree->get();
            currentDim = currentKnoop->getDimension();
            if (point.coordinates[currentDim] < currentKnoop->punt.coordinates[currentDim])
            {

                currentTree = &(currentKnoop->links);   //& is nodig vanwege het sterretje in  '''KDTree<k>* currentTree = this;'''
            }
            else
            {
                currentTree = &(currentKnoop->rechts);
            }
        }

        // de knoop nu ook toevoegen
        KDTree<k> newSubTree = make_unique<KDKnoop<k>>(point, ((currentDim % k) + 1));
    }
    else
    {
        cout << "making new tree\n";
        // er is nog geen wortel
        KDTree<k> newTree = make_unique<KDKnoop<k>>(point, 0);
        cout << "new tree created\n";
        (*this) = move(newTree);
        
    }
};

/*
    Code uit oplossing.
    Teken de KDboom
*/
template <unsigned int k>
void KDTree<k>::draw(const char *filename)
{

    ofstream out(filename);
    assert(out);

    int counter = 0;

    out << "digraph {\n";
    this->drawRecursive(out, counter);
    out << "}";
};
template <unsigned int k>
string KDTree<k>::drawRecursive(ostream &out, int &counter)
{

    ostringstream content;

    if (!*this)
    {
        content << "null" << ++counter;
        out << content.str() << " [shape=point];\n";
    }
    else
    {
        content << '"' << this->get()->punt << '"';
        out << content.str() << "[label=\"" << this->get()->punt << "\"]";
        out << ";\n";

        string left_child = this->get()->links.drawRecursive(out, counter);
        string right_child = this->get()->rechts.drawRecursive(out, counter);

        out << content.str() << " -> " << left_child << ";\n";
        out << content.str() << " -> " << right_child << ";\n";
    };

    return content.str();
};