/*
    @author: Chas Shipman   Feb 2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include "lexer.h"
#include "lexeme.h"
#include "parser.h"
#include "type.h"

LEXEME *current;
FILE *fp;

LEXEME *parser(char *filename);

static LEXEME *defenition();
static LEXEME *varDef();
static LEXEME *funcDef();
static LEXEME *lambdaDef();
static LEXEME *classDef();
static LEXEME *optInit();
static LEXEME *optParameterList();
static LEXEME *block();
static LEXEME *statementList();
static LEXEME *statement();
static LEXEME *assign();
static LEXEME *expression();
static LEXEME *parameterList();
static LEXEME *ifStatement();
static LEXEME *whileLoop();
static LEXEME *unary();
static LEXEME *operator();
static LEXEME *uoperator();
static LEXEME *optElse();
static LEXEME *variableExpr();
static LEXEME *optExpressionList();
static LEXEME *expressionList();

static bool programPending();
static bool varDefPending();
static bool funcDefPending();
static bool classDefPending();
static bool optInitPending();
static bool optParameterListPending();
static bool statementListPending();
static bool expressionPending();
static bool ifStatementPending();
static bool whileLoopPending();
static bool defenitionPending();
static bool operatorPending();
static bool uoperatorPending();
static bool variableExprPending();
static bool elsePending();
static bool expressionListPending();
static bool parameterListPending();
static bool statementPending();
static bool unaryPending();

/*
int main(int argc, char **argv){
    assert(argc = 2);
	parser(argv[1]);
	return 0;
}*/

LEXEME *parser(char *filename) {
	fp = lexOpen(filename);
	advance();
	LEXEME *tree = program();
	match(END_OF_INPUT);
	
	return tree;
}

LEXEME *advance() {
	LEXEME *previous = current;
	current = lex(fp);
	return previous;
}

bool check(char *type) {
	return getType(current) == type;
}

LEXEME *match(char *type) {
	if(check(type)) {
		return advance();
	}
	printf("SYNTAX ERROR Line %d: Recieved %s\n", getLineNum(), getType(current));
	printf("EXPECTED: %s\n",type);
    printf("illegal\n");
    exit(1);
}

LEXEME *program() {
	LEXEME *a, *b = NULL;
	if(defenitionPending()) {
		a = defenition();
	}
	else {
		a = statement();
	}
	if(programPending()) b = program();
	return cons(PROGRAM,a,b);
}

LEXEME *defenition() {
	if(varDefPending()) return varDef();
	else if(funcDefPending()) return funcDef();
	else if(classDefPending()) return classDef();
	return NULL;
}

LEXEME *varDef() {
	match(VAR);
	LEXEME *a = match(ID);
	LEXEME *b = optInit();
	match(SEMICOLON);
	return cons(VARDEF, a, b);
}

LEXEME *funcDef() {
	LEXEME *tree, *l, *r = NULL;
	match(FUNCTION);
	tree = match(ID);
	match(OPEN_PAREN);
	l = optParameterList();
	match(CLOSE_PAREN);
	r = block();
	return cons(FUNCDEF, tree, cons(GLUE, l,r));
}

LEXEME *lambdaDef() {
	match(LAMBDA);
	match(OPEN_PAREN);
	LEXEME *a = optParameterList();
	match(CLOSE_PAREN);
	LEXEME *b = block();
	return cons(LAMBDADEF, NULL, cons(GLUE,a,b));
}

LEXEME *classDef() {
	LEXEME *a, *b = NULL;
	match(CLASS);
	a = match(ID);
	b = block();
	match(SEMICOLON);
	return cons(CLASSDEF,a,b);
}

LEXEME *optInit() {
	LEXEME *a = NULL, *b = NULL;
	if(optInitPending()) {
		a = assign();
		b = expression();
	}
	return cons(VARINIT,a,b);
}

LEXEME *optParameterList() {
	if(optParameterListPending()) {
		return parameterList();
	}
	return NULL;
}

LEXEME *block() {
	LEXEME *a = NULL;
	match(OPEN_BRACE);
	a = statementList();
	match(CLOSE_BRACE);
	return cons(PBLOCK, a, NULL);
}

LEXEME *statementList() {
	LEXEME *a, *b = NULL;
	a = statement();
	if(statementListPending()) b = statementList();
	return cons(STATEMENT_LIST,a,b);
}

