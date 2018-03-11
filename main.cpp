#include <iostream>

#include "Table.h"

using namespace std;

int main() {
    Table a;

    set<string> foundKeys = a.keyGen();

//    set<string>:: iterator it;
//    string key = *foundKeys.begin();


//    for( it = foundKeys.begin(); it!=foundKeys.end(); ++it){
//        cout << *it << endl;
//    }

    if (a.checkNF("AB") == 3)
        cout << "\n\n3NF\n\n";
    else if (a.checkNF("AB") == 2)
        cout << "\n\n2NF\n\n";
    else
        cout << "\n\n1NF\n\n";

    return 0;
}