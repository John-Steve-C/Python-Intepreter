//
// Created by Leonard C on 2021/11/27.
//
#include "new_any.h"
#include <sstream>
#include <iomanip>

int2048 double_to_int(const double &x){
    string temp = to_string(x);
    temp.resize(temp.length() - 7);
    return int2048(temp);
}

//删除头尾的 ”
string Format_string(string s){
    if (s.length() <= 2) return "";
    if (s[0] == '\"' && s[s.length() - 1] == '\"')
        return s.substr(1,s.length() - 2 );
}

bool validateVarName(const std::string& str) {
    for (const auto& ch : str) {
        if (!(ch >= 'a' && ch <= 'z') && !(ch >= 'A' && ch <= 'Z')) {
            return false;
        }
    }
    return true;
}

//构造函数
New_Any::New_Any() {
    type_id = 0;
    int_data = 0;
    double_data = 0;
    str_data = "";
    bool_data = false;
}
New_Any::New_Any(const int2048 &x){
    int_data = int2048(x);
    type_id = 1;
}
New_Any::New_Any(const double &x) {
    double_data = x;
    type_id = 2;
}
New_Any::New_Any(const bool &x) {
    bool_data = x;
    type_id = 3;
}
New_Any::New_Any(const string &s) {
    if (s == "True") { //Python的标识符与C++不同！
        bool_data = true;
        type_id = 3;
    }
    else if (s == "False") {
        bool_data = false;
        type_id = 3;
    }
    else {
        string temp = s;
//        cout << s << '\n';
        str_data = Format_string(temp);
        type_id = 4;
//        cout << str_data << "x\n";
    }
}
New_Any::New_Any(const New_Any &x) {
    this->type_id = x.type_id;
    this->int_data = x.int_data;
    this->double_data = x.double_data;
    this->bool_data = x.bool_data;
    this->str_data = x.str_data;
}

int2048 New_Any::to_int() const {
    if (type_id == 1) {return int_data;}
    if (type_id == 2) {return double_to_int(double_data);}
    if (type_id == 3) {return bool_data;}
    if (type_id == 4) {return str_data;}
    return int2048(0);
}
double New_Any::to_double() const {
    if (type_id == 1) {return (double) int_data;}
    if (type_id == 2) {return double_data;}
    if (type_id == 3) {return bool_data;}
    if (type_id == 4) {return std::stod(str_data);}
    return 0;
}
bool New_Any::to_bool() const {
    if (type_id == 1) {return (bool) (int_data != (int2048) 0 );}
    else if (type_id == 2) {return (bool) double_data;}
    else if (type_id == 3) {return bool_data;}
    else if (type_id == 4) {return !str_data.empty();}
    return false;
}
string New_Any::to_str() const {
    if (type_id == 4) {
        return str_data;
    }
    else {
        stringstream s;
        if (type_id == 1) s << int_data;
        else if (type_id == 2) s << double_data;
        else if (type_id == 3) {
            if (bool_data == true) s << "True";
            else s << "False";
        }
        return s.str();
    }
}

//todo:重载比较运算符
bool New_Any::operator> (const New_Any &x){
    if (type_id == 1) {return int_data > x.to_int();}
    if (type_id == 2) {return double_data > x.to_double();}
    if (type_id == 3) {return bool_data > x.to_bool();}
    if (type_id == 4) {return str_data > x.to_str();}
}
bool New_Any::operator< (const New_Any &x){
    if (type_id == 1) {return int_data < x.to_int();}
    if (type_id == 2) {return double_data < x.to_double();}
    if (type_id == 3) {return bool_data < x.to_bool();}
    if (type_id == 4) {return str_data < x.to_str();}
}
bool New_Any::operator<= (const New_Any &x){
    return !(*this > x);
}
bool New_Any::operator>= (const New_Any &x){
    return !(*this < x);
}
bool New_Any::operator== (const New_Any &x){
    if (type_id != x.type_id) return false;
    if (type_id == 1) {return int_data == x.int_data;}
    if (type_id == 2) {return double_data == x.double_data;}
    if (type_id == 3) {return bool_data == x.bool_data;}
    if (type_id == 4) {return str_data == x.str_data;}
}
bool New_Any::operator!= (const New_Any &x){
    return !(*this == x);
}