LEXEME *statement() {
	LEXEME *a = NULL;
	if(expressionPending()) {
		a = expression();
		match(SEMICOLON);
	}
	else if(ifStatementPending())	a = ifStatement();
	else if(whileLoopPending())		a = whileLoop();
	else if(defenitionPending())	a = defenition();
	else {
		fprintf(stderr,"Type: %s\n", getType(current));
		fprintf(stderr, "FATAL ERROR BAD STATEMENT");
		exit(1);
	}
	return a;
}

LEXEME *assign() {
	LEXEME *a = NULL;
	if(check(EQUALS)) a = match(EQUALS);
	else if(check(PLUS_EQUALS))  a = match(PLUS_EQUALS);
    else if(check(MINUS_EQUALS))  a = match(MINUS_EQUALS);
    else if(check(TIMES_EQUALS)) a = match(TIMES_EQUALS);
    else if(check(DIVISION_EQUALS)) a = match(DIVISION_EQUALS);
    else if(check(EXPONENT_EQUALS))  a = match(EXPONENT_EQUALS);
    else if(check(MODULO_EQUALS)) a = match(MODULO_EQUALS);
    return a;
}

LEXEME *expression() {
	LEXEME *u, *a, *b = NULL;
	u = unary();
	if(operatorPending()) {
		a = operator();
		b = expression();
		return cons(getType(a), u, b);
	}
	else if(uoperatorPending()) {
		a = uoperator();
		return cons(UNARY_UOP, a, u);
	}
	return u;
}

LEXEME *parameterList() {
	LEXEME *a, *b = NULL;
	a = match(ID);
	if(check(COMMA)) {
		match(COMMA);
		b = parameterList();
		return cons(PARAM_LIST,a,b);
	}
	return cons(PARAM_LIST,a,NULL);
}

LEXEME *ifStatement() {
	LEXEME *a, *b, *c = NULL;
	match(IF);
	match(COLON);
	a = expression();
	//match(CLOSE_PAREN);
	b = block();
	c = optElse();
	return cons(IFSTATE, a, cons(IFBLOCK, b, c));
}

LEXEME *whileLoop() {
	LEXEME *a, *b = NULL;
	match(WHILE);
	match(COLON);
	a = expression();
	//match(CLOSE_PAREN);
	b = block();
	return cons(WHILELOOP, a, cons(WHILEBLOCK, b, NULL));
}

LEXEME *unary() {
	LEXEME *a, *b = NULL;
	if(variableExprPending()) a = variableExpr();
	else if(check(INTEGER)) a = match(INTEGER);
	else if(check(REAL)) a = match(REAL);
	else if(check(CHARACTER)) a = match(CHARACTER);
	else if(check(STRING)) a = match(STRING);
	else if(check(LAMBDA)) a = lambdaDef();
	else if(uoperatorPending()) {
		a = uoperator();
		b = unary();
		return cons(UNARY_UOP, a, b);
	}
	else {
		match(OPEN_PAREN);
		a = expression();
		match(CLOSE_PAREN);
	}
	return a;
}

LEXEME *operator() {
	LEXEME *a = NULL;
	if(check(PLUS)) a = match(PLUS);
    else if(check(MINUS)) a = match(MINUS);
    else if(check(TIMES)) a = match(TIMES);
    else if(check(EXPONENT)) a = match(EXPONENT);
    else if(check(DIVISION)) a = match(DIVISION);
    else if(check(MODULO)) a = match(MODULO);
    else if(check(EQUAL_TO))  a = match(EQUAL_TO);
    else if(check(NOT_EQUAL_TO)) a = match(NOT_EQUAL_TO);
    else if(check(GREATER_THAN)) a = match(GREATER_THAN);
    else if(check(GREATER_THAN_OR_EQUAL_TO)) a = match(GREATER_THAN_OR_EQUAL_TO);
    else if(check(LESS_THAN)) a = match(LESS_THAN);
    else if(check(LESS_THAN_OR_EQUAL_TO)) a = match(LESS_THAN_OR_EQUAL_TO);
    else if(check(AND)) a = match(AND);
    else if(check(OR))  a = match(OR);
    else if(check(DOT)) a = match(DOT);
    else if(check(EQUALS)) a = match(EQUALS);
    else if(check(PLUS_EQUALS))  a = match(PLUS_EQUALS);
    else if(check(MINUS_EQUALS))  a = match(MINUS_EQUALS);
    else if(check(TIMES_EQUALS)) a = match(TIMES_EQUALS);
    else if(check(DIVISION_EQUALS)) a = match(DIVISION_EQUALS);
    else if(check(EXPONENT_EQUALS))  a = match(EXPONENT_EQUALS);
    else if(check(MODULO_EQUALS)) a = match(MODULO_EQUALS);
    return a;
}

