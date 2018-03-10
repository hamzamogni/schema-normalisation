#include "Table.h"

using namespace std;

Table::Table()
{
    cout << "\nHow many attributes do you have ? ";
    cin >> _nbrAttributes;

    for (int i = 0, chr = 65; i < _nbrAttributes; ++i, chr++)
        _attributes.append(1, chr);

    cout << *this;

    cout << "\nHow many functional Dependencies ? ";
    cin >> _nbrFunctDepen;

    string left, right;
    for (int i = 0; i < _nbrFunctDepen; ++i)
    {
        cout << "The FD N° " << i + 1;
        cout << "\n\t-Left part : ";
        cin >> left;
        for (int j = 0; j < left.length(); ++j)
            left[j] = toupper(left[j]);

        cout << "\t-Right part : ";
        cin >> right;
        for (int j = 0; j < right.length(); ++j)
            right[j] = toupper(right[j]);


        _fds.emplace_back(FuncDepen(left, right));
    }

}

string Table::getAttr() const
{
    return _attributes;
}

vector<FuncDepen> Table::getFD() const
{
    return _fds;
}

int Table::getNbrFD() const
{
    return _nbrFunctDepen;
}

string Table::concatenateRight() const
{
    vector<FuncDepen> arr = getFD();
    string ret;

    for (int i = 0; i < arr.size(); ++i)
        ret.append(arr[i].getRight());

    return format(ret);
}

string Table::notInRight() const
{
    string temp = concatenateRight();
    string ret;

    for (int i = 0; i < _attributes.length(); ++i)
        if (temp.find_first_of(_attributes[i]) == string::npos)
            ret.push_back(_attributes[i]);

    return ret;
}

string Table::closure(string const &src) const
{
    string clos = src;
    vector<FuncDepen> arr = getFD();
    int count = 1;

    do
    {
        for (int i = 0; i < _nbrFunctDepen; ++i)
        {
            string left = arr[i].getLeft();

            for (int j = 0; j < left.length(); ++j)
                if( clos.find(left[j]) == string::npos)
                    break;
                else if (j == left.length()-1 && clos.find(left[j]) != string::npos)
                    clos.append(arr[i].getRight());
        }
    } while (count++ < _nbrFunctDepen);

    return format(clos);
}

set<string> Table::keyGen() const
{
    set<string> foundKeys;
    vector<FuncDepen> arr = getFD();
    string initKey = notInRight(), tmp;

    if (closure(initKey) == _attributes)
    {
        tmp = format(tmp);
        foundKeys.insert(initKey);
        return foundKeys;
    } else
    {
        for (int i = 0; i < _nbrFunctDepen; ++i)
        {
            tmp = initKey + arr[i].getLeft();
            if (closure(tmp) == _attributes)
                foundKeys.insert(format(tmp));
        }

        return foundKeys;
    }
}

std::string Table::format(std::string& src) const
{
    string ret = src;

    sort(ret.begin(), ret.end());

    string::iterator it = unique(ret.begin(), ret.end());
    ret.erase(it, ret.end());

    return ret;
}

ostream &operator<<(ostream &flux, Table const &table)
{
    vector<FuncDepen> arr = table.getFD();

    cout << "\tR(";
    for (int j = 0; j < table.getAttr().length(); ++j)
    {
        if (j == table.getAttr().length() - 1)
        {
            cout << table.getAttr()[j] << ")";
            break;
        }
        cout << table.getAttr()[j] << ", ";
    }

    cout << endl;
    for (int i = 0; i < table.getNbrFD(); ++i)
        cout << "\t\tFD N° " << i + 1 << " : " << arr[i].getLeft() << " --> " << arr[i].getRight() << endl;

    return flux;
}