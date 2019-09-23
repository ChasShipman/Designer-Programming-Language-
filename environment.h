/*
    @author: Chas Shipman   Jan 2019
*/

#ifndef __ENVIRONMENT_INCLUDED__
#define __ENVIRONMENT_INCLUDED__

#include <stdio.h>
#include "lexeme.h"

extern LEXEME *createEnv();
extern LEXEME *lookupEnv(LEXEME *variable, LEXEME *env);
extern LEXEME *updateEnv(LEXEME *variable, LEXEME *value, LEXEME *env);
extern LEXEME *insertEnv(LEXEME *variable, LEXEME *value, LEXEME *env);
extern LEXEME *extendEnv(LEXEME *variable, LEXEME *value, LEXEME *env);
extern void displayEnv(LEXEME *env);
#endif