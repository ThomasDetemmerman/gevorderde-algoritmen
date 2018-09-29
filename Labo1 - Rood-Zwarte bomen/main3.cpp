#include "rzwboom14.h"
#include "vector"
#include "iostream"

using std::vector;
using std::cout;

int main(){
    const vector<int> primes({97,101,103,107,109,113,181,191,193,173,179,2,3,5,7,11,13,17,19,23,29,233,239,241,251,257,197,199,211,223,227,229,127,131,137,139,149,151,157,163,167,173,179,2,3,5,7,11,13,17,19,23,29,233,239,241,251,257,263,269,271,31,37,41,43,47,53,59,61,67,71,73,79,83,89});
    RZWboom<int> zkbm;

    for(int i=0; i<primes.size(); i++){
         zkbm.voegtoe(primes[i]);
    }
    zkbm.tekenAlsBinaireBoom("oef1.dot");
    cout << "oef 1: file generated\n";
    cout << "Please copy content of the created file here: http://webgraphviz.com\n";
}