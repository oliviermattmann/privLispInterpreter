#include "types.h"
//#include <iostream>

std::string TrueType::inspect() {
    return sym;
}
Types TrueType::getType() {
    return True;
}

std::string FalseType::inspect() {
    return sym;
}
Types FalseType::getType() {
    return False;
}

std::string NilType::inspect() {
    return sym;
}
Types NilType::getType() {
    return Nil;
}


ListType::ListType() {

}
ListType::ListType(std::vector<Type*> listElemnts) {
    for (auto elements : listElemnts) {
        list.push_back(elements);
    }

}

void ListType::push(Type * type) {
    list.push_back(type);
}

std::string ListType::inspect() {
    std::string list_str = "(";
    for (auto *type : list) {
        list_str.append(type->inspect());
        list_str.append(" ");
    }
    if (list.size() > 0) {
        list_str.pop_back();
    }
    list_str.append(")");
    return list_str;
}

Types ListType::getType() {
    return Types::List;
}

std::vector<Type*> ListType::getList() {
    return list;
}

bool ListType::isEmpty() {
    return list.empty();
}

Type* ListType::at(int index) {
    return list.at(index);
}


SymbolType::SymbolType(std::string str) {
    str_val = str;
}

std::string SymbolType::symStr() {
    return str_val;
}

std::string SymbolType::inspect() {
    return symStr();
}

Types SymbolType::getType() {
    return Types::Symbol;
}

IntType::IntType(std::string str) {
    val = std::stoi(str);
}

int IntType::getVal() {
    return val;
}

std::string IntType::inspect() {
    return std::to_string(getVal());
}

Types IntType::getType() {
    return Types::Integer;
}

DoubleType::DoubleType(std::string str) {
    val = std::stod(str);
}

double DoubleType::getVal() {
    return val;
}

std::string DoubleType::inspect() {
    return std::to_string(getVal());
}

Types DoubleType::getType() {
    return Types::Double;
}

BuiltInFuncType::BuiltInFuncType(func_p f_p) {
    fp = f_p;
}
std::string BuiltInFuncType::inspect() {
    return "<fn... represent function somehow?>";
}

Types BuiltInFuncType::getType() {
    return Types::BuiltInFunction;
}
func_p BuiltInFuncType::getFunctionPointer() {
    return fp;
}

LambdaFuncType::LambdaFuncType(ListType* bindings, Type* body, Env* env) {
    this->bindings = bindings;
    this->body = body;
    this->env = env;
}
std::string LambdaFuncType::inspect() {
    return "<Lambda expression>";
}
Types LambdaFuncType::getType() {
    return Types::Lambda;
}





