
#ifndef CPPLISPINTERPRETER_READER_H
#define CPPLISPINTERPRETER_READER_H
#include <string>
#include <vector>
#include <optional>
#include <exception>
#include <regex>
#include <iostream>

#include "types.h"

struct EOF_Exception : public std::exception {
    const char * what () const throw () {
        return "Parenthesis are not balanced!";
    }
};

class Reader {
public:
    Reader(std::vector<std::string>&);
    ~Reader();

    /*
     *  returns the token at the current position and increment the position
    */
    std::string next();

    /*
     *  returns the token at the current position
    */
    std::string peek();


    Type* read_form();

    Type* read_list();

    Type* read_atom();

private:
    uint16_t currIndex;
    std::vector<std::string> tokens;
};

std::vector<std::string> tokenize(std::string input);
Type* read_str(std::string &input);
#endif //CPPLISPINTERPRETER_READER_H
