#include "headers/Table.h"


using namespace std;

int main() {
    Table a;
    cin >> a;
    cout << a;

    vector<string> keys = a.keyGen();
    for (int i = 0; i < keys.size(); ++i)
        cout << keys[i] << endl;

//    set<string> keys = a.getKeys();
//    set<string>::iterator it;
//
//    for (it = keys.begin();  it != keys.end() ; it++) {
//        cout << *it << endl;
//    }

//    vector<Table> b;
//    if (a.checkNF(key) == 3)
//        cout << "\n3NF\n";
//    else if (a.checkNF(key) == 2)
//    {
//        cout << "\n2NF\n";
//        b = a.deco3fn(key);
//        for (const auto &j : b)
//            cout << j;
//    }
//    else
//    {
//        cout << "\n1NF\n";
//        b = a.deco2fn(key);
//        for (const auto &j : b)
//            cout << j;
//    }
//

    return 0;
}