#include "rzwboom14.h"

RZWboom<int> maakBoom(int sl)
{
    RZWboom<int> nieuweboom(new RZWknoop<int>(sl));
    return nieuweboom;
}

RZWboom<int> *voeglinkstoe(RZWboom<int> &root, int sl)
{
    RZWboom<int> nieuweboom(new RZWknoop<int>(sl));
    root->links = move(nieuweboom);
    return &(root->links); //temp, fout
}

int main()
{

    RZWboom<int> zkbm = maakBoom(5);
    RZWboom<int> *zkbmlinks = voeglinkstoe(zkbm, 1);

    voeglinkstoe(*zkbmlinks, 0);
    zkbm.tekenAlsBinaireBoom("test.dot");
}
