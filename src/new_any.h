//
// Created by Leonard C on 2021/11/27.
//

#ifndef PYTHON_INTERPRETER_NEW_ANY_H
#define PYTHON_INTERPRETER_NEW_ANY_H

#include <string>
#include "int2048.hpp"
#include <map>

using namespace sjtu;
using namespace std;


int2048 double_to_int(const double &x);
string Format_string(string s);
bool validateVarName(const std::string &str);

class New_Any {
public:
    int2048 int_data;
    double double_data;
    bool bool_data;
    string str_data;
//    string type[5] = {"_","int","double","bool","string"};
    int type_id;

public:

    //构造函数
    New_Any();
    New_Any(const int2048 &);
    New_Any(const double &);
    New_Any(const string &);
    New_Any(const bool &);
    New_Any(const New_Any &);

    //类型转换函数
    bool to_bool() const;
    int2048 to_int() const;
    double to_double() const;
    string to_str() const;

    //重载运算符（>=）
    bool operator> (const New_Any &x);
    bool operator< (const New_Any &x);
    bool operator<= (const New_Any &x);
    bool operator>= (const New_Any &x);
    bool operator== (const New_Any &x);
    bool operator!= (const New_Any &x);
    //+,-,*,/
    New_Any Add(const New_Any &x,const New_Any &y);
    New_Any Minus(const New_Any &x,const New_Any &y);
    New_Any Multiple(const New_Any &x,const New_Any &y);
    New_Any Div(const New_Any &x,const New_Any &y);
    New_Any operator+ (const New_Any &x);
    New_Any operator- (const New_Any &x);
    New_Any operator* (const New_Any &x);
    New_Any operator/ (const New_Any &x);
    New_Any operator% (const New_Any &x);

    New_Any operator+= (const New_Any &x);
    New_Any operator-= (const New_Any &x);
    New_Any operator*= (const New_Any &x);
    New_Any operator/= (const New_Any &x);
    New_Any operator%= (const New_Any &x);

    friend std::ostream &operator<<(std::ostream &output,const New_Any &x);

//    New_Any operator// (const New_Any &x);

};

#endif //PYTHON_INTERPRETER_NEW_ANY_H
