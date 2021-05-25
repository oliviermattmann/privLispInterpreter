CC = g++

CFLAGS = -g -std=c++17 -Wall

build: cppLispInterpreter

cppLispInterpreter: REPL.cpp reader.cpp printer.cpp Env.cpp builtin_functions.cpp types.cpp
	$(CC) $(CFLAGS) -o $@ $< reader.cpp printer.cpp Env.cpp builtin_functions.cpp types.cpp

