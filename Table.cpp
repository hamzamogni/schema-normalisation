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
        for (char &j : left)
            j = char(toupper(j));

        cout << "\t-Right part : ";
        cin >> right;
        for (char &j : right)
            j = char(toupper(j));


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

    for (auto &i : arr)
        ret.append(i.getRight());

    return format(ret);
}

string Table::notInRight() const
{
    string temp = concatenateRight();
    string ret;

    for (char _attribute : _attributes)
        if (temp.find_first_of(_attribute) == string::npos)
            ret.push_back(_attribute);

    return ret;
}

string Table::closure(string const &src) const
{
    string clos = src;
    vector<FuncDepen> arr = getFD();
    int count = 0;

    while (++count < _nbrFunctDepen)
        for (int i = 0; i < _nbrFunctDepen; ++i)
        {
            string left = arr[i].getLeft();

            for (int j = 0; j < left.length(); ++j)
                if (j == left.length() - 1 && clos.find(left[j]) != string::npos)
                    clos.append(arr[i].getRight());
                else if (clos.find(left[j]) == string::npos)
                    break;
        }

    return format(clos);
}

set<string> Table::keyGen() const
{
    set<string> foundKeys;
    vector<FuncDepen> arr = getFD();
    string initKey = notInRight(), tmp;

    if (closure(initKey) == _attributes)
    {
//        tmp = format(tmp);
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

std::string Table::format(std::string &src) const
{
    string ret = src;

    sort(ret.begin(), ret.end());

    string::iterator it = unique(ret.begin(), ret.end());
    ret.erase(it, ret.end());

    return ret;
}

bool Table::check3NF(string key)
{
    vector<FuncDepen> fds = getFD();
    for (auto &fd : fds)
    {
        if (search(fd.getLeft(), key) != 0 )
            return true;
    }
    return false;
}

bool Table::check2NF(string key)
{
    vector<FuncDepen> fds = getFD();
    for (auto &fd : fds)
    {
        if( (search(fd.getLeft(), key) > 0) && (search(fd.getLeft(), key) < key.length()) )
            if( search(fd.getLeft(), key) == fd.getLeft().length() )
                return false;
    }
    return true;
}

int Table::checkNF(std::string key)
{
    if (check2NF(key))
    {
        if (check3NF(key))
            return 3;

        return 2;
    }
    return 1;
}

int Table::search(string fd, string key)
{
    int ret(0);
    for (int i = 0; i < fd.length(); ++i)
        if(fd.find_first_of(key[i]) != string::npos)
            ret++;

    return ret;
}

ostream &operator<<(ostream &flux, Table const &table)
{
    vector<FuncDepen> arr(table.getFD());

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