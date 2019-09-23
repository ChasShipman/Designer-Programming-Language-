/*
    @author: Chas Shipman   Jan 2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "lexeme.h"
#include "type.h"
#include "environment.h"

static bool sameVariable(LEXEME *lex1, LEXEME *lex2);
static void displayLexEnv(LEXEME *lex);

LEXEME *createEnv() {
	return cons(ENV, NULL, cons(VALUES,NULL,NULL));
}

LEXEME *lookupEnv(LEXEME *variable, LEXEME *env) {
	while(env != NULL) {
		LEXEME *vars = car(env);
		LEXEME *vals = car(cdr(env));
		while(vars != NULL) {
			if(sameVariable(variable, car(vars))) {
				return car(vals);
			}

			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(cdr(env));
	}
	fprintf(stderr, "FATAL ERROR: VARIABLE %s IS UNDEFINED\n",getString(variable));
    exit(1);

    return NULL;
}


LEXEME *updateEnv(LEXEME *variable, LEXEME *value, LEXEME *env) {
	while(env != NULL) {
		LEXEME *vars = car(env);
		LEXEME *vals = car(cdr(env));
		while(vars != NULL) {
			if(sameVariable(variable, car(vars))) {
				setCar(vals, value);
				return value;
			}
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(cdr(env));
	}
	fprintf(stderr, "FATAL ERROR: VARIABLE %s IS UNDEFINED \n",getString(variable));
    exit(1);

    return NULL;
}


LEXEME *insertEnv(LEXEME *variable, LEXEME *value, LEXEME *env) {
	setCar(env,cons(JOIN, variable, car(env)));
	setCar(cdr(env),cons(JOIN,value,car(cdr(env))));
	return value;
}


LEXEME *extendEnv(LEXEME *variable, LEXEME *value, LEXEME *env) {
	return cons(ENV, variable, cons(VALUES, value, env));
}

static bool sameVariable(LEXEME *variable1, LEXEME *variable2) {
	//return compareLex(variable1, variable2);
    if(variable1 == NULL || variable2 == NULL) return 0;
    if(strcmp(getString(variable1), getString(variable2)) == 0){
        return 1;
    }
    else return 0;
}

void displayEnv(LEXEME *env){
    if(getType(env) != ENV){
        printf("THIS IS BAD\n");
        exit(1);
    }
    int i = 0;
    while(env != NULL){
        printf("ENVIRONMENT %d is:     \n", i++);
        LEXEME *varList = car(env);
        LEXEME *valList = car(cdr(env));
        while(varList != NULL){
            printf("\t");
            LEXEME *var = car(varList);
            LEXEME *val = car(valList);
           // print("%s = %d\n", )
            displayLexEnv(var);
            displayLexEnv(val);
            varList = cdr(varList);
            valList = cdr(valList);
        }
        printf("\n");
        env = cdr(cdr(env));
    } 
    printf("\n");
}


static void displayLexEnv(LEXEME *lex){
    if(getType(lex) == INTEGER)
        printf("%d\n", getInt(lex));
    else if(getType(lex) == REAL)
        printf("%.2f\n", getReal(lex));
    else if(getType(lex) == STRING)
        printf("\"%s\"\n", getString(lex));
    else if(getType(lex) == ID)
        printf("%s = ", getString(lex));
    else if(getType(lex) == CHARACTER)
        printf("\'%s\'\n", getString(lex));
   // else printf("%s ", getString(lex));
    return;
}
