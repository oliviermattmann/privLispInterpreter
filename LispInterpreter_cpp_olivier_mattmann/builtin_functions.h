#ifndef CPPLISPINTERPRETER_BUILTIN_FUNCTIONS_H
#define CPPLISPINTERPRETER_BUILTIN_FUNCTIONS_H

#include "types.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>

class Env;
class REPL;
//using extern because some built-in functions need to use those functions declared elsewhere
extern Env* global_env;
extern Type* EVAL(Type* ast, Env* global_env);
extern Type* READ(std::string s);
extern std::string PRINT(Type* expression);
extern std::string REPL(std::string input/*environment*/, Env *env);
extern Type* read_str(std::string &input);


class BuiltInException : public std::exception {
    std::string description;

public:
    BuiltInException(std::string _description) : description(_description) {}

    std::string get_symbol() const {
        return description;
    }
    const char* what () const throw () {
        return "Built-In Function exception: ";
    }
};


Type* add(std::vector<Type*> args);

Type* mul(std::vector<Type*> args);

Type* div(std::vector<Type*> args);

Type* sub(std::vector<Type*> args);

Type* geq(std::vector<Type*> args);

Type* gr(std::vector<Type*> args);

Type* leq(std::vector<Type*> args);

Type* lr(std::vector<Type*> args);

Type* eqfunc(std::vector<Type*> args);

Type* eval_func(std::vector<Type*> args);

Type* cons(std::vector<Type*> args);

Type* car(std::vector<Type*> args);

Type* cdr(std::vector<Type*> args);

Type* load(std::vector<Type*> args);

Type* parse(std::vector<Type*> args);

Type* list(std::vector<Type*> args);

Type* isNull(std::vector<Type*> args);

Type* isAtom(std::vector<Type*> args);

Type* isNumber(std::vector<Type*> args);

Type* isSymbol(std::vector<Type*> args);

Type* isPair(std::vector<Type*> args);

Type* display_func(std::vector<Type*> args);

#endif //CPPLISPINTERPRETER_BUILTIN_FUNCTIONS_H
