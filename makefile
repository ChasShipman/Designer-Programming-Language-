OBJS = evaluator.o type.o lexeme.o lexer.o parser.o environment.o
OOPTS = -Wall -Wextra -std=c99 -g -c
LOPTS = -Wall -Wextra -std=c99 -g

all : evaluator

evaluator : $(OBJS)
	gcc $(LOPTS) $(OBJS) -o eval

evaluator.o : evaluator.c type.h lexeme.h environment.h parser.h lexer.h
	gcc $(OOPTS) evaluator.c

environment.o : environment.c type.h lexeme.h 
	gcc $(OOPTS) environment.c 

parser.o : parser.c type.h lexeme.h lexer.h
	gcc $(OOPTS) parser.c

scanner.o : scanner.c type.h lexeme.h lexer.h
	gcc $(OOPTS) scanner.c

type.o : type.c type.h
	gcc $(OOPTS) type.c

lexeme.o : lexeme.c lexeme.h type.h
	gcc $(OOPTS) lexeme.c

lexer.o : lexer.c lexer.h type.h
	gcc $(OOPTS) lexer.c

clean :
	rm -f $(OBJS) scanner.o pretty.o
	rm -f eval

error1 : all
	@cat tests/error1.txt

error1x : all
	@run tests/error1.txt

error2 : all
	@cat tests/error2.txt

error2x : all
	@run tests/error2.txt

error3 : all
	@cat tests/error3.txt

error3x : all
	@run tests/error3.txt

error4 : all
	@cat tests/error4.txt

error4x : all
	@run tests/error4.txt

error5 : all
	@cat tests/error5.txt

error5x : all
	@run tests/error5.txt

arrays : all
	@cat tests/arrays.txt

arraysx : all
	@run tests/arrays.txt

conditionals : all
	@cat tests/conditionals.txt

conditionalsx : all
	@run tests/conditionals.txt

recursion : all
	@cat tests/recursion.txt

recursionx : all
	@run tests/recursion.txt

iteration : all
	@cat tests/iteration.txt

iterationx : all
	@run tests/iteration.txt

functions : all
	@cat tests/functions.txt

functionsx : all
	@run tests/functions.txt

lambda : all
	@cat tests/lambda.txt

lambdax : all
	@run tests/lambda.txt

objects : all
	@cat tests/objects.txt

objectsx : all
	@run tests/objects.txt

problem : all
	@cat tests/IAVL

problemx : all
	@run tests/IAVL commands


run : all
	@make arrays 
	@make arraysx 
	@make conditionals 
	@make conditionalsx 
	@make recursion 
	@make recursionx 
	@make iteration 
	@make iterationx
	@make functions
	@make functionsx
	@make lambda
	@make lambdax
	@make objects
	@make objectsx
	@make problem
	@make problemx

