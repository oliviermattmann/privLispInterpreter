#ifndef CPPLISPINTERPRETER_TYPES_H
#define CPPLISPINTERPRETER_TYPES_H
#include <vector>
#include <string>
#include <memory>

class Env;

#define DYNAMIC_CAST(Type, Value)  (dynamic_cast<Type*>(Value))

enum Types{
    Integer,
    Double,
    Symbol,
    List,
    BuiltInFunction,
    Lambda,
    True,
    False,
    Nil
};

/**
 * Uppermost class in the Type hierarchy
 */
class Type {
public:
    virtual std::string inspect() = 0;
    virtual Types getType() = 0;
};

class TrueType: public Type {
public:
    virtual std::string inspect();
    virtual Types getType();
private:
    std::string sym = "t";
};

class FalseType: public Type {
public:
    virtual std::string inspect();
    virtual Types getType();
private:
    std::string sym = "()";
};

class NilType: public Type {
public:
    virtual std::string inspect();
    virtual Types getType();
private:
    std::string sym = "()";
};

class IntType: public Type {
public:
    IntType(std::string);
    int getVal();
    virtual std::string inspect();
    virtual Types getType();
private:
    int val;
};

class DoubleType: public Type {
public:
    DoubleType(std::string);
    double getVal();
    virtual std::string inspect();
    virtual Types getType();

private:
    double val;
};

class SymbolType: public Type {
public:
    SymbolType(std::string);
    std::string symStr();
    virtual std::string inspect();
    virtual Types getType();
private:
    std::string str_val;
};

class ListType: public Type {
public:
    ListType();
    ListType(std::vector<Type*>);

    void push(Type*);
    virtual std::string inspect();
    virtual Types getType();

    std::vector<Type*> getList();
    bool isEmpty();
    Type* at(int index);
private:
    std::vector<Type*> list;
};

class FuncType: public Type {
};

using func_p = Type* (*)(std::vector<Type*>);
/**
 * BuiltInFuncType is a class that represents functions that are implemented in c++
 */
class BuiltInFuncType : public FuncType {
public:
    BuiltInFuncType(func_p);
    virtual std::string inspect();
    virtual Types getType();
    ::func_p getFunctionPointer();
private:
    ::func_p fp;
};

/**
 * LambdaFuncType is a class that represent functions that are implemented in Lisp
 */
class LambdaFuncType : public FuncType {
public:
    LambdaFuncType(ListType* bindings, Type* body, Env* env);
    virtual std::string inspect();
    virtual Types getType();
    Env *env;
    ListType *bindings;
    Type *body;
};

#endif //CPPLISPINTERPRETER_TYPES_H
