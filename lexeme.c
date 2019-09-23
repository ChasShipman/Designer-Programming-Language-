/*
    @author: Chas Shipman   Jan 2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include "lexeme.h"
#include "type.h"

struct lexeme {
	char *type;
	int ival;
	char *sval;
	double rval;
	char cval;
	LEXEME **aval;
	LEXEME *left;
	LEXEME *right;
	LEXEME *(*fval)(LEXEME *);
	FILE *fptr;
};

//basic type LEXEME with no values
LEXEME *newLEXEME(char *passedType) {
	LEXEME *item = malloc(sizeof(LEXEME));
	assert(item != 0);
	item->type = passedType;
	return item;
}

//LEXEME with int value
LEXEME *newLexInt(char *passedType, int value) {
	LEXEME *item = newLEXEME(passedType);
	item->ival = value;
	return item;
}

//LEXEME with real value
LEXEME *newLexReal(char *passedType, double value) {
	LEXEME *item = newLEXEME(passedType);
	item->rval = value;
	return item;
}

//LEXEME with string value 	OR	ERROR LEXEME with string value
LEXEME *newLexString(char *passedType, char *value) {
	LEXEME *item = newLEXEME(passedType);
	item->sval = value;
	return item;
}

//LEXEME with char value 	OR	ERROR LEXEME with char value
LEXEME *newLexChar(char *passedType, char value) {
	LEXEME *item = newLEXEME(passedType);
	item->cval = value;
	return item;
}

LEXEME *newLexFunction(char *passedType, LEXEME *(*fval)(LEXEME *)) {
	LEXEME *item = newLEXEME(passedType);
	item->fval = fval;
	return item;
}

LEXEME *newLexArray(int size) {
	LEXEME *item = newLEXEME(ARRAY);
	assert(item != 0);
	item->ival = size;
	item->aval = malloc(sizeof(LEXEME *) * item->ival);
	return item;
}

LEXEME *newLexFile(char *file) {
	LEXEME *item = newLEXEME(FILEPTR);
	item->fptr = fopen(file,"r");
	return item;
}

void setArrayVal(LEXEME *array, LEXEME *index, LEXEME *val) {
	array->aval[index->ival] = val;
}

LEXEME *getArrayVal(LEXEME *array, LEXEME *index){
    return array->aval[index->ival];
}

int getArraySize(LEXEME *array){
    return array->ival;
}

char *getType(LEXEME *lex) {
	return lex->type;
}

int getInt(LEXEME *lex) {
	return lex->ival;
}

double getReal(LEXEME *lex) {
	return lex->rval;
}

char *getString(LEXEME *lex) {
	return lex->sval;
}

char getChar(LEXEME *lex) {
	return lex->cval;
}

fval getFunction(LEXEME *lex) {
	return lex->fval;
}

FILE *getFptr(LEXEME *lex) {
	return lex->fptr;
}

LEXEME *cons(char *type, LEXEME *left, LEXEME *right) {
	LEXEME *lex = newLEXEME(type);
	lex->left = left;
	lex->right = right;
	return lex;
}

LEXEME *car(LEXEME *lex) {
	return lex->left;
}

LEXEME *cdr(LEXEME *lex) {
	return lex->right;
}

void setCar(LEXEME *lex, LEXEME *left) {
	lex->left = left;
}

void setCdr(LEXEME *lex, LEXEME *right) {
	lex->right = right;
}

//true if same
bool compareLex(LEXEME *lex1, LEXEME *lex2) {
	if(lex1->type != lex2->type) return false;
	if(lex1->ival != lex2->ival) return false;
	if(lex1->rval != lex2->rval) return false;
	if(lex1->sval != lex2->sval) return false;
	if(lex1->cval != lex2->cval) return false;
	return true;
}

void displayLex(LEXEME *lex) {
	if(lex->type == INTEGER) {
		printf("%d",lex->ival);
	}
	else if(lex->type == STRING) {
		printf("%s",lex->sval);
	}
	else if(lex->type == REAL) {
		printf("%f",lex->rval);
	}
	else if(lex->type == CHARACTER) {
		printf("%c", lex->cval);
	}
}