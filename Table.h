#ifndef DATABASES_TABLE_H
#define DATABASES_TABLE_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "FuncDepen.h"
#include <set>
class Table {

public:
    Table();
    std::string getAttr() const;
    int         getNbrFD() const;
    std::vector<FuncDepen>   getFD() const;
    std::string concatenateRight() const;
    std::string notInRight() const;
    std::string closure(std::string const&) const;
    std::set<std::string> keyGen() const;
private:
    std::string _attributes;
    int         _nbrAttributes;
    int         _nbrFunctDepen;
    std::vector<FuncDepen> _fds;

    std::string format(std::string&) const;
};


std::ostream& operator<<( std::ostream& flux, Table const& table );

#endif //DATABASES_TABLE_H
