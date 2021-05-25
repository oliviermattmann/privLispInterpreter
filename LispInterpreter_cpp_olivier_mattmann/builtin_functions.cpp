
#include "builtin_functions.h"

/**
 * + function
 * @param args expects at least 2 arguments
 * @return new Double or Integer Type with the sum as value
 */
Type* add(std::vector<Type*> args) {
    Types returnType = Integer;
    double sum = 0;
    for (auto values : args) {
        if (values->getType() == Types::Integer) {
            sum += DYNAMIC_CAST(IntType, values)->getVal();
        } else if (values->getType() == Types::Double) {
            sum += DYNAMIC_CAST(DoubleType, values)->getVal();
            returnType = Types::Double;
        }
    }
    if (returnType == Double) {
        return  new DoubleType(std::to_string(sum));
    } else {
        return new IntType(std::to_string(sum));
    }
}

/**
 * * function
 * @param args expects at least 2 arguments
 * @return Integer or Double Type with the product as value
 */
Type* mul(std::vector<Type*> args) {
    Types returnType = Integer;
    double prod = 1;
    for (auto values : args) {
        if (values->getType() == Types::Integer) {
            prod *= DYNAMIC_CAST(IntType, values)->getVal();
        } else if (values->getType() == Types::Double) {
            prod *= DYNAMIC_CAST(DoubleType, values)->getVal();
            returnType = Types::Double;
        }
    }
    if (returnType == Double) {
        return  new DoubleType(std::to_string(prod));
    } else {
        return new IntType(std::to_string(prod));
    }
}

/**
 * / function
 * @param args expects 2 arguments
 * @return DoubleType with the quotient as value
 */
Type* div(std::vector<Type*> args) {
    if (args.size() != 2) {
        throw BuiltInException("div, expected 2 argument, received " + std::to_string(args.size()));
    }
    double dend;
    double isor;
    if (args.at(0)->getType() == Double || args.at(1)->getType() == Double) {
        DoubleType *dividend = DYNAMIC_CAST(DoubleType, args.at(0));
        dend = dividend->getVal();
    } else {
        IntType *dividend = DYNAMIC_CAST(IntType, args.at(0));
        dend = dividend->getVal();
    }
    if (args.at(1)->getType() == Double) {
        DoubleType *divisor = DYNAMIC_CAST(DoubleType, args.at(1));
        isor = divisor->getVal();
    } else {
        IntType *divisor = DYNAMIC_CAST(IntType, args.at(1));
        isor = divisor->getVal();
    }
    if (isor == 0) {
            std::cerr << "cannot divide by 0" << std::endl;
    }
    return new DoubleType(std::to_string(dend/isor));
}

/**
 * - function
 * @param args expects at least 2 arguments
 * @return Double or Integer Type with minuend as value
 */
Type* sub(std::vector<Type*> args) {
    //subtraction in lisp works like this
    //(- 1 2 3 4) -> (- 1 (+ 2 3 4)) -> -8
    Types returnType = Integer;
    double minuend;
    if (args[0]->getType() == Integer) {
        minuend = DYNAMIC_CAST(IntType,args[0])->getVal();
    } else  {
        minuend = DYNAMIC_CAST(DoubleType,args[0])->getVal();
    }

    for (uint32_t i = 1; i < args.size(); i++) {

        if (args[i]->getType() == Types::Integer) {
            minuend -= DYNAMIC_CAST(IntType, args[i])->getVal();
        } else if (args[i]->getType() == Types::Double) {
            minuend -= DYNAMIC_CAST(DoubleType, args[i])->getVal();
            returnType = Types::Double;
        }
    }
    if (returnType == Double) {
        return  new DoubleType(std::to_string(minuend));
    } else {
        return new IntType(std::to_string(minuend));
    }
}

/**
 * eval function
 * @param args expects 1 argument to evaluate
 * @return evaluated argument
 */
Type* eval_func(std::vector<Type*> args) {
    if (args.size() != 1) {
        throw BuiltInException("eval, expected 1 argument, received " + std::to_string(args.size()));
    }
    return EVAL(args[0], global_env);
}

