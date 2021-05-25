
#include "Env.h"

Env::Env(Env* outer, bool global) : outer(outer) {
    map = Map();
    if (global) {
        setBuiltIns();
    }
}

Env* Env::find(std::string &symbol) {
    if (map.find(symbol) != map.end()) {
        //if we find the symbol in the environment we just return it
        return this;
    } else {
        //the symbol was not found in the current environment
        //if it is already the outermost/global environment we return null
        if (outer == nullptr) {
            throw EnvException(symbol);
        } else {
            //otherwise we keep looking in the outer environment of the current one
            return outer->find(symbol);
        }
    }
}

Type* Env::get(std::string symbol) {
    //This function behaves like the find function, but returns the expression
    //the symbol is mapped to
    if (map.find(symbol) != map.end()) {
        return map.find(symbol)->second;
    } else {
        if (outer == nullptr) {
            throw EnvException(symbol);
        } else {
            return outer->get(symbol);
        }
    }
}

Type* Env::set(std::string &symbol, Type* type) {
    //here we just add a new pair to the map of the current environment
    //or override it if it already is in the map
    map[symbol] = type;
    return type;
}
/**
 * sets the built-in functions in the environment
 */
void Env::setBuiltIns() {
    std::string plus_sign = "+";
    std::string sub_sign = "-";
    std::string mul_sign = "*";
    std::string div_sign = "/";
    std::string geq_sign = ">=";
    std::string gr_sign = ">";
    std::string leq_sign = "<=";
    std::string lr_sign = "<";
    std::string eq_sign = "eq";
    std::string eval = "eval";
    std::string load_sign = "load";
    std::string parse_sign = "parse";
    std::string car_sign = "car";
    std::string cons_sign = "cons";
    std::string cdr_sign = "cdr";
    std::string list_sign = "list";
    std::string is_null = "null?";
    std::string is_atom = "atom";
    std::string is_number = "number?";
    std::string is_symbol = "symbol?";
    std::string is_pair = "pair?";
    std::string display = "display";

    set(plus_sign, new BuiltInFuncType(add));
    set(sub_sign, new BuiltInFuncType(sub));
    set(mul_sign, new BuiltInFuncType(mul));
    set(div_sign, new BuiltInFuncType(div));
    set(geq_sign,new BuiltInFuncType(geq));
    set(gr_sign, new BuiltInFuncType(gr));
    set(leq_sign, new BuiltInFuncType(leq));
    set(lr_sign, new BuiltInFuncType(lr));
    set(eq_sign, new BuiltInFuncType(eqfunc));
    set(eval, new BuiltInFuncType(eval_func));
    set(car_sign, new BuiltInFuncType(car));
    set(cdr_sign, new BuiltInFuncType(cdr));
    set(list_sign, new BuiltInFuncType(list));
    set(cons_sign, new BuiltInFuncType(cons));
    set(parse_sign, new BuiltInFuncType(parse));
    set(load_sign, new BuiltInFuncType(load));
    set(is_null, new BuiltInFuncType(isNull));
    set(is_atom, new BuiltInFuncType(isAtom));
    set(is_number, new BuiltInFuncType(isNumber));
    set(is_symbol, new BuiltInFuncType(isSymbol));
    set(display, new BuiltInFuncType(display_func));
    set(is_pair, new BuiltInFuncType(isPair));
}

