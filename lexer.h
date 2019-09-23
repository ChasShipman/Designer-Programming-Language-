/*
    @author: Chas Shipman   Jan 2019
*/

#ifndef __LEXER_INCLUDED__
#define __LEXER_INCLUDED__

#include <stdio.h>
#include "lexeme.h"

extern int getLineNum();
extern char skipWhiteSpace(FILE *fp); //skips white space + comments
extern char comment(FILE *fp); //skips comments

extern FILE *lexOpen(char *filename);
extern LEXEME *lex(FILE *fp);//reads file one char at a time
extern LEXEME *lexString(FILE *fp); //reads strings
extern LEXEME *lexChar(FILE *fp); //reads chars
extern LEXEME *lexNumber(FILE *fp); //reads number
extern LEXEME *lexVariableOrKeyword(FILE *fp); //reads ID or KEY 

#endif