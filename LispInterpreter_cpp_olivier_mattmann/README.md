# Lisp Interpreter in C++
## Olivier Mattmann

### Things to keep in mind:

    1) In the Lisp code  true is denoted with 't and false with '() and null with nil
    
    2) nested defines do not work like in racket! for example:
         (define (f n)
            (define (g m) (* 2 m))
            (+ n (g n)))
       doesn't work, but there is a workaround with begin:
        (define (f n)
            (begin (define (g m) (* 2 m))
                   (+ n (g n))))
                   
    3) there is no set! special form, but you can redefine existing Symbol Value pairs 
       in the environment.
       
    4) The homework exercises and the Lisp Interpreter from Graham can be found in the textfiles folder. There were minimal changes done to the code snippets from Graham, namely the expressions of the form (cddr x) were split into (cdr (cdr x))
       
    5) The let* special form is implemented as specified in the MAL-guide. It is defined like this:
       (let* (var1 value1 var2 value2) (body))
       as opposed to the SICP book:
       (let ((var1 value1) (var2 value2)) (body))
       
    6) Small note on the load function: expressions which are in enclosed in parenthesis don't work or won't get recognized. eg. "'a" won't work in when loading it from a textfile. To load files use the command: (load "filepath").    

I have included the compiled files for Windows and Linux (if you are using macOS you may need to delete and recompile with the provided Makefile).