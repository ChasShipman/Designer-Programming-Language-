/*
	CREATED BY: Robert J. Mallow 	ON: 1/26/19
	Project Scyther
*/

#include <stdio.h>
#include <assert.h>
#include "lexer.h"
#include "lexeme.h"
#include "type.h"

FILE *fp;

void scanner(char *filename);

int main(int argc, char **argv){
    assert(argc = 2);
	scanner(argv[1]);
	return 0;
}

void scanner(char *filename) {
	fp = lexOpen(filename);
	LEXEME * token = lex(fp);

	while(getType(token) != END_OF_INPUT) {
		displayLex(token);
		token = lex(fp);
		if(getType(token) == END_OF_INPUT) {
			printf("%s\n", END_OF_INPUT);
		}
	}
}