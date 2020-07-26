#include "expression.h"

Expression::Expression(string str) : exp(str)
{
    int argc = 4;
    char *argv[5] = { "-linkname", "Expression", "-linkmode", "connect", NULL };
    Link = WSOpen(argc, argv);
    linkFlag = ((WSLINK)0 == Link);
}
Expression::~Expression(){}
void Expression::ExpToMathematica()
{
    //向Mathematica发送表达式，使其计算符号积分
    WSPutFunction(Link, "EvaluatePacket", 1);
    WSPutFunction(Link, "ToExpression", 1);
    WSPutString(Link, exp.c_str());
    WSEndPacket(Link);
}
string Expression::SymbExpToC()
{
    //得到Mathematica计算后的符号积分结果，并返回
    WSNewPacket(Link);
    WSGetString(Link, &String);
    string res(String);
    WSReleaseString(Link, String);
    WSEndPacket(Link);
    return res;
}
void Expression::LinkClose()
{
    WSClose(Link);
    linkFlag = false;
}
