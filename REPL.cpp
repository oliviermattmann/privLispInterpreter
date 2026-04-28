
#include <iostream>
#include <string>
#include "reader.h"
#include "printer.h"
#include "Env.h"

//global environment defined as global variable, is used for the eval built in function
Env *global_env = new Env(nullptr, true);

//Reads in the input and converts it into an abstract syntax tree, to be evaluated
Type* READ(std::string s) {
    return read_str(s);
}


//Forward Declaration of EVAL_AST because it is used in EVAL
Type* EVAL_AST(Type* ast, Env *env);

Type* EVAL(Type* ast, Env *env = global_env) {
    //First we check if the passed AST is of Type List
    //If it is not, then we just evaluate it directly
    //using EVAL_AST
    if (ast->getType() != Types::List) {
        return EVAL_AST(ast, env);
    //if it is an empty list we can just return the empty List
    } else if (DYNAMIC_CAST(ListType, ast)->isEmpty()) {
        return ast;
    } else {
        //first we check for special forms like define or let etc.
        //if we end up here we have a list which is not empty
        //so we can safely cast
        ListType *SymAndParams = DYNAMIC_CAST(ListType, ast);
        //the first element is assumed to be either a special form, variable in the environment or a built-in function
        //there is an exception though, for example: ((lambda (f) (f ‘(b c))) (lambda (x) (cons ‘a x))) is a valid expression
        //where the first element is a list. That the reason for the "workaround" below
        std::string firstSymbol;
        if (SymAndParams->at(0)->getType() == Symbol) {
            //in this case it is a symbol and we can start looking for special forms
            firstSymbol = SymAndParams->at(0)->inspect();
        } else {
            //by setting the first symbol to the empty string we start evaluating the lists at the end of this function
            firstSymbol = "";
        }

        if (firstSymbol == "define" || firstSymbol == "label") {
            //either we set symbol-value pair or define a new function
            //if the second element in the list is a list again we define a new function, if it is a symbol
            //we set a new symbol-value pair
            if (SymAndParams->at(1)->getType() == Symbol) {
                //first we get the symbol key (parameter after define)
                std::string symbolKey = SymAndParams->getList().at(1)->inspect();
                //then we set bind the symbolKey in the current environment to the evaluated second parameter
                return env->set(symbolKey, EVAL(SymAndParams->getList().at(2), env));
            } else {

                //alternative syntax would be (define square (lambda (x) (* x x))), but here we have (define (square x) (* x x))
                //We will create a a new AST to fit the first form. then we can use the same approach as in the return statement above
                ListType* funcAndParams = DYNAMIC_CAST(ListType, SymAndParams->at(1));
                std::string symbolKey = funcAndParams->at(0)->inspect();
                //then we extract the binding Symbols (parameter Symbols) from the expression

                ListType *params = new ListType();
                for (uint32_t i = 1; i < funcAndParams->getList().size(); i++) {
                    params->push(funcAndParams->at(i));
                }
                std::vector<Type*> content;
                content.push_back(new SymbolType("lambda"));
                content.push_back(params);
                content.push_back(SymAndParams->getList().at(2));
                ListType *lambdaExp = new ListType(content);

                return env->set(symbolKey, EVAL(lambdaExp, env));

            }
        } else if (firstSymbol == "defun") {
            //this is an alternative way to define functions, mainly used in the graham article
            //the syntax for this special form is:
            //(defun <function name> (<parameters>) <body>)
            //index 1 = name
            std::string functionName = SymAndParams->at(1)->inspect();
            //index 2 = parameters
            ListType *bindSymbols = DYNAMIC_CAST(ListType, SymAndParams->getList().at(2));
            //index 3 = body
            Type *body = SymAndParams->getList().at(3);
            //the same approach as with the define special form is used, i.e we create a new ast that is using
            //the lambda special form to define the function
            std::vector<Type*> content;
            content.push_back(new SymbolType("lambda"));
            content.push_back(bindSymbols);
            content.push_back(body);
            ListType *lambdaExp = new ListType(content);

            return env->set(functionName, EVAL(lambdaExp, env));

        } else if (firstSymbol == "let*") {
            //let creates a new environment with the current environment as the outer environment
            Env *innerEnv = new Env(env, false);
            //then we get the bindings and set them in the new environment
            ListType *bindings = DYNAMIC_CAST(ListType, SymAndParams->getList().at(1));
            for (uint32_t i = 0; i < bindings->getList().size()-1; i+=2) {
                //the bindings are structured like this (bindSymbol1 bindValue1 bindSymbol2 bindValue2 ...)
                std::string bindSymbol = bindings->getList().at(i)->inspect();
                //then we bind the bindSymbol to the evaluated bindValue in our new environment.
                //We pass the innerEnv into EVAL because later bindValues can depend on earlier bindSymbols
                innerEnv->set(bindSymbol, EVAL(bindings->getList().at(i+1), innerEnv));
            }
            //after we added all the new binding to the new environment we call the body of the let
            //with this new environment
            return EVAL(SymAndParams->getList().at(2), innerEnv);
        } else if (firstSymbol == "lambda") {
                //First we extract the binding Symbols (parameter Symbols) from the expression
                ListType *bindSymbols = DYNAMIC_CAST(ListType, SymAndParams->getList().at(1));
                //Then the Body of the lambda function is extracted
                Type *body = SymAndParams->getList().at(2);
                //Finally a new LambdaFunctionType is created, this type holds a pointer to the parameter, body and outer
                //environment (environment when this Type is created)
                return new LambdaFuncType(bindSymbols, body, env);
        } else if (firstSymbol == "quote") {
            //for the special form quote we just return the following expression without evaluating it
            return SymAndParams->at(1);
        } else if (firstSymbol == "begin") {
            //begin evaluates all following expressions but only returns the result of the last evaluation
            for (uint32_t i = 1; i < SymAndParams->getList().size()-1; i++) {
                EVAL(SymAndParams->at(i), env);
            }
            return EVAL(SymAndParams->at(SymAndParams->getList().size()-1),env);
        } else if (firstSymbol == "if") {
            //first we check if the condition is true (in list with index 1)
            bool isTrue = EVAL(SymAndParams->at(1),env)->getType() == True;
            //if only the body for a true evaluation is supplied, but we receive false from the
            //evaluation then we return Nil
            if (!isTrue && SymAndParams->getList().size() == 3) {
                return new NilType();
            }
            //evaluating the expression depending on the result of the condition
            if (isTrue) {
                return EVAL(SymAndParams->at(2),env);
            } else {
                return EVAL(SymAndParams->at(3), env);
            }
        } else if (firstSymbol == "cond") {
            //we iterate over the lists after cond
            //once the condition in a list is true, we execute/eval the body
            uint32_t index;
            for (index = 1; index < SymAndParams->getList().size(); index++) {
                ListType *condOptions = DYNAMIC_CAST(ListType,SymAndParams->at(index));
                if (EVAL(condOptions->at(0),env)->getType() == True) {
                    return EVAL(DYNAMIC_CAST(ListType,SymAndParams->at(index))->at(1),env);
                }
            }
            //we should never end up here in a correct lisp program
            throw "no cond option matched!";
            return new SymbolType("");
        } else {
                //If we end up here we are not dealing with a special form
                //The options are: Built-in function , LambdaFuncType
                //ast is still a list though, where the first symbol is either a symbol mapped to a built-in function or
                // lambda function in the environment, to continue we need to evaluate the ast
                Type* func_list = EVAL_AST(ast, env);
                if (DYNAMIC_CAST(ListType, func_list)->at(0)->getType() == BuiltInFunction) {
                    //The BuiltInFuncType holds a function pointer
                    BuiltInFuncType *func = DYNAMIC_CAST(BuiltInFuncType, DYNAMIC_CAST(ListType, func_list)->at(0));
                    //First we cast func_list to a ListType to access the internal vector
                    std::vector<Type*> parameters = DYNAMIC_CAST(ListType, func_list)->getList();
                    //then we erase the BuiltInFuncType from the beginning of this list to only have the parameters
                    parameters.erase(parameters.begin());
                    //finally we return the result of the function called with the parameters
                    return func->getFunctionPointer()(parameters);
                } else {
                    //First we cast func_list to a ListType to access the internal vector
                    ListType* lambdaList = DYNAMIC_CAST(ListType, func_list);
                    //the first element will be a LambdaFuncType
                    LambdaFuncType* lambdaFunc = DYNAMIC_CAST(LambdaFuncType, lambdaList->at(0));
                    //the lambdaFunc contains the symbols to bind before execution
                    ListType* bindings = lambdaFunc->bindings;
                    //a new environment gets created to set the bindings
                    Env* newEnv = new Env(lambdaFunc->env, false);
                    for (uint32_t i = 1; i < lambdaList->getList().size(); i++) {
                        //then we iterate over the provided parameters to the lambda function
                        //and bind them to the respective bind symbols
                        std::string bindSymbol = bindings->getList().at(i-1)->inspect();
                        newEnv->set(bindSymbol, lambdaList->getList().at(i));
                    }
                    //Finally we evaluate the body with our environment holding the needed values
                    return EVAL(lambdaFunc->body, newEnv);
                }

        }
    }
    return nullptr;
}


