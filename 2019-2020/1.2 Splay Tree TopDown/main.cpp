#include <iostream>
#include "SplayTree.h"

int main() {
    SplayTree<int, char> st;
    st.voegToe(5, 'a');
    st.voegToe(4, 'b');
    st.voegToe(6, 'c');
    st.voegToe(1, 'c');
    st.voegToe(2, 'c');
    st.teken("output.dot");
    return 0;
}