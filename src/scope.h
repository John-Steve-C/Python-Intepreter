//
// Created by Leonard C on 2021/11/24.
////接口文件内,不能写函数的实现(即{}号)

#ifndef PYTHON_INTERPRETER_SCOPE_H
#define PYTHON_INTERPRETER_SCOPE_H


#include <map>
#include <string>
#include "new_any.h"

class Scope {

private:
    std::map<std::string, New_Any> varTable;

public:
    Scope();//

    void varRegister(const std::string &varName, New_Any varData);//重载函数,存储变量

//    void varRegister(const std::string &varName, int varData);//重载函数,存储变量
//    void varRegister(const std::string &varName, double varData);
//    void varRegister(const std::string &varName, std::string varData);

    std::pair<bool, New_Any> varQuery(const std::string &varName) const;//查询变量是否存在
};

#endif //PYTHON_INTERPRETER_SCOPE_H