/**
 * cons function
 * @param args expects 2 arguments
 * @return a ListType
 */
Type* cons(std::vector<Type*> args) {
    if (args.size() != 2) {
        throw BuiltInException("cons, expected 2 argument, received " + std::to_string(args.size()));
    }
    auto first = args[0];
    auto rest = args[1];
    if (rest->getType() == Nil) {
        return first;
    }
    ListType* consList = new ListType;
    consList->push(first);
    if (rest->getType() == List) {
        ListType *restList = DYNAMIC_CAST(ListType, rest);
        for (uint32_t i = 0; i < restList->getList().size(); i++) {
            consList->push(restList->at(i));
        }
    } else {
        consList->push(rest);
    }
    return consList;
}

/**
 * car function
 * @param args expects 1 ListType
 * @return head of that list
 */
Type* car(std::vector<Type*> args) {
    if (args.size() != 1) {
        throw BuiltInException("car, expected 1 argument, received " + std::to_string(args.size()));
    }
    ListType *list = DYNAMIC_CAST(ListType, args[0]);
    if (!list) {
        return args[0];
    }
    if (list->isEmpty()) {
        return new NilType();
    }
    return list->at(0);
}

/**
 * cdr function
 * @param args expects 1 ListType
 * @return Tail of that list
 */
Type* cdr(std::vector<Type*> args) {
    if (args.size() != 1) {
        throw BuiltInException("cdr, expected 1 argument, received " + std::to_string(args.size()));
    }
    ListType *cdr = new ListType;
    ListType *list = DYNAMIC_CAST(ListType, args[0]);
    if (!list || list -> getList().size() < 2) {
        return new NilType();
    }
    for (uint32_t i = 1; i < list->getList().size(); i++) {
        cdr->push(list->at(i));
    }
    return cdr;
}

/**
 * load function, evaluates all expressions in file
 * @param args SymbolType with filename/path as value
 * @return Emyty SymbolType
 */
Type* load(std::vector<Type*> args) {
    if (args.size() != 1) {
        throw BuiltInException("load, expected 1 argument, received " + std::to_string(args.size()));
    }
    std::string filename = args[0]->inspect();
    //remove quotes
    filename.erase(filename.begin());
    filename.erase(filename.end()-1);
    std::ifstream f(filename); //taking file as inputstream
    std::string str;
    if(f) {
        std::ostringstream ss;
        ss << f.rdbuf(); // reading data
        str = ss.str();
        f.close();
    }

    str = std::regex_replace(str, std::regex("\n"), "");
    //\n is enough for windows, for linux i also need to replace the \r's
    str = std::regex_replace(str, std::regex("\r"), "");

    //create a vector with all the different expressions found in the file
    bool firstBracketread = false;
    std::vector<std::string> vector;
    int numOpeningBrackets = 0;
    int numClosingBrackets = 0;
    uint16_t start = 0;
    uint16_t end;
    for (end = 0; end < str.size(); end++) {
        if (str[end] == '(') {
            firstBracketread = true;
            numOpeningBrackets++;
        } else if (str[end] == ')') {
            numClosingBrackets ++;
        }
        if (numClosingBrackets == numOpeningBrackets && firstBracketread) {
            vector.push_back(str.substr(start, end-start+1));
            start = end+1;
        }

    }
    //if there is only one expression we convert it into an ast and return it
    //like this the load function can be used inline.
    if (vector.size() == 1) {
        return  EVAL(read_str(vector.at(0)), global_env);
    } else {
        //otherwise we just call repl on all the expressions
        for (auto exp : vector) {
            std::cout << REPL(exp, global_env) << std::endl;
        }
        return new SymbolType("");
    }
}

/**
 * parses a string
 * @param args
 * @return
 */
Type* parse(std::vector<Type*> args) {
    std::string sexp_string = DYNAMIC_CAST(SymbolType, args[0])->symStr();
    sexp_string.erase(sexp_string.begin());
    sexp_string.erase(sexp_string.end()-1);
    return new SymbolType(sexp_string);
}

/**
 * creates a new ListType from the arguments
 * @param args vector of elements
 * @return new ListType
 */
