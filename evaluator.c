/*
    @author: Chas Shipman   Feb 2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "lexeme.h"
#include "parser.h"
#include "type.h"
#include "evaluator.h"
#include "environment.h"

//BUILTINS
static LEXEME *evalPrint(LEXEME *args);
static LEXEME *evalNewArray(LEXEME *args);
static LEXEME *evalSetArray(LEXEME *args);
static LEXEME *evalGetArray(LEXEME *args);
static LEXEME *evalOpen(LEXEME *args);
static LEXEME *evalReadInteger(LEXEME *args);
static LEXEME *evalAtFileEnd(LEXEME *args);
static LEXEME *evalCloseFile(LEXEME *args);
static LEXEME *evalGetArgCount();
static LEXEME *evalGetArg(LEXEME *args);

static LEXEME *evalBlock(LEXEME *tree, LEXEME *env);
static LEXEME *evalStatementList(LEXEME *tree, LEXEME *env);
static LEXEME *evalArgs(LEXEME *tree, LEXEME *env);

static LEXEME *evalProgram(LEXEME *tree, LEXEME *env);
static LEXEME *evalStatement(LEXEME *tree, LEXEME *env);
static LEXEME *evalVarDef(LEXEME *tree, LEXEME *env);

static LEXEME *evalFuncDef(LEXEME *tree, LEXEME *env);
static LEXEME *getFuncDefParams(LEXEME *tree);
static LEXEME *getFuncDefBody(LEXEME *tree);
static LEXEME *getFuncDefName(LEXEME *tree);

static LEXEME *evalLambdaDef(LEXEME *tree, LEXEME *env);

static LEXEME *evalFuncCall(LEXEME *tree, LEXEME *env);
static LEXEME *getParams(LEXEME *closure);
static LEXEME *getBody(LEXEME *closure);
static LEXEME *evalParams(LEXEME *tree, LEXEME *env);

static LEXEME *evalClassDef(LEXEME *tree, LEXEME *env);
static LEXEME *evalConstructor(LEXEME *closure);

static LEXEME *evalIf(LEXEME *tree, LEXEME *env);
static LEXEME *evalWhile(LEXEME *tree, LEXEME *env);
static LEXEME *evalOptElse(LEXEME *tree, LEXEME *env);


static LEXEME *evalUoperator(LEXEME *tree, LEXEME *env);
static LEXEME *evalSimpleOp(LEXEME *tree, LEXEME *env);
static LEXEME *evalPlus(LEXEME *left, LEXEME *right);
static LEXEME *evalMinus(LEXEME *left, LEXEME *right);
static LEXEME *evalTimes(LEXEME *left, LEXEME *right);
static LEXEME *evalDivision(LEXEME *left, LEXEME *right);
static LEXEME *evalExponent(LEXEME *left, LEXEME *right);
static LEXEME *evalModulo(LEXEME *left, LEXEME *right);
static LEXEME *evalEqualTo(LEXEME *left, LEXEME *right);
static LEXEME *evalNotEqualTo(LEXEME *left, LEXEME *right);
static LEXEME *evalGreaterThan(LEXEME *left, LEXEME *right);
static LEXEME *evalGreaterThanOrEqaulTo(LEXEME *left, LEXEME *right);
static LEXEME *evalLessThan(LEXEME *left, LEXEME *right);
static LEXEME *evalLessThanOrEqualTo(LEXEME *left, LEXEME *right);
static LEXEME *evalAnd(LEXEME *left, LEXEME *right);
static LEXEME *evalOr(LEXEME *left, LEXEME *right);
static LEXEME *evalDot(LEXEME *left, LEXEME *right);
static LEXEME *evalEquals(LEXEME *left, LEXEME *right, LEXEME *env);
static LEXEME *evalPlusEquals(LEXEME *left, LEXEME *right, LEXEME *env);
static LEXEME *evalMinusEquals(LEXEME *left, LEXEME *right, LEXEME *env);
static LEXEME *evalTimesEquals(LEXEME *left, LEXEME *right, LEXEME *env);
static LEXEME *evalDivisionEquals(LEXEME *left, LEXEME *right, LEXEME *env);
static LEXEME *evalExponentEquals(LEXEME *left, LEXEME *right, LEXEME *env);
static LEXEME *evalModuloEquals(LEXEME *left, LEXEME *right, LEXEME *env);

static double getValue(LEXEME *lex);

int argCount;
char **argVector;
void initArgVars(int argc, char **argv);//will pass to main;

int main(int argc, char **argv){
    assert(argc = 2);
    LEXEME *env = createEnv();
    insertEnv(newLexString(ID, "print"), newLexFunction(BUILTIN, evalPrint),env);
    insertEnv(newLexString(ID, "newArray"), newLexFunction(BUILTIN, evalNewArray),env);
	insertEnv(newLexString(ID, "setArray"), newLexFunction(BUILTIN, evalSetArray),env);
	insertEnv(newLexString(ID, "getArray"), newLexFunction(BUILTIN, evalGetArray),env);
	
	insertEnv(newLexString(ID, "open"), newLexFunction(BUILTIN, evalOpen),env);
	insertEnv(newLexString(ID, "readInteger"), newLexFunction(BUILTIN, evalReadInteger),env);
	insertEnv(newLexString(ID, "atFileEnd"), newLexFunction(BUILTIN, evalAtFileEnd),env);
	insertEnv(newLexString(ID, "closeFile"), newLexFunction(BUILTIN, evalCloseFile),env);
	insertEnv(newLexString(ID, "getArgCount"), newLexFunction(BUILTIN, evalGetArgCount),env);
	insertEnv(newLexString(ID, "getArg"), newLexFunction(BUILTIN, evalGetArg),env);
    //seed functions here
    LEXEME *tree = parser(argv[1]);
    initArgVars(argc,argv);
	eval(tree,env);
	return 0;
}

void initArgVars(int argc, char **argv){
	argCount = argc;
	argVector = argv;
	return;
}

LEXEME *eval(LEXEME *tree, LEXEME *env) {
	char *type = getType(tree);

	//printf("TYPE: %s\n", type);
	if(type == INTEGER || type == STRING || type == CHARACTER || type == REAL) {
		return tree;
	}
	else if(type == ID) {
		return lookupEnv(tree,env);
	}
	else if(type == PLUS || type == MINUS || type == TIMES || type == DIVISION
		|| type == EXPONENT || type == MODULO || type == EQUAL_TO || type == NOT_EQUAL_TO
		|| type == GREATER_THAN || type == GREATER_THAN_OR_EQUAL_TO || type == LESS_THAN
		|| type == LESS_THAN_OR_EQUAL_TO || type == AND || type == OR || type == DOT
		|| type == EQUALS || type == PLUS_EQUALS || type == MINUS_EQUALS
		|| type == TIMES_EQUALS || type == DIVISION_EQUALS || type == MODULO_EQUALS
		|| type == EXPONENT_EQUALS) {
		return evalSimpleOp(tree,env);
	}
	else if(type == UNARY_UOP) {
		return evalUoperator(tree,env);
	}
	else if(type == VARDEF) {
		return evalVarDef(tree,env);
	}
	else if(type == FUNCDEF) {
		return evalFuncDef(tree,env);
	} 
	else if(type == CLASSDEF) {
		return evalClassDef(tree,env);
	} 
	else if(type == LAMBDADEF) {
		return evalLambdaDef(tree,env);
	}
	else if(type == IFSTATE) {
		evalIf(tree,env);
	}
	else if(type == POPTELSE) {
		evalOptElse(tree,env);
	}
	else if(type == WHILELOOP) {
		evalWhile(tree,env);
	}
	else if(type == FUNCCALL) return evalFuncCall(tree,env);
	else if(type == PBLOCK) return evalBlock(tree,env);
	else if(type == PROGRAM) return evalProgram(tree,env);
	else if(type == STATEMENT) return evalStatement(tree,env);
	else if(type == EXPR_LIST) return evalArgs(tree,env); //might not be right
	else if(type == PARAM_LIST) return evalParams(tree,env);
	else if(type == STATEMENT_LIST) return evalStatementList(tree,env);
	else {
		fprintf(stderr, "FATAL BAD TYPE: %s",getType(tree));
		//return newLexInt(INTEGER,1);
    	exit(1);
	}
	return newLexInt(INTEGER,0);
}

//change this function
static LEXEME *evalPrint(LEXEME *args) {
	while(args != NULL) {
		displayLex(car(args));
		args = cdr(args);
	}
	return newLexString(STRING,	"END OF THE LINE BUCKO ");
}

static LEXEME *evalNewArray(LEXEME *args){
	if(car(args) == NULL){
		printf("Missing arg in function newArray\n");
		exit(1);
	}
	else if(getType(car(args)) != INTEGER) {
		printf("Argument type is not INTEGER\n");
		exit(1);
	}
	int size = getInt(car(args));
	LEXEME *array = newLexArray(size);
	return array;
}

static LEXEME *evalSetArray(LEXEME *args){
	LEXEME *tempArgs = args;
	int count = 0;
	while(tempArgs != NULL){
		count++;
		tempArgs = cdr(tempArgs);
	}

	if(count != 3){
		printf("Incorrect arg count in function getArray\n");
		exit(1);
	}
	LEXEME *array= car(args);
	LEXEME *index = car(cdr(args));
	if(getType(index) != INTEGER || getInt(index) >= getArraySize(array)){
		printf("out of bounds access in array\n");
		exit(1);
	}

	LEXEME *val = car(cdr(cdr(args)));
	if(getType(array) != ARRAY || getType(index) != INTEGER){
		printf("incorrect args for set array: setArray(array, integer, value)\n");
		exit(1);
	}

	setArrayVal(array, index, val);
	return val;
}

static LEXEME *evalGetArray(LEXEME *args){
	LEXEME *tempArgs = args;
	int count = 0;
	while(tempArgs != NULL){
		count++;
		tempArgs = cdr(tempArgs);
	}

	if(count != 2){
		printf("Incorrect arg count in function getArray\n");
		exit(1);
	}
	LEXEME *array= car(args);
	LEXEME *index = car(cdr(args));
	if(getInt(index) >= getArraySize(array)){
		printf("out of bounds access in array\n");
		exit(1);
	}

	if(getType(array) != ARRAY || getType(index) != INTEGER){
		printf("incorrect args for get array: setArray(array, integer, value)\n");
		exit(1);
	}

	LEXEME *result = getArrayVal(array, index);
	if(result == NULL) return NULL; //notsure
	return result;
}

static LEXEME *evalOpen(LEXEME *args){
	LEXEME *fileName = car(args);
	if(getType(fileName) != STRING){
		printf("arg must be string in Open\n");
		exit(1);
	}
	LEXEME *filePtr = newLexFile(getString(fileName));
	return filePtr;
}

static LEXEME *evalReadInteger(LEXEME *args){
	if(getType(car(args)) != FILEPTR){
		printf("arg must be FILE_PTR in readInteger\n");
		exit(1);
	}
	FILE *fp = getFptr(car(args));
	int x;
	fscanf(fp, "%d", &x);
	return newLexInt(INTEGER, x);
}

static LEXEME *evalAtFileEnd(LEXEME *args){
	if(getType(car(args)) != FILEPTR){
		printf("arg must be FILEPTR in atFileEnd\n");
		exit(1);
	}
	FILE *fp = getFptr(car(args));
	if(feof(fp)) return newLexInt(INTEGER,1); //newLexeme(TRUE)? should I make a new Lexeme for these?
	else return newLexInt(INTEGER,0); //newLexeme(TRUE)? new lexeme only or add boolean field to lex constructor and add boolean constructors?
	//made new lexeme + added field + made new bool constructor.
}

static LEXEME *evalCloseFile(LEXEME *args){
	if(getType(car(args)) != FILEPTR){
		printf("arg must be FILEPTR in closeFile\n");
		exit(1);
	}

	FILE *fp = getFptr(car(args));
	fclose(fp);
	return newLexInt(INTEGER,1);
}

static LEXEME *evalGetArgCount(){
	return newLexInt(INTEGER, argCount);
}

static LEXEME *evalGetArg(LEXEME *args){
	if(getType(car(args)) != INTEGER){
		printf("args for GetArg must be INTEGERS\n");
		exit(1);
	}
	LEXEME *argIndex = car(args);
	return newLexString(STRING, argVector[getInt(argIndex)]);
}

static LEXEME *evalProgram(LEXEME *tree, LEXEME *env) {
	while(tree != NULL) {
		LEXEME *result = eval(car(tree),env);
		if(getType(result) == RETURNED) {
			return result;
		}
		tree = cdr(tree);
	}
	return tree;
}

static LEXEME *evalStatement(LEXEME *tree, LEXEME *env) {
	return eval(car(tree),env);
}


static LEXEME *evalVarDef(LEXEME *tree, LEXEME *env) {
	LEXEME *left = car(tree);
	if(car(cdr(tree)) == NULL) { //checking if init is null
		insertEnv(car(tree),NULL,env);
		return NULL;
	} 
	else {
		LEXEME *right = eval(cdr(cdr(tree)),env);
		if(getType(left) == ID) {
			insertEnv(left,right,env);
		}
		else if(getType(left) == DOT) {
			LEXEME *object = eval(car(left),env);
			updateEnv(cdr(left),right,object);
		}
		else {
			fprintf(stderr, "Bad assignment");
		}
		return right;
	}
}

static LEXEME *evalFuncDef(LEXEME *tree, LEXEME *env) {
	LEXEME *closure = 
		cons(CLOSURE, env,
			cons(JOIN,getFuncDefParams(tree),
				cons(JOIN,getFuncDefBody(tree),
					NULL)));
	insertEnv(getFuncDefName(tree), closure,env);
	return closure;
}

static LEXEME *evalLambdaDef(LEXEME *tree, LEXEME *env) {
	LEXEME *closure = 
		cons(CLOSURE, env,
			cons(JOIN,getFuncDefParams(tree),
				cons(JOIN,getFuncDefBody(tree),
					NULL)));
	return closure;
}
static LEXEME *getFuncDefParams(LEXEME *tree) {
	return car(cdr(tree));
}

static LEXEME *getFuncDefBody(LEXEME *tree) {
	return cdr(cdr(tree));
}

static LEXEME *getFuncDefName(LEXEME *tree) {
	return car(tree);
}

static LEXEME *evalFuncCall(LEXEME *tree,LEXEME *env) {
	//printf("FUNC CALL\n");
	LEXEME *closure = lookupEnv(car(tree),env);
	LEXEME *args = evalArgs(cdr(tree),env);
	if(getType(closure) == BUILTIN) 
		return getFunction(closure)(args);
	//double check on below line
	else if(getType(closure) == OCLOSURE) 
		return evalConstructor(closure);
	LEXEME *senv = car(closure);
	LEXEME *params = getParams(closure);
	LEXEME *xenv = extendEnv(params,args,senv);
	LEXEME *body = getBody(closure);
	return eval(body, xenv);
}

static LEXEME *getParams(LEXEME *closure) {
	return car(cdr(closure));
}

static LEXEME *getBody(LEXEME *closure) {
	return car(cdr(cdr(closure)));
}

static LEXEME *evalParams(LEXEME *tree, LEXEME *env) {
	if (tree == NULL) {
        return NULL;
    }
    return cons(GLUE, car(tree), evalParams(cdr(tree), env));
}

//defenitely update this
static LEXEME *evalArgs(LEXEME *tree, LEXEME *env) {
	if (tree == NULL) {
        return NULL;
    }
    return cons(GLUE, eval(car(tree), env), evalArgs(cdr(tree), env));
}

static LEXEME *evalClassDef(LEXEME *tree, LEXEME *env) {
	return insertEnv(car(tree), cons(OCLOSURE,env,tree),env);
}

static LEXEME *evalConstructor(LEXEME *closure) {
	LEXEME *senv = car(closure);
	LEXEME *xenv = extendEnv(NULL, NULL,senv);
	LEXEME *body = cdr(cdr(closure));
	eval(body,xenv);
	return xenv;
}

//these two are identical, block and satement list, idc
static LEXEME *evalBlock(LEXEME *tree, LEXEME *env) {
	LEXEME *result;
	while(tree != NULL) {
		result = eval(car(tree),env);
		tree = cdr(tree);
	}
	return result;
}

static LEXEME *evalStatementList(LEXEME *tree, LEXEME *env) {
	LEXEME *result;
	while(tree != NULL) {
		result = eval(car(tree),env);
		tree = cdr(tree);
	}
	return result;
}

static LEXEME *evalIf(LEXEME *tree, LEXEME *env) {
	LEXEME *result = NULL;
	LEXEME *expr = eval(car(tree),env);
	if((getType(expr) == INTEGER && getInt(expr) != 0 )
		|| (getType(expr) == STRING && !strcmp(getString(expr),"0"))
		|| (getType(expr) == REAL && getReal(expr) != 0)) {
			result = eval(car(cdr(tree)),env);
		} 
	else {
		result = eval(cdr(cdr(tree)),env);
	}
	return result;
}

static LEXEME *evalOptElse(LEXEME *tree, LEXEME *env) {
	if(tree == NULL) {
		return NULL;
	}
	else {
		return eval(car(tree),env);
	}
}

static LEXEME *evalWhile(LEXEME *tree, LEXEME *env) {
	LEXEME *result = NULL;
	LEXEME *expr = eval(car(tree),env);
	while((getType(expr) == INTEGER && getInt(expr) != 0 )
		|| (getType(expr) == STRING && !strcmp(getString(expr),"0"))
		|| (getType(expr) == REAL && getReal(expr) != 0)) {
			result = eval(car(cdr(tree)),env);
			expr = eval(car(tree),env);
		} 
	return result;
}

static LEXEME *evalUoperator(LEXEME *tree, LEXEME *env) {
	char *type = getType(car(tree));
	LEXEME *right = NULL;
	if(type == MINUS) {
		right = eval(cdr(tree),env);
		if(getType(right) == INTEGER) {
			//printf("even here\n");
			return newLexInt(INTEGER, -1 * getInt(right));
		}
		else if(getType(right) == REAL) {
			return newLexReal(REAL, -1 * getReal(right));
		}
		else {
			fprintf(stderr, "negative must only be used on a number ");
			exit(1);
		}
	}
	else if(type == NOT) {
		right = eval(cdr(tree),env);
		double rightValue = getValue(right);
		if(rightValue == 0) {
			return newLexInt(INTEGER,1);
		}
		else {
			return newLexInt(INTEGER,0);
		}
	}
	else if(type == PLUS_PLUS) {
		right = cdr(tree);
		if(getType(right) == ID) {
			LEXEME *holder = lookupEnv(right,env);
			if(getType(holder) == INTEGER) holder = newLexInt(INTEGER, getInt(holder) + 1);
			else if(getType(holder) == REAL) holder = newLexReal(REAL, getReal(right) + 1);
			updateEnv(right,holder,env);
			return holder;
		}
		else {
			right = eval(right,env);
			if(getType(right) == INTEGER) return newLexInt(INTEGER, getInt(right) + 1);
			else if(getType(right) == REAL) return newLexReal(REAL, getReal(right) + 1);
			else {
				fprintf(stderr, "plus pluse must only be used on a number");
				exit(1);
			}
		}
	}
	else if(type == MINUS_MINUS) {
		right = cdr(tree);
		if(getType(right) == ID) {
			LEXEME *holder = lookupEnv(right,env);
			if(getType(holder) == INTEGER) holder = newLexInt(INTEGER, getInt(holder) - 1);
			else if(getType(holder) == REAL) holder = newLexReal(REAL, getReal(right) - 1);
			updateEnv(right,holder,env);
			return holder;
		}
		else {
			right = eval(right,env);
			if(getType(right) == INTEGER) return newLexInt(INTEGER, getInt(right) - 1);
			else if(getType(right) == REAL) return newLexReal(REAL, getReal(right) - 1);
			else {
				fprintf(stderr, "plus pluse must only be used on a number");
				exit(1);
			}
		}
	}
	return right;
}

static LEXEME *evalSimpleOp(LEXEME *tree, LEXEME *env) {
	char *type = getType(tree);
	LEXEME *left = NULL;
	if(!(type == EQUALS || type == PLUS_EQUALS || type == MINUS_EQUALS 
		|| type == TIMES_EQUALS || type == DIVISION_EQUALS || 
		type == MODULO_EQUALS || type == EXPONENT_EQUALS)) {
		left = eval(car(tree),env);
	}
	else left = car(tree);
	LEXEME *right = NULL;
	if(type != DOT) {
		right = eval(cdr(tree),env);
	}
	else {
		right = cdr(tree);
	}
	if(type == PLUS) return evalPlus(left,right);
	else if(type == MINUS) return evalMinus(left,right);
	else if(type == DIVISION) return evalDivision(left,right);
	else if(type == TIMES) return evalTimes(left,right);
	else if(type == EXPONENT) return evalExponent(left,right);
	else if(type == MODULO) return evalModulo(left,right);
	else if(type == EQUAL_TO) return evalEqualTo(left,right);
	else if(type == NOT_EQUAL_TO) return evalNotEqualTo(left,right);
	else if(type == GREATER_THAN) return evalGreaterThan(left,right);
	else if(type == GREATER_THAN_OR_EQUAL_TO) return evalGreaterThanOrEqaulTo(left,right);
	else if(type == LESS_THAN) return evalLessThan(left,right);
	else if(type == LESS_THAN_OR_EQUAL_TO) return evalLessThanOrEqualTo(left,right);
	else if(type == AND) return evalAnd(left,right);
	else if(type == OR) return evalOr(left,right);
	else if(type == DOT) return evalDot(left,right);
	else if(type == EQUALS) return evalEquals(left,right,env);
	else if(type == PLUS_EQUALS) return evalPlusEquals(left,right,env);
	else if(type == MINUS_EQUALS) return evalMinusEquals(left,right,env);
	else if(type == TIMES_EQUALS) return evalTimesEquals(left,right,env);
	else if(type == DIVISION_EQUALS) return evalDivisionEquals(left,right,env);
	else if(type == EXPONENT_EQUALS) return evalExponentEquals(left,right,env);
	else if(type == MODULO_EQUALS) return evalModuloEquals(left,right,env);
	return left;
}

static double getValue(LEXEME *lex) {
	if(getType(lex) == REAL) {
		return getReal(lex);
	}
	else if(getType(lex) == INTEGER) {
		return ((double)getInt(lex));
	}
	else {
		return 0;
	}
}

static LEXEME *evalPlus(LEXEME *left, LEXEME *right) {
	double leftValue = getValue(left);
	double rightValue = getValue(right);
	if(getType(left) == REAL || getType(right) == REAL) {
		return newLexReal(REAL,leftValue+rightValue);
	}
	else {
		return newLexInt(INTEGER,(int)(leftValue+rightValue));
	}
}

static LEXEME *evalMinus(LEXEME *left, LEXEME *right) {
	double leftValue = getValue(left);
	double rightValue = getValue(right);
	if(getType(left) == REAL || getType(right) == REAL) {
		return newLexReal(REAL,leftValue-rightValue);
	}
	else {
		return newLexInt(INTEGER,(int)(leftValue-rightValue));
	}
}

static LEXEME *evalDivision(LEXEME *left, LEXEME *right) {
	double leftValue = getValue(left);
	double rightValue = getValue(right);
	if(getType(left) == REAL || getType(right) == REAL) {
		return newLexReal(REAL,leftValue/rightValue);
	}
	else {
		return newLexInt(INTEGER,(int)(leftValue/rightValue));
	}
}

static LEXEME *evalTimes(LEXEME *left, LEXEME *right) {
	double leftValue = getValue(left);
	double rightValue = getValue(right);
	if(getType(left) == REAL || getType(right) == REAL) {
		return newLexReal(REAL,leftValue*rightValue);
	}
	else {
		return newLexInt(INTEGER,(int)(leftValue*rightValue));
	}
}

static LEXEME *evalExponent(LEXEME *left, LEXEME *right) {
	double leftValue = getValue(left);
	double rightValue = getValue(right);
	if(getType(left) == REAL || getType(right) == REAL) {
		return newLexReal(REAL,pow(leftValue,rightValue));
	}
	else {
		return newLexInt(INTEGER,(int)(pow(leftValue,rightValue)));
	}
}

static LEXEME *evalModulo(LEXEME *left, LEXEME *right) {
	double leftValue = getValue(left);
	double rightValue = getValue(right);
	if(getType(left) == REAL || getType(right) == REAL) {
		return newLexReal(REAL,fmod(leftValue,rightValue));
	}
	else {
		return newLexInt(INTEGER,(int)fmod(leftValue,rightValue));
	}
}

static LEXEME *evalEqualTo(LEXEME *left, LEXEME *right) {
	double leftValue = getValue(left);
	double rightValue = getValue(right);
	if(leftValue == rightValue) {
		return newLexInt(INTEGER,1);
	}
	return newLexInt(INTEGER,0);
}

static LEXEME *evalNotEqualTo(LEXEME *left, LEXEME *right) {
	double leftValue = getValue(left);
	double rightValue = getValue(right);
	if(leftValue != rightValue) {
		return newLexInt(INTEGER,1);
	}
	return newLexInt(INTEGER,0);
}

static LEXEME *evalGreaterThan(LEXEME *left, LEXEME *right) {
	double leftValue = getValue(left);
	double rightValue = getValue(right);
	if(leftValue > rightValue) {
		return newLexInt(INTEGER,1);
	}
	return newLexInt(INTEGER,0);
}

static LEXEME *evalGreaterThanOrEqaulTo(LEXEME *left, LEXEME *right) {
	double leftValue = getValue(left);
	double rightValue = getValue(right);
	if(leftValue >= rightValue) {
		return newLexInt(INTEGER,1);
	}
	return newLexInt(INTEGER,0);
}

static LEXEME *evalLessThan(LEXEME *left, LEXEME *right) {
	double leftValue = getValue(left);
	double rightValue = getValue(right);
	if(leftValue < rightValue) {
		return newLexInt(INTEGER,1);
	}
	return newLexInt(INTEGER,0);
}

static LEXEME *evalLessThanOrEqualTo(LEXEME *left, LEXEME *right) {
	double leftValue = getValue(left);
	double rightValue = getValue(right);
	if(leftValue <= rightValue) {
		return newLexInt(INTEGER,1);
	}
	return newLexInt(INTEGER,0);
}

static LEXEME *evalAnd(LEXEME *left, LEXEME *right) {
	double leftValue = getValue(left);
	double rightValue = getValue(right);
	if(leftValue != 0 && rightValue != 0) {
		return newLexInt(INTEGER,1);
	}
	return newLexInt(INTEGER,0);
}

static LEXEME *evalOr(LEXEME *left, LEXEME *right) {
	double leftValue = getValue(left);
	double rightValue = getValue(right);
	if(leftValue != 0 || rightValue != 0) {
		return newLexInt(INTEGER,1);
	}
	return newLexInt(INTEGER,0);
}

//no idea if im gonna implement this or not
//CHECK AGAIN ON THIS IMPLEMENTATION, OO TAB
static LEXEME *evalDot(LEXEME *left, LEXEME *right) {
	return eval(right,left);
}

static LEXEME *evalEquals(LEXEME *left, LEXEME *right, LEXEME *env) {
	if(getType(left) == ID) {
		updateEnv(left,right,env);
	}
	else if(getType(left) == DOT) {
		LEXEME *object = eval(car(left),env);
		updateEnv(cdr(left),right,object);
	}
	else {
		fprintf(stderr, "Bad assignment");
	}
	return right;
}

static LEXEME *evalPlusEquals(LEXEME *left, LEXEME *right, LEXEME *env) {
	LEXEME *result = NULL;
	if(getType(left) == ID) {
		LEXEME *leftValueLex = lookupEnv(left,env);
		double leftValue = getValue(leftValueLex);
		double rightValue = getValue(right);
		if(getType(left) == REAL || getType(right) == REAL) {
			result = newLexReal(REAL,leftValue+rightValue);
		}
		else {
			result = newLexInt(INTEGER,(int)(leftValue+rightValue));
		}
		updateEnv(left,result,env);
	}
	else if(getType(left) == DOT) {
		LEXEME *object = eval(car(left),env);
		LEXEME *leftValueLex = lookupEnv(cdr(left),object);
		double leftValue = getValue(leftValueLex);
		double rightValue = getValue(right);
		if(getType(left) == REAL || getType(right) == REAL) {
			result = newLexReal(REAL,leftValue+rightValue);
		}
		else {
			result = newLexInt(INTEGER,(int)(leftValue+rightValue));
		}
		updateEnv(cdr(left),result,object);
	}
	else {
		fprintf(stderr, "Bad assignment");
	}
	return result;
}

static LEXEME *evalMinusEquals(LEXEME *left, LEXEME *right, LEXEME *env) {
	LEXEME *result = NULL;
	if(getType(left) == ID) {
		LEXEME *leftValueLex = lookupEnv(left,env);
		double leftValue = getValue(leftValueLex);
		double rightValue = getValue(right);
		if(getType(left) == REAL || getType(right) == REAL) {
			result = newLexReal(REAL,leftValue-rightValue);
		}
		else {
			result = newLexInt(INTEGER,(int)(leftValue-rightValue));
		}
		updateEnv(left,result,env);
	}
	else if(getType(left) == DOT) {
		LEXEME *object = eval(car(left),env);
		LEXEME *leftValueLex = lookupEnv(cdr(left),object);
		double leftValue = getValue(leftValueLex);
		double rightValue = getValue(right);
		if(getType(left) == REAL || getType(right) == REAL) {
			result = newLexReal(REAL,leftValue-rightValue);
		}
		else {
			result = newLexInt(INTEGER,(int)(leftValue-rightValue));
		}
		updateEnv(cdr(left),result,object);
	}
	else {
		fprintf(stderr, "Bad assignment");
	}
	return result;
}

static LEXEME *evalTimesEquals(LEXEME *left, LEXEME *right, LEXEME *env) {
	LEXEME *result = NULL;
	if(getType(left) == ID) {
		LEXEME *leftValueLex = lookupEnv(left,env);
		double leftValue = getValue(leftValueLex);
		double rightValue = getValue(right);
		if(getType(left) == REAL || getType(right) == REAL) {
			result = newLexReal(REAL,leftValue*rightValue);
		}
		else {
			result = newLexInt(INTEGER,(int)(leftValue*rightValue));
		}
		updateEnv(left,result,env);
	}
	else if(getType(left) == DOT) {
		LEXEME *object = eval(car(left),env);
		LEXEME *leftValueLex = lookupEnv(cdr(left),object);
		double leftValue = getValue(leftValueLex);
		double rightValue = getValue(right);
		if(getType(left) == REAL || getType(right) == REAL) {
			result = newLexReal(REAL,leftValue*rightValue);
		}
		else {
			result = newLexInt(INTEGER,(int)(leftValue*rightValue));
		}
		updateEnv(cdr(left),result,object);
	}
	else {
		fprintf(stderr, "Bad assignment");
	}
	return result;
}

static LEXEME *evalDivisionEquals(LEXEME *left, LEXEME *right, LEXEME *env) {
	LEXEME *result = NULL;
	if(getType(left) == ID) {
		LEXEME *leftValueLex = lookupEnv(left,env);
		double leftValue = getValue(leftValueLex);
		double rightValue = getValue(right);
		if(getType(left) == REAL || getType(right) == REAL) {
			result = newLexReal(REAL,leftValue/rightValue);
		}
		else {
			result = newLexInt(INTEGER,(int)(leftValue/rightValue));
		}
		updateEnv(left,result,env);
	}
	else if(getType(left) == DOT) {
		LEXEME *object = eval(car(left),env);
		LEXEME *leftValueLex = lookupEnv(cdr(left),object);
		double leftValue = getValue(leftValueLex);
		double rightValue = getValue(right);
		if(getType(left) == REAL || getType(right) == REAL) {
			result = newLexReal(REAL,leftValue/rightValue);
		}
		else {
			result = newLexInt(INTEGER,(int)(leftValue/rightValue));
		}
		updateEnv(cdr(left),result,object);
	}
	else {
		fprintf(stderr, "Bad assignment");
	}
	return result;
}

static LEXEME *evalExponentEquals(LEXEME *left, LEXEME *right, LEXEME *env) {
	LEXEME *result = NULL;
	if(getType(left) == ID) {
		LEXEME *leftValueLex = lookupEnv(left,env);
		double leftValue = getValue(leftValueLex);
		double rightValue = getValue(right);
		if(getType(left) == REAL || getType(right) == REAL) {
			result = newLexReal(REAL,pow(leftValue,rightValue));
		}
		else {
			result = newLexInt(INTEGER,(int)(pow(leftValue,rightValue)));
		}
		updateEnv(left,result,env);
	}
	else if(getType(left) == DOT) {
		LEXEME *object = eval(car(left),env);
		LEXEME *leftValueLex = lookupEnv(cdr(left),object);
		double leftValue = getValue(leftValueLex);
		double rightValue = getValue(right);
		if(getType(left) == REAL || getType(right) == REAL) {
			result = newLexReal(REAL,pow(leftValue,rightValue));
		}
		else {
			result = newLexInt(INTEGER,(int)(pow(leftValue,rightValue)));
		}
		updateEnv(cdr(left),result,object);
	}
	else {
		fprintf(stderr, "Bad assignment");
	}
	return result;
}

static LEXEME *evalModuloEquals(LEXEME *left, LEXEME *right, LEXEME *env) {
	LEXEME *result = NULL;
	if(getType(left) == ID) {
		LEXEME *leftValueLex = lookupEnv(left,env);
		double leftValue = getValue(leftValueLex);
		double rightValue = getValue(right);
		if(getType(left) == REAL || getType(right) == REAL) {
			result = newLexReal(REAL,fmod(leftValue,rightValue));
		}
		else {
			result = newLexInt(INTEGER,(int)(fmod(leftValue,rightValue)));
		}
		updateEnv(left,result,env);
	}
	else if(getType(left) == DOT) {
		LEXEME *object = eval(car(left),env);
		LEXEME *leftValueLex = lookupEnv(cdr(left),object);
		double leftValue = getValue(leftValueLex);
		double rightValue = getValue(right);
		if(getType(left) == REAL || getType(right) == REAL) {
			result = newLexReal(REAL,fmod(leftValue,rightValue));
		}
		else {
			result = newLexInt(INTEGER,(int)(fmod(leftValue,rightValue)));
		}
		updateEnv(cdr(left),result,object);
	}
	else {
		fprintf(stderr, "Bad assignment");
	}
	return result;
}