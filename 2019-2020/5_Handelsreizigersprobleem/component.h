//
// Created by Thomas on 24/10/2019.
//
#include <set>
#ifndef INC_5_HANDELSREIZIGERSPROBLEEM_COMPONENT_H
#define INC_5_HANDELSREIZIGERSPROBLEEM_COMPONENT_H


class component : public std::set<int> {
private:
    int _verantwoordelijke;
public:

    int knoopID;

    component(std::set<int> leden): _verantwoordelijke(*(leden.begin())), std::set<int>(leden){};
    bool contains(int lid) const{
       return (this->find(lid) != this->end());
    }

    int getVerantwoordelijke() const{
        return _verantwoordelijke;
    }
};


#endif //INC_5_HANDELSREIZIGERSPROBLEEM_COMPONENT_H