Type* list(std::vector<Type*> args) {
    return new ListType(args);
}

Type* geq(std::vector<Type*> args) {
    if (args.size() != 2) {
        throw BuiltInException(">=, expected 2 argument, received " + std::to_string(args.size()));
        return new NilType();
    }
    double first = std::stod(args.at(0)->inspect());
    double second = std::stod(args.at(1)->inspect());
    if (first >= second) {
        return new TrueType();
    } else {
        return new FalseType();
    }
}

Type* gr(std::vector<Type*> args) {
    if (args.size() != 2) {
        throw BuiltInException(">, expected 2 argument, received " + std::to_string(args.size()));
    }
    double first = std::stod(args.at(0)->inspect());
    double second = std::stod(args.at(1)->inspect());
    if (first > second) {
        return new TrueType();
    } else {
        return new FalseType();
    }
}


Type* leq(std::vector<Type*> args) {
    if (args.size() != 2) {
        throw BuiltInException("<=, expected 2 argument, received " + std::to_string(args.size()));

    }
    double first = std::stod(args.at(0)->inspect());
    double second = std::stod(args.at(1)->inspect());
    if (first <= second) {
        return new TrueType();
    } else {
        return new FalseType();
    }
}

/**
 * < function
 * @param args expects number arguments
 * @return
 */
Type* lr(std::vector<Type*> args) {
    if (args.size() != 2) {
        throw BuiltInException("<, expected 2 argument, received " + std::to_string(args.size()));
    }
    //TODO check if they are numbers
    double first = std::stod(args.at(0)->inspect());
    double second = std::stod(args.at(1)->inspect());
    if (first < second) {
        return new TrueType();
    } else {
        return new FalseType();
    }
}

/**
 * eq function
 * @param args expects 2 arguments to be compared
 * @return TrueType when equal, FalseType otherwise
 */
Type* eqfunc(std::vector<Type*> args) {
    if (args.size() != 2) {
        throw BuiltInException("eq, expected 2 argument, received " + std::to_string(args.size()));
    }
    if (args.at(0)->inspect() == args.at(1)->inspect()) {
        return new TrueType();
    } else {
        return new FalseType();
    }
}

/**
 * null? function
 * @param args expects 1 argument
 * @return TrueType if argument is NullType, FalseType otherwise
 */
Type* isNull(std::vector<Type*> args) {
    if (args.at(0)->getType() == Nil) {
        return new TrueType();
    } else {
        return new FalseType();
    }
}
/**
 * atom function
 * @param args expects 1 argument
 * @return TrueType if argument is not a list or is an empty list, FalseType otherwise
 */
Type* isAtom(std::vector<Type*> args) {
    Types type = args.at(0)->getType();
    if (type != List || DYNAMIC_CAST(ListType, args.at(0))->isEmpty()) {
        return new TrueType();
    } else {
        return new FalseType();
    }
}

/**
 * number? function
 * @param args expects 1 argument
 * @return TrueType if argument is a Double or Integer, FalseType otherwise
 */
Type* isNumber(std::vector<Type*> args) {
    if (args.at(0)->getType() == Double || args.at(0)->getType() == Integer) {
        return new TrueType();
    } else {
        return new FalseType();
    }
}

/**
 * symbol? function
 * @param args expects 1 argument
 * @return TrueType if argument is a Symbol, FalseType otherwise
 */
Type* isSymbol(std::vector<Type*> args) {
    if (args.at(0)->getType() == Symbol) {
        return new TrueType();
    } else {
        return new FalseType();
    }
}

/**
 * pair? function
 * @param args expects 1 argument
 * @return TrueType if argument is a List, FalseType otherwise
 */
Type* isPair(std::vector<Type*> args) {
    if (args.at(0)->getType() == List) {
        return new TrueType();
    } else {
        return new FalseType();
    }
}

/**
 * display function (prints argument to terminal)
 * @param args expects 1 argument
 * @return empty SymbolType
 */
Type* display_func(std::vector<Type*> args) {
    std::cout << args.at(0)->inspect() << std::endl;
    return new SymbolType("");
}