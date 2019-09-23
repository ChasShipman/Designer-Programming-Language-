/*
    @author: Chas Shipman   Feb 2019
*/

#ifndef __EVALUATOR_INCLUDED__
#define __EVALUATOR_INCLUDED__

#include <stdio.h>
#include "lexeme.h"

LEXEME *eval(LEXEME *tree, LEXEME *env);


#endif