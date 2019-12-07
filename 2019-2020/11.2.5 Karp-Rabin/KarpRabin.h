#include <string>
#include <math.h>
#include <queue>
using namespace std;

class KarpRabin{
public:
   KarpRabin(const string zt);
    queue<int> zoek(const string tekst);

private:
    int d = 26; // base value which idicates the size of the alphabet.
    int r = 7919; // r is a prime number between 0 and 2^w (which is 2^64 − 1 assuming a 64bit processor)
    int p; // size zoekterm
    string zoekterm;
    uint64_t zoekhash;
    int conv2int(char x);
    uint64_t h(string zt);
};

KarpRabin::KarpRabin(const string zt): zoekterm(zt), p(zt.size()){
    zoekhash = h(zoekterm);
}

int KarpRabin::conv2int(char x){
    return x-'a';
}

uint64_t KarpRabin::h(string zt) {
    // formula page 126
    // H(p) = sum from i=0 to p-1: P[i]d^(p-i-1)

    // example, imagine we have only 10 letters so d is 10. a=0, b=1 ... j=9
    // then the word "bac" whould hash to 109
    int p = zt.size();
    int h=0;
    for (int i = 0; i < zt.size(); ++i) {
        h += conv2int(zt[i])*pow(d,(p-i-1));
       // h = h % r;
    }
    return h;
}

queue<int> KarpRabin::zoek(const string tekst) {
    queue<int> results;
    if(tekst.size() > p) {
        uint64_t rollingtextHash = h(tekst.substr(0, p));
        for (int i = 0; i < tekst.size()-p+1; i++) {
            // first we check if the hashes match. If that is the case, we dubbelcheck by verifying the string itself.
            string tmp =  tekst.substr(i, p);
            if (rollingtextHash == zoekhash && tekst.substr(i, p) == zoekterm){
                results.push(i);
            }
            // role the hash an index futher. P126 at the bottom
            // First we remove the first value which becomes now out of scope
            int tmp2 = (conv2int(tekst[i])* pow(d, p-1));
            rollingtextHash -= tmp2;
            // move current index
            rollingtextHash *= d;
            // Next we add the new value which just entered the window
            rollingtextHash += conv2int(tekst[i+p]);
            // check if the word still fits in a processor word.
            //rollingtextHash %= r;
        }
    }
    return results;
}
