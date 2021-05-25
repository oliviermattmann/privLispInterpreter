#include "printer.h"
//print the resulting AST. Inspect on lists calls inspect on all its
//elements
std::string pr_str(Type* type) {
    return type->inspect();
}