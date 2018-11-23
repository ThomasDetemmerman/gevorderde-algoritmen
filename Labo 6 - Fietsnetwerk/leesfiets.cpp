#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <cstdlib>
using std::cout;
using std::endl;
#include "lib/tinyxml.h"
#include "lib/tinystr.cpp"
#include "lib/tinyxml.cpp"
#include "lib/tinyxmlparser.cpp"
#include "lib/tinyxmlerror.cpp"

//volgende zou eigenlijk een lidfunctie van de klasse TiXmlElement moeten zijn,
//er zou opportuniteitsovererving kunnen worden toegepast.
//hier losse functie gebruikt om gebruik van tinyxml beter te illustreren.
void schrijfAttributen(TiXmlElement* el){
    TiXmlAttribute* attr=el->FirstAttribute();
    while (attr!=0){
        cout<<attr->Name()<<" "<<attr->Value()<<"  ";
        attr=attr->Next();
    }
    cout<<endl;
}

int main()
{
        TiXmlDocument doc( "fietsgraaf.xml" );
        bool loadOkay = doc.LoadFile();
        assert(loadOkay);
        TiXmlNode* netwerk;
        netwerk = doc.RootElement();
        assert( netwerk );
        cout<<netwerk->Value()<<"\n";
//Alternatief:
//        for (TiXmlElement* knooppunt=netwerk->FirstChildElement();
//                   knooppunt!=0;knooppunt=knooppunt->NextSiblingElement()){
        for (TiXmlNode* knooppunt=netwerk->FirstChild(); knooppunt!=0;knooppunt=knooppunt->NextSibling()){
            cout<<knooppunt->Value()<<"\n";
            schrijfAttributen(dynamic_cast<TiXmlElement*>(knooppunt));
            for(TiXmlNode* route=knooppunt->FirstChild(); route != 0;route=route->NextSibling()){
                cout<<route->Value()<<"\n";
                schrijfAttributen(dynamic_cast<TiXmlElement*>(route));

            }
        }
        return 0;
}

