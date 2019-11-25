#include <iostream>
#include "regexp11.h"
#include "thompsonna.h"
#include "zoeker.h"



int main() {
    Regexp regex("et|aliquet");
    ThompsonNA thompsonNa(regex);
    thompsonNa.teken("output.dot");
    string data = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui.";
    queue<int> results = ZoekerNA(thompsonNa).zoek(data);
    while(!results.empty()){
        cout << "found value match at the end index " << results.front() << endl;
        results.pop();
    }
    return 0;
}