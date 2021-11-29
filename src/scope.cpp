//
// Created by Leonard C on 2021/11/25.
//实现文件中,要写作用域::

#include "scope.h"
//#include "new_any.h"

Scope::Scope() : varTable() {}

void Scope::varRegister(const std::string &varName, New_Any varData) {//重载函数,存储变量
    varTable[varName] = varData;
}

std::pair<bool, New_Any> Scope::varQuery(const std::string &varName) const {//查询变量是否存在
    auto it = varTable.find(varName);
    if (it == varTable.end()) return std::make_pair(false, New_Any() );
    return std::make_pair(true, it->second);
}
