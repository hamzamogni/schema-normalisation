#ifndef DATABASES_FUNCDEPEN_H
#define DATABASES_FUNCDEPEN_H

#include <string>

class FuncDepen {

public:
    FuncDepen(std::string& left, std::string& right);

    std::string getLeft();
    std::string getRight();

private:
    std::string _left;
    std::string _right;
};


#endif