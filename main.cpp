#include <iostream>

#include "Table.h"

using namespace std;

int main() {
    Table a;
    string b("A");
    //a.keyGen();



    set<string> foundKeys = a.keyGen();

    set<string>:: iterator it;
    for( it = foundKeys.begin(); it!=foundKeys.end(); ++it){
        cout << *it << endl;
    }

    return 0;
}