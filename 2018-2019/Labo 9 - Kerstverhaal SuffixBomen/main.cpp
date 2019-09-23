


#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <vector>
#include <fstream>
#include <string>
#include "suffixboom17.h"

using namespace std;

const string DATA_DIR="./verhalen";


vector<string> list_dir(string path) {
   struct dirent *entry;
   std::vector<std::string> files;
   DIR *dir = opendir(path.c_str());
   
   if (dir == NULL) {
      return files;
   }
   while ((entry = readdir(dir)) != NULL) {
    files.push_back(entry->d_name);
   }
   closedir(dir);
   return files;
}

int main() {
   vector<string> verhaalfiles = list_dir(DATA_DIR);

   for(string file: verhaalfiles){

         // DEEL 1
         // Alles inlezen. Hierbij komt alle bits op 1 te staan.

         //In de kerstverhalen zit er geen enkel karakter met rangnummer -60 tot 0, grenzen inbegrepen
         int TERMINATE_SIGN=0;
        ifstream bigFile(DATA_DIR+"/"+file);
        //volledige verhaal inlezen als 1 string
        string content( (std::istreambuf_iterator<char>(bigFile) ),(std::istreambuf_iterator<char>()));
        //gebruiker kan niet aan de bitpatronen. Daarom sluiten we elke tekst af met spefciaal teken dat de 
        //gebruiker wel kan gebruiken. Wij moeten dit daarna omzetten naar ons bitpatroon.

        //to do: elke tekst ander afsluitteken
        content += TERMINATE_SIGN;
        TERMINATE_SIGN--;
        //correcte manier van aanmaken van objecten.
        Sboom suffixboom;
        suffixboom.voegtoe(content,TERMINATE_SIGN);
        vector<char> result; 
        //suffixboom.geefLGD(result);

        for(char c: result){
           cout << c << "";
        }

        // DEEL 2
        // files per paren inlezen.

   }
}