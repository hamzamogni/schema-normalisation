#include "FuncDepen.h"

using namespace std;

FuncDepen::FuncDepen(string& left, string& right) {
    _left  = left;
    _right = right;
}

string FuncDepen::getLeft() {
    return _left;
}

string FuncDepen::getRight() {
    return _right;
}