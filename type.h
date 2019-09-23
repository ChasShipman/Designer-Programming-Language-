/*
    @author: Chas Shipman   Jan 2019
*/

#ifndef __TYPE_INCLUDED__
#define __TYPE_INCLUDED__

#include <stdio.h>

//keywords
char *VAR;	char *FUNCTION;	char *CLASS; 
char *ID;

char *IF;	char *ELSE;	char *WHILE;	char *FOR;

char *INTEGER;	char *REAL;
char *CHARACTER;	char *STRING;

char *PRINT; char *PRINTLN;
char *END_OF_INPUT; char *RETURN;

//nonKeyWords
char *OPEN_PAREN;	char *CLOSE_PAREN;
char *OPEN_BRACE;	char *CLOSE_BRACE;
char *OPEN_BRACKET;	char *CLOSE_BRACKET;
char *COLON;	char *SEMICOLON;	char *COMMA;
char *PLUS;	char *MINUS;	char *DIVISION;
char *TIMES;	char *MODULO;	char *EXPONENT;
char *DOT;	char *AMPERSAND;
char *EQUAL_TO;	char *NOT_EQUAL_TO;
char *GREATER_THAN;	char *GREATER_THAN_OR_EQUAL_TO;
char *LESS_THAN;	char *LESS_THAN_OR_EQUAL_TO;
char *AND;	char *OR;	char *NOT;	
char *EQUALS;	char *PLUS_EQUALS;	char *MINUS_EQUALS;
char *TIMES_EQUALS;	char *DIVISION_EQUALS;
char *MODULO_EQUALS;	char *EXPONENT_EQUALS;
char *PLUS_PLUS;	char *MINUS_MINUS;

char *BAD_CHAR; 	char *BAD_REAL;	char *BAD_VAR;
char *NON_DEFINED_SINGLE_VERTICAL_BAR;

char *PROGRAM; char *VARDEF; char *FUNCDEF; char *CLASSDEF; //char *ARRAYDEF;
char *VARINIT; char *STATEMENT_LIST; char *EXPR_OP; char *EXPR_COMMA; 
char *EXPR_UOP; char *PARAM_LIST; char *IFSTATE; char *WHILELOOP;
char *FORLOOP; char *UNARY_UOP; char *PRINTLNSTATE; 
char *PRINTSTATE; char *PAREN_EXPR; char *FUNCCALL; 
char *EXPR_LIST; char *IFBLOCK; char *FOREXPR; char *FORBLOCK; 
char *FUNCBLOCK; char *OP_EXPR; char *PBLOCK; char *OPTPARAMLIST; char *PSTATEMENT;
char *PASSIGN; char *PEXPR; char *PPARAMLIST; char *PRETURN;
char *UNARY; char *POPERATOR; char *PUOP; char *POPTELSE; 
char *POPTEXPR; char *PVAREXPR;
char *OPTEXPRLIST; char *PEXPRLIST;
char *STATEMENT; char *CLASSCALL; char *CALLCLASS;
char *WHILEBLOCK;

char *ENV;	char *UNDEFINED_VAR;
char *VALUES;	char *JOIN;

char *CLOSURE; char *OCLOSURE;
char *BUILTIN; char *GLUE;
char *RETURNED; char *ARRAY;
char *FILEPTR; char *LAMBDA;
char *LAMBDADEF;
#endif