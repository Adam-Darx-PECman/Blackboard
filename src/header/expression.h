#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "wstp.h"
#include <cmath>
#include <string>
using namespace std;

class Expression
{
private:
    WSLINK Link;
    string exp;
    const char *String;
public:
    bool linkFlag;
    Expression(string str);
    ~Expression();
    void ExpToMathematica();
    string SymbExpToC();
    void LinkClose();
};

#endif // EXPRESSION_H
