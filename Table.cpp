#include "headers/Table.h"


using namespace std;

Table::Table()
{
    cout << "\nHow many attributes do you have ? ";
    cin >> _nbrAttributes;

    for (int i = 0, chr = 65; i < _nbrAttributes; ++i, chr++)
        _attributes.append(1, chr);

    cout << *this;

    cout << "How many functional Dependencies ? ";
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



    _key = *keyGen().begin();

}

Table::Table
        (std::string attr, std::string key)
{
    _attributes = attr;
    _key        = key;
}

string Table::getAttr
        () const
{
    return _attributes;
}

vector<FuncDepen> Table::getFD
        () const
{
    return _fds;
}

int Table::getNbrFD
        () const
{
    return _nbrFunctDepen;
}

std::string Table::getKey
        () const
{
    return _key;
}

string Table::concatenateRight
        () const
{
    vector<FuncDepen> arr = getFD();
    string ret;

    for (auto &i : arr)
        ret.append(i.getRight());

    return format(ret);
}

string Table::concatenateLeft
        () const
{
    vector<FuncDepen> arr = getFD();
    string ret;

    for (auto &i : arr)
        ret.append(i.getLeft());

    return format(ret);
}

string Table::notInRight
        () const
{
    string temp = concatenateRight();
    string ret;

    for (char _attribute : _attributes)
        if (temp.find_first_of(_attribute) == string::npos)
            ret.push_back(_attribute);

    return ret;
}


std::string Table::notInLeft() const
{
    string temp = concatenateLeft();
    string ret;

    for (char _attribute : _attributes)
        if (temp.find_first_of(_attribute) == string::npos)
            ret.push_back(_attribute);

    return ret;
}

string Table::closure
        (string const &src) const
{
    /*
     * Function to return the closure of a given string
     */
    string clos = src;
    vector<FuncDepen> arr = getFD();
    int count = 0;

    while (++count <= _nbrFunctDepen)
        for (int i = 0; i < _nbrFunctDepen; ++i)
            for (int j = 0; j < arr[i].getLeft().length(); ++j)
                if (j == arr[i].getLeft().length() - 1 && clos.find(arr[i].getLeft()[j]) != string::npos)
                    clos.append(arr[i].getRight());
                else if (clos.find(arr[i].getLeft()[j]) == string::npos)
                    break;

    return format(clos);
}


set<string> Table::keyGen
        () const
{
    /*
     * function to generate the set of all the possible minimal keys
     */
    set<string> foundKeys;                // the returned set
    vector<FuncDepen> arr = getFD();      // all the functional dependencies
    string  initKey = notInRight(), tmp;  // the initial key (attributes that doesn't exist in all right parts

    if (closure(initKey) == _attributes)
    {
        // if the initial key closure contains all the attributes then we're done, it's the minimal key
        foundKeys.insert(initKey);
        return foundKeys;
    } else
    {
        /*
         * else we loop over all the functional dependencies,
         * we check the closure of the string containing the initial key and the left part of the dependency
         * if the closure contains all the attributes then we insert that key to the set of the keys
         */
        string clos, temp("fffffffffffffffffff");
        for (int i = 0; i < _nbrFunctDepen; ++i)
        {
            string str = initKey + arr[i].getLeft();
            tmp = format(str);
            clos = closure(tmp);
            if ( clos == _attributes)
            {
                if (tmp.length() > temp.length())
                    return foundKeys;
                foundKeys.insert(format(tmp));
                temp = format(tmp);
            }
        }
        return foundKeys;
    }
}

std::string Table::format
        (std::string &src) const
{
    /*
     * This function format a string by sorting it alphabetically
     * and removing repeated letters
     */
    string ret = src;

    sort(ret.begin(), ret.end()); // to sort the string

    // removing repeated letters
    string::iterator it = unique(ret.begin(), ret.end()); // removes consecutive duplicate elements in a range
    ret.erase(it, ret.end());

    return ret;
}

bool Table::check3NF
        (string key)
{
    vector<FuncDepen> fds = getFD();
    for (int i = 0; i < fds.size(); ++i)
        for (int j = 0; j < fds.size(); ++j)
            if (fds[i].getLeft() == fds[j].getRight())
                return false;
    return true;
}

bool Table::check2NF
        (string key)
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

int Table::checkNF
        (std::string key)
{
    if (key.length() == 1)
    {
        if (check3NF(key))
            return 3;
        return 2;
    }
    else
    {
        if (check2NF(key))
        {
            if (check3NF(key))
                return 3;
            return 2;
        }
    }
    return 1;
}

int Table::search
        (string fd, string key)
{
    int ret(0);
    for (char i : key)
        if(fd.find_first_of(i) != string::npos)
            ret++;

    return ret;
}

vector<Table> Table::deco2fn
        (std::string key)
{
    vector<FuncDepen> fds = getFD();
    vector<Table> ret;
    for (auto &fd : fds)
    {
        if( (search(fd.getLeft(), key) > 0) && (search(fd.getLeft(), key) < key.length()) )
        {
            if( search(fd.getLeft(), key) == fd.getLeft().length() && notContains(ret, Table(closure(fd.getLeft()), fd.getLeft())))
            {
                string tmp = closure(fd.getLeft());
                ret.emplace_back(Table(format(tmp), fd.getLeft()));
            }

        } else if(fd.getLeft().length() == key.length() && notContains(ret, Table(fd.getLeft()+fd.getRight(), fd.getLeft()))) {
            string tmp = fd.getLeft()+fd.getRight();
            ret.emplace_back(Table(format(tmp), fd.getLeft()));

        } else if (search(notInLeft(), notInRight()) > 0  &&  notContains(ret, Table(key, key)))
            ret.emplace_back(Table(key, key));

    }

    return ret;
}

vector<Table> Table::deco3fn
        (string key)
{
    vector<Table> ret;
    vector<FuncDepen> fds = getFD();
    for (int i = 0; i < fds.size(); ++i)
    {
        for (int j = 0; j < fds.size(); ++j)
            if (fds[i].getLeft() == fds[j].getRight() && notContains(ret, Table(fds[i].getLeft()+fds[i].getRight(), fds[i].getLeft())))
            {
                string tmp = fds[i].getLeft()+fds[i].getRight();
                ret.emplace_back(Table(format(tmp), fds[i].getLeft()));
            }

        if (fds[i].getLeft().length() == key.length() && notContains(ret, Table(fds[i].getLeft()+fds[i].getRight(), fds[i].getLeft())))
        {
            string tmp = fds[i].getLeft()+fds[i].getRight();
            ret.emplace_back(Table(format(tmp), fds[i].getLeft()));
        }
    }

    return ret;
}

bool notContains(vector<Table> to, Table a)
{
    for (int i = 0; i < to.size(); ++i)
    {
        if (a == to[i])
            return false;
    }
    return true;
}

ostream &operator<<
        (ostream &flux, Table const &table)
{
    /*
     * Overloading the output stream for Table class
     */
    vector<FuncDepen> arr(table.getFD());

    cout << "\n\tR(";
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
    if (!table.getKey().empty())
        cout << "The Key : " << table.getKey() << endl;

    return flux;
}

bool operator==(Table const &a, Table const &b)
{
    return (a.getAttr() == b.getAttr());
}