LEXEME *uoperator() {
	LEXEME *a = NULL;
	if(check(PLUS_PLUS)) a = match(PLUS_PLUS);
	else if(check(MINUS_MINUS)) a = match(MINUS_MINUS);
	else if(check(NOT)) a = match(NOT);
    else if(check(MINUS)) a = match(MINUS);
    return a;
}

LEXEME *optElse() {
	LEXEME *a = NULL;
	if(elsePending()) {
		match(ELSE);
		match(COLON);
		if(ifStatementPending()) a = ifStatement();
		else a = block();
		return cons(POPTELSE, a, NULL);
	}
	return NULL;
}

LEXEME *variableExpr() {
	LEXEME *a, *b = NULL;
	a = match(ID);
	if(check(OPEN_PAREN)) {
		match(OPEN_PAREN);
		b = optExpressionList();
		match(CLOSE_PAREN);
		return cons(FUNCCALL,a,b);
	}
	return a;
}

LEXEME *optExpressionList() {
	if(expressionListPending()) {
		return expressionList();
	}
	return NULL;
}

LEXEME *expressionList() {
	LEXEME *a, *b = NULL;
	a = expression();
	if(check(COMMA)) {
		match(COMMA);
		b = expressionList();
		return cons(EXPR_LIST,a,b);
	}
	return cons(EXPR_LIST, a, NULL);
}

bool programPending() {
	return defenitionPending() || statementPending();
}

bool varDefPending() {
	return check(VAR);
}

bool funcDefPending() {
	return check(FUNCTION);
}

bool classDefPending() {
	return check(CLASS);
}

bool optInitPending() {
    return check(EQUALS) || check(PLUS_EQUALS) || check(MINUS_EQUALS)
    	|| check(TIMES_EQUALS)	|| check(DIVISION_EQUALS)
    	|| check(EXPONENT_EQUALS) || check(MODULO_EQUALS);
}

bool optParameterListPending() {
	return parameterListPending();
}

bool statementListPending() {
	return statementPending();
}

bool expressionPending() {
	return unaryPending();
}

bool ifStatementPending() {
	return check(IF);
}

bool whileLoopPending() {
	return check(WHILE);
}


bool defenitionPending() {
	return varDefPending() || funcDefPending() || classDefPending();
}

bool operatorPending() {
	return check(PLUS) || check(MINUS)
		|| check(TIMES) || check(EXPONENT)
		|| check(DIVISION) || check(MODULO) || check(EQUAL_TO)
		|| check(GREATER_THAN) || check(GREATER_THAN_OR_EQUAL_TO)
		|| check(LESS_THAN) || check(LESS_THAN_OR_EQUAL_TO) || check(NOT_EQUAL_TO)
		|| check(AND) || check(OR) || check(DOT) || check(EQUALS)
		|| check(PLUS_EQUALS) || check(MINUS_EQUALS)
		|| check(TIMES_EQUALS) || check(DIVISION_EQUALS)
		|| check(EXPONENT_EQUALS) || check(EXPONENT_EQUALS);
}

bool uoperatorPending() {
	return check(PLUS_PLUS) || check(MINUS_MINUS) || check(MINUS) || check(NOT);
}

bool variableExprPending() {
	return check(ID);
}

bool elsePending() {
	return check(ELSE);
}

bool expressionListPending() {
	return expressionPending();
}

bool parameterListPending() {
	return check(ID);
}

bool statementPending() {
	return expressionPending() || ifStatementPending()
		|| whileLoopPending() || defenitionPending();
}

bool unaryPending() {
	return variableExprPending() || check(INTEGER) || check(REAL)
		|| check(CHARACTER) || check(STRING) || uoperatorPending()
		|| check(OPEN_PAREN) || check(LAMBDA);
}




