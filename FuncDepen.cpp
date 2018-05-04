#include "headers/FuncDepen.h"

using namespace std;

FuncDepen::FuncDepen(string& left, string& right)
{
    _left  = left;
    _right = right;
}

string FuncDepen::getLeft() const
{
    return _left;
}

string FuncDepen::getRight() const
{
    return _right;
}