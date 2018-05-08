#include "headers/Table.h"


using namespace std;

Table::Table()
{
    _nbrFunctDepen = 0;
    _nbrAttributes = 0;
    _attributes = "";
}

Table::Table
        (std::string attr, std::string key)
{
    _attributes = attr;
    _nbrAttributes = _attributes.size();
    _nbrFunctDepen = 0;
    _keyComposed        = key;
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

std::vector<string> Table::getKeys
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


vector<string> Table::keyGen
        () const
{
    /*
     * function to generate the set of all the possible minimal keys
     */
    set<string> foundKeys;
    vector<string> returned;
    vector<FuncDepen> arr = getFD();      // all the functional dependencies
    string  initKey = notInRight(), tmp;  // the initial key (attributes that doesn't exist in all right parts

    // if the initial key closure contains all the attributes, we add it as a minimal key
//    if (closure(initKey) == _attributes)
//        returned.push_back(initKey);

    /*
     * we loop over all the functional dependencies,
     * we check the closure of the string containing the initial key and the left part of the dependency
     * if the closure contains all the attributes then we insert that key to the set of the keys
     */
    string clos, temp("ffffffff");
    for (int i = 0; i < _nbrFunctDepen; ++i)
    {
        string str = initKey + arr[i].getLeft();
        tmp = format(str);
        clos = closure(tmp);
        if ( clos == _attributes)
            if (tmp.length() <= temp.length())
            {
                foundKeys.insert(format(tmp));
                temp = format(tmp);

            } else break;

    }

    for (set<string>::iterator it = foundKeys.begin(); it != foundKeys.end() ; ++it)
        returned.push_back(*it);

    return returned;
}

std::string format
        (std::string &src)
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

bool Table::isBCNF()
{
    vector<FuncDepen> toRemove;
    for (int i = 0; i < _fds.size(); ++i)
        for (int j = 0; j < _key.size(); ++j)
            if (_fds[i].getLeft().find(_key[j]) != string::npos)
                toRemove.push_back(_fds[i]);

    for (int k = 0; k < toRemove.size(); ++k)
        for (int l = 0; l < _fds.size(); ++l)
        {
            if (toRemove[k].getLeft() == _fds[l].getLeft() && toRemove[k].getRight() == _fds[l].getRight())
            {
                _fds.erase(_fds.begin() + l);
                _nbrFunctDepen--;
            }

        }

    return (_fds.size() == 0 ? true : false);

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

bool notContains
        (vector<Table> to, Table a)
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
    vector<FuncDepen> arr(table.getFD());

    flux << "\n\tR(";
    for (int j = 0; j < table.getAttr().length(); ++j)
    {
        if (j == table.getAttr().length() - 1)
        {
            flux << table.getAttr()[j] << ")";
            break;
        }
        flux << table.getAttr()[j] << ", ";
    }
    cout << endl;
    for (int i = 0; i < table._fds.size(); ++i)
        flux << table._fds[i].getLeft() << " --> " << table._fds[i].getRight() << endl;

    return flux;
}

istream &operator>>
        (istream &flux, Table &table)
{
    int nbrAttr, nbrFDs;
    cout << "\nHow many attributes do you have ? ";
    flux >> nbrAttr;
    table._nbrAttributes = nbrAttr;

    for (int i = 0, chr = 65; i < table._nbrAttributes; ++i, chr++)
        table._attributes.append(1, chr);

    cout << "R(" <<  table._attributes << ")" << endl;
    cout << "How many functional Dependencies ? ";
    flux >> nbrFDs;
    table._nbrFunctDepen = nbrFDs;

    string left, right;
    for (int i = 0; i < table._nbrFunctDepen; ++i)
    {
        cout << "The FD N° " << i + 1;
        cout << "\n\t-Left part : ";
        flux >> left;
        for (char &j : left)
            j = char(toupper(j));

        cout << "\t-Right part : ";
        flux >> right;
        for (char &j : right)
            j = char(toupper(j));

        table._fds.emplace_back(FuncDepen(left, right));
    }
    table._key = table.keyGen();

    return flux;
}

void Table::setNbrAttr(int a)
{
    _nbrAttributes = a;
}

void Table::setFD(FuncDepen const& in)
{
    _fds.push_back(in);
    _nbrFunctDepen++;
}

void Table::delFD(FuncDepen const & in)
{
    for (int i = 0; i < _fds.size(); ++i)
        if (in.getLeft() == _fds[i].getLeft() && in.getRight() == _fds[i].getRight())
        {
            _fds.erase(_fds.begin() + i);
            _nbrFunctDepen--;
        }
}


bool operator==(Table const &a, Table const &b)
{
    return (a.getAttr() == b.getAttr());
}


string substitute(std::string attr, std::string clos)
{
    unsigned long j;

    for (int i = 0; i < clos.size(); ++i)
    {
        j = attr.find_first_of(clos[i]);
        if (j != string::npos)
            attr.erase(attr.begin() + j);
    }

    return format(attr);

}

bool classifyFD(string const& attr, FuncDepen& toCheck)
{
    string concatenated = toCheck.getLeft() + toCheck.getRight();

    for (int i = 0; i < concatenated.size(); ++i)
        if (attr.find_first_of(concatenated[i]) == string::npos)
            return false;

    return true;
}