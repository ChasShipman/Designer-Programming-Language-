/*
    @author: Chas Shipman   Feb 2019
*/

#ifndef __PARSER_INCLUDED__
#define __PARSER_INCLUDED__

#include <stdio.h>
#include "lexeme.h"

extern LEXEME *parser(char *filename);
extern LEXEME *advance();
extern bool check(char *type);
extern LEXEME *match(char *type);
extern LEXEME *program();

#endif
