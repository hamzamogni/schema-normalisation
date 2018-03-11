#include <iostream>

#include "Table.h"

using namespace std;

int main() {
    Table a;

    string key = *a.keyGen().begin();

    cout << key;

    if (a.checkNF(key) == 3)
        cout << "\n\n3NF\n\n";
    else if (a.checkNF(key) == 2)
        cout << "\n\n2NF\n\n";
    else
        cout << "\n\n1NF\n\n";

    return 0;
}