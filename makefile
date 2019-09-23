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
	@cat error1.txt

error1x : all
	@run error1.txt

error2 : all
	@cat error2.txt

error2x : all
	@run error2.txt

error3 : all
	@cat error3.txt

error3x : all
	@run error3.txt

error4 : all
	@cat error4.txt

error4x : all
	@run error4.txt

error5 : all
	@cat error5.txt

error5x : all
	@run error5.txt

arrays : all
	@cat arrays.txt

arraysx : all
	@run arrays.txt

conditionals : all
	@cat conditionals.txt

conditionalsx : all
	@run conditionals.txt

recursion : all
	@cat recursion.txt

recursionx : all
	@run recursion.txt

iteration : all
	@cat iteration.txt

iterationx : all
	@run iteration.txt

functions : all
	@cat functions.txt

functionsx : all
	@run functions.txt

lambda : all
	@cat lambda.txt

lambdax : all
	@run lambda.txt

objects : all
	@cat objects.txt

objectsx : all
	@run objects.txt

problem : all
	@cat iAvl

problemx : all
	@run iAvl commands


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

