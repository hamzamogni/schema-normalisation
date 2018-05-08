#include "headers/Table.h"

using namespace std;



void decoBCNF(Table a)
{
    vector<Table> ret;
    vector<int> toDelete;
    if (a.isBCNF()){
        cout << a << endl;
    } else {
        FuncDepen df = a.getFD()[0];
        string leftPart(df.getLeft());

        Table one(a.closure(leftPart));
        Table two(leftPart+substitute(a.getAttr(), a.closure(leftPart)));

        for (int i = 0; i < a.getFD().size(); ++i)
            if (classifyFD(one.getAttr(), a.getFD()[i]))
            {
                FuncDepen processed = a.getFD()[i];
                one.setFD(processed);
            }
        for (int j = 0; j < a.getFD().size(); ++j)
        {
            if (classifyFD(two.getAttr(), a.getFD()[j]))
            {
                FuncDepen processed = a.getFD()[j];
                two.setFD(processed);
            }
        }

        one.setKeys();
        two.setKeys();

        decoBCNF(one);
        decoBCNF(two);
        a.delFD(df);
    }
}


int main() {

    Table a;
    cin >> a;
    decoBCNF(a);
//    cout << a << endl;
//
//    if (a.isBCNF()) cout << "BCNF";
//    else cout << "not BCNF";
//
//    cout << a;
//
//
//    vector<string> keys = a.getKeys();
//    for (int i = 0; i < keys.size(); ++i)
//        cout << keys[i] << endl;

    return 0;
}
