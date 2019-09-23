/*
    @author: Chas Shipman   Jan 2019
*/

#ifndef __LEXEME_INCLUDED__
#define __LEXEME_INCLUDED__

#include <stdio.h>
#include <stdbool.h>

typedef struct lexeme LEXEME;
typedef LEXEME *(*fval)(LEXEME *lex);

extern LEXEME *newLEXEME(char *passedType);
extern LEXEME *newLexInt(char *passedType, int value);
extern LEXEME *newLexReal(char *passedType, double value);
extern LEXEME *newLexString(char *passedType, char *value);
extern LEXEME *newLexChar(char *passedType, char value);
extern LEXEME *newLexFunction(char *passedType, LEXEME *(*fval)(LEXEME *));
extern LEXEME *newLexArray(int size);
extern LEXEME *newLexFile(char *file);
extern void setArrayVal(LEXEME *array, LEXEME *index, LEXEME *value);
extern LEXEME *getArrayVal(LEXEME *array, LEXEME *index);
extern int getArraySize(LEXEME *lex);
extern char *getType(LEXEME *lex);
extern int getInt(LEXEME *lex);
extern double getReal(LEXEME *lex);
extern char *getString(LEXEME *lex);
extern char getChar(LEXEME *lex);
extern fval getFunction(LEXEME *lex);
extern FILE *getFptr(LEXEME *lex);
extern LEXEME *cons(char *type, LEXEME *left, LEXEME *right);
extern LEXEME *car(LEXEME *lex);
extern LEXEME *cdr(LEXEME *lex);
extern void setCar(LEXEME *lex, LEXEME *left);
extern void setCdr(LEXEME *lex, LEXEME *right);
extern bool compareLex(LEXEME *lex1, LEXEME *lex2);
extern void displayLex(LEXEME *lex);
#endif