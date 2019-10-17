

template <class Sleutel, class Data>
class BinomialTree;

template <class Sleutel, class Data>
class zoekKnoop
{
    friend class BinomialTree<Sleutel, Data>;

public:
    zoekKnoop() {};
    zoekKnoop(const Sleutel &sl, const Data &d) : prioriteit{sl}, data(d){};   //copy constructor
    zoekKnoop(Sleutel &&sl, Data &&d) : prioriteit{move(sl)}, data(move(d)){}; // move constructor
    Sleutel prioriteit;
    Data data;
    BinomialTree<Sleutel, Data> kind, broer;

};

