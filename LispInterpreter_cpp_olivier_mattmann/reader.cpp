
#include "reader.h"
#include <regex>
#include <iostream>

Reader::Reader(std::vector<std::string> &input) {
    currIndex = 0;
    tokens = input;
}
/*
 * Maybe I need to clean up?
 */
Reader::~Reader() = default;

/*
 *  returns the token at the current position and increment the position
*/
std::string Reader::next() {
    if (currIndex >= tokens.size()) {
        throw EOF_Exception();
    }
    return tokens[currIndex++];
}

/*
 *  returns the token at the current position
*/
std::string Reader::peek() {
    if (currIndex >= tokens.size()) {
        throw EOF_Exception();
    }
    return tokens[currIndex];
}

Type* Reader::read_form() {
    auto token = peek();
    //vector used to handle the ' operator (quote)
    //we create a new list that starts with the special form quote and push
    //what follows the '
    std::vector<Type*> vec;
    switch (peek()[0]) {
        case '(':
            //when we encounter an opening bracket we will start reading a list
            next();
            return read_list();
        case '\'':
            next();
            vec.push_back(new SymbolType("quote"));
            vec.push_back(read_form());
            return new ListType(vec);
        default:
            return read_atom();
    }
}

Type* Reader::read_list() {
    ListType* list = new ListType();
    auto token = peek();
    while(true) {
        //while we don't encounter the corresponding closing bracket
        //we call read_form for each element on the list (can be lists aswell)
        if (token == ")") {
            next();
            break;
        }
        list->push(read_form());
        token = peek();
    }
    return list;
}

Type* Reader::read_atom() {
    //first we check what symbol we are dealing with
    std::string atom = peek();
    //currently Integer, Doubles, nil, true and false get recognized
    //if it is none of those we return it as a new Symbol
    if (std::regex_match(atom, std::regex("^[-+]?\\d+$"))) {
        return new IntType(next());
      //regex for double from: https://stackoverflow.com/questions/10516967/regexp-for-a-double
    } else if (std::regex_match(atom, std::regex("^(-?)(0|([1-9][0-9]*))(\\.[0-9]+)?$"))){
        return new DoubleType(next());
    } else {
        if (atom == "nil") {
            next();
            return new NilType();
        } else if (atom == "t") {
            next();
            return new TrueType();
        } else if (atom == "'()") {
            next();
            return new FalseType();
        } else {
                return new SymbolType(next());
        }
    }
}

/*
 * regular expression from MakeALisp Resource:
 * https://github.com/kanaka/mal/blob/master/process/guide.md#step1
 * extracts the tokens form the user input
 * returns them in a vector of strings
 */
std::vector<std::string> tokenize(std::string input) {
    std::regex re(R"([\s,]*(~@|[\[\]{}()'`~^@]|"(?:\\.|[^\\"])*"?|;.*|[^\s\[\]{}('"`,;)]*))");
    std::vector<std::string> tokens(std::sregex_token_iterator(input.begin(), input.end(), re, 1), std::sregex_token_iterator());
    return tokens;
}

Type* read_str(std::string &input) {
    //1. call tokenize
    std::vector<std::string> tokens = tokenize(input);
    //2. create reader instance with tokens from tokenize
    Reader reader = Reader(tokens);
    //3. call read_form with reader instance which will return an AST from the supplied tokens
    return reader.read_form();
}