//todo:四则运算,返回类型与 x同
New_Any New_Any::Add(const New_Any &x, const New_Any &y) {//类型转换有优先级
    New_Any tp(x),temp(y);
    if (x.type_id == 2 || y.type_id == 2) {
        tp.double_data = tp.to_double() + temp.to_double();
        tp.type_id = 2;
        return tp;
    }
    if ( x.type_id == 1 || y.type_id == 1 ||
        (x.type_id == 3 && y.type_id == 3) ) {
        tp.int_data = tp.to_int() + temp.to_int();
        tp.type_id = 1;
        return tp;
    }
//    if (x.type_id == 3) {
//        return x.bool_data || temp.to_bool();
//    }

    if (x.type_id == 4) {
        tp.str_data = x.str_data + temp.to_str();
        return tp;
    }
}
New_Any New_Any::Minus(const New_Any &x, const New_Any &y) {
    New_Any tp(x),temp(y);
    if (x.type_id == 2 || y.type_id == 2) {
        tp.double_data = tp.to_double() - temp.to_double();
        tp.type_id = 2;
        return tp;
    }
    if ( x.type_id == 1 || y.type_id == 1 ||
         (x.type_id == 3 && y.type_id == 3) ) {
        tp.int_data = tp.to_int() - temp.to_int();
        tp.type_id = 1;
        return tp;
    }
}
New_Any New_Any::Multiple(const New_Any &x, const New_Any &y) {
    New_Any tp(x),temp(y);
    //字符串，特殊处理
    if (x.type_id == 4 || y.type_id == 4) {//两个都要判断
        if (x.type_id == 4) {
            for (int2048 i = 1;i < temp.to_int();i += 1){
                tp.str_data += x.str_data;
            }
            return tp;
        }
        else {
            for (int2048 i = 1;i < tp.to_int();i += 1){
                temp.str_data += y.str_data;
            }
            return temp;
        }
    }
    //double
    if (x.type_id == 2 || y.type_id == 2) {
        tp.double_data = tp.to_double() * temp.to_double();
        tp.type_id = 2;
        return tp;
    }
    if ( x.type_id == 1 || y.type_id == 1 ||
         (x.type_id == 3 && y.type_id == 3) ) {
        tp.int_data = tp.to_int() * temp.to_int();
        tp.type_id = 1;
        return tp;
    }
//    if (x.type_id == 3) return x.bool_data && temp.to_bool();

}
New_Any New_Any::Div(const New_Any &x, const New_Any &y) {
    New_Any tp(x),temp(y);
    if (x.type_id == 2 || y.type_id == 2) {//浮点除
        New_Any t(x);
        return t.to_double() * temp.to_double();
    }
    //剩下的都是整除
    if (x.type_id == 1) return x.int_data / temp.to_int();
//    if (x.type_id == 3) return x.bool_data / temp.to_bool();
}

New_Any New_Any::operator+ (const New_Any &x){
    return Add(*this,x);
}
New_Any New_Any::operator- (const New_Any &x){
    return Minus(*this,x);
}
New_Any New_Any::operator* (const New_Any &x){
    return Multiple(*this,x);
}
New_Any New_Any::operator/ (const New_Any &x){
    return Div(*this,x);
}
New_Any New_Any::operator% (const New_Any &x) {
    New_Any a(*this),b(x);
    return a - ( a / b ) * b;
}
New_Any New_Any::operator-() const { //负号的重载
    return New_Any(int2048(0)) - *this;
}

New_Any New_Any::operator+= (const New_Any &x){
    return *this = *this + x;
}
New_Any New_Any::operator-= (const New_Any &x){
    return *this = *this - x;
}
New_Any New_Any::operator*= (const New_Any &x){
    return *this = *this * x;
}
New_Any New_Any::operator/= (const New_Any &x){
    return *this = *this / x;
}
New_Any New_Any::operator%=(const New_Any &x) {
    return *this = *this % x;
}

//todo:重载输出流
std::ostream &operator<<(std::ostream &output,const New_Any &x){
    if (x.type_id == 0) output << "None";
    if (x.type_id == 1) output << (int2048) x.int_data;
    if (x.type_id == 2) output << std::fixed << std::setprecision(6) << x.double_data;
    //设置默认输出6位小数
    if (x.type_id == 3) {
        if (x.bool_data) output << "True";
        else output << "False";
    }
    if (x.type_id == 4) {
        output <<(string) x.str_data;
    }
    return output;
}