Type* EVAL_AST(Type* ast, Env *env) {
    switch (ast->getType()) {
        case Types::Symbol:
            //workaround for the built-in load function, when we find a symbol surrounded by quotation marks
            //we just return the ast (Symbol) as it is, instead of looking it up in the environment
            if(ast->inspect()[0] == '"' && ast->inspect()[ast->inspect().length()-1] == '"') {
                return ast;
            }
            //otherwise we get the value of the Symbol from the environment
            return env->get(ast->inspect());
        case Types::List: {
            //when evaluating a list we iterate over the elements of the list and push the evaluated elements to a new
            //list and return that
            ListType *out = new ListType();
            for (auto val : DYNAMIC_CAST(ListType, ast)->getList()) {
                out->push(EVAL(val, env));
            }
            return out;
        }
        //if we are not dealing with a list or symbol we can just return the AST as is
        default:
            return ast;
    }
}

std::string PRINT(Type* expression) {
    return pr_str(expression);
}


std::string REPL(std::string input, Env *env) {
    //we first read/parse the input, then evaluate the generated AST with the provided environment
    //and after the evaluation we print the resulting expression
    return PRINT(EVAL(READ(input), env));
}

int main(){

    //start Read-Eval-Print-Loop
    std::string input;
    while (true) {
        std::cout << "user>";
        if (!std::getline(std::cin, input)) {
            break;
        }
        if (input == "quit") {
            break;
        }
        try  {
            std::cout << REPL(input, global_env) << std::endl;
        } catch (EOF_Exception &e) {
            std::cout << e.what() << std::endl;
        } catch (EnvException &e) {
            std::cout << e.get_symbol() << e.what() << std::endl;
        } catch (BuiltInException &e) {
            std::cout << e.what() << e.get_symbol() << std::endl;
        } catch (const char* e) {
            std::cout << e << std::endl;
        }
    }
    return 0;
}
