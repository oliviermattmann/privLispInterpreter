#ifndef CPPLISPINTERPRETER_ENV_H
#define CPPLISPINTERPRETER_ENV_H

#include <map>
#include "types.h"
#include "builtin_functions.h"

/**
 * Small exception class, so REPL doesn't crash when
 * a symbol is not found in the environments
 */
class EnvException : public std::exception {
    std::string symbol;

public:
    EnvException(std::string _symbol) : symbol(_symbol) {}

    std::string get_symbol() const {
        return symbol;
    }
    const char* what () const throw () {
        return " is undefined";
    }
};


class Env {
public:
    Env(Env* outer, bool global);

    Env* find(std::string&);

    Type* get(std::string);

    Type* set(std::string&, Type*);

    void setBuiltIns();

private:
    typedef std::map<std::string, Type*> Map;
    Map map;
    Env* outer = nullptr;
};

#endif //CPPLISPINTERPRETER_ENV_H
