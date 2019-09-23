/*
    @author: Chas Shipman   Jan 2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include "lexer.h"
#include "lexeme.h"
#include "type.h"

//helper functions for tidier code
//static void displayLexer(LEXEME *lex);
//static LEXEME *equalsHelper(FILE *fp);
static LEXEME *plusHelper(FILE *fp);
static LEXEME *minusHelper(FILE *fp);
static LEXEME *timesHelper(FILE *fp);
static LEXEME *divisionHelper(FILE *fp);
static LEXEME *exponentHelper(FILE *fp);
static LEXEME *moduloHelper(FILE *fp);
static LEXEME *notHelper(FILE *fp);
static LEXEME *greaterHelper(FILE *fp);
static LEXEME *lessHelper(FILE *fp);
static LEXEME *andHelper(FILE *fp);
static LEXEME *orHelper(FILE *fp);

static char escChar(FILE *fp, char ch); //finds escape char in string

static int lineNum = 1; //lineNum
static int escapeFlag = 0;

FILE  *lexOpen(char *filename){
    lineNum = 1;
    FILE *fp = fopen(filename, "r");
    return fp;
}

int getLineNum() {
    return lineNum;
}

/*
static void displayLexer(LEXEME *lex){
   
    if(getType(lex) == INTEGER){
        printf("%s %d\n", getType(lex), getInt(lex));
    }
    else if(getType(lex) == REAL)
        printf("%s %f\n", getType(lex), getReal(lex));
    else if(getType(lex) == STRING)
        printf("%s \"%s\"\n", getType(lex), getString(lex)); //<----Remove quotes from this line to get rid of quotes in display
    else if(getType(lex) == CHARACTER)
    	printf("%s \'%c\'\n", getType(lex), getChar(lex));
    else if(getType(lex) == VAR)
        if(getString(lex) == 0)
            printf("%s\n", getType(lex));
        else
            printf("%s %s\n", getType(lex), getString(lex));
    else if(getType(lex) == BAD_REAL){
        fprintf(stderr, "FATAL ERROR line %d: BAD_REAL\n",lineNum);// fatal error in case of bad double or float ex: 3..4 is bad
        exit(1);
    }
    else if(getType(lex) == BAD_VAR) {
        fprintf(stderr, "FATAL ERROR line %d: BAD_VAR\n",lineNum);// fatal error in case of bad var ex: var longer than 63 chars
        exit(1);
    }
    else if(getType(lex) == BAD_CHAR) {
        fprintf(stderr, "FATAL ERROR line %d: BAD_CHAR\n",lineNum);// fatal error in case of bad char
        exit(1);
    }
    else
        printf("%s\n", getType(lex));
}
*/
char skipWhiteSpace(FILE *fp){
    char ch = fgetc(fp); //grab char
    while(ch != EOF){ //read file
	    if(isspace(ch)) {
	        if(ch == '\n')	lineNum++; //incr line num
	       	ch = fgetc(fp);
	    }
    	else if(ch == '#') 	ch = comment(fp);
	    else break;
    }
    return ch;
}

char comment(FILE *fp){
    char ch = fgetc(fp); // read comment char
    if(ch != ':'){
        while(ch != '\n'){
            ch = fgetc(fp);
        }
        lineNum++; //newline break out so incr
    }
    else {//if ch = ':' we have a comment block;
        ch = fgetc(fp); //read next char
        while(ch != EOF){
            if(ch == '\n'){
                lineNum++;
            }
            if(ch == ':') {
            	ch = fgetc(fp);
            	if(ch == '#') break;
            	ungetc(ch,fp);
            }
            ch = fgetc(fp); //read next char
        }
    }
    return ch;
}

LEXEME *lex(FILE *fp) {
	char ch = '\0';
	ch = skipWhiteSpace(fp);

	if(ch == EOF) return newLEXEME(END_OF_INPUT);

	switch(ch) {
		case '(' : return newLEXEME(OPEN_PAREN);
		case ')' : return newLEXEME(CLOSE_PAREN);
		case '{' : return newLEXEME(OPEN_BRACE);
		case '}' : return newLEXEME(CLOSE_BRACE);
		case ';' : return newLEXEME(SEMICOLON);
		case ',' : return newLEXEME(COMMA);
		case ':' : return newLEXEME(COLON);
		case '.' : return newLEXEME(DOT);
        case '=' : return newLEXEME(EQUAL_TO);
		case '+' : return plusHelper(fp);
		case '-' : return minusHelper(fp);
		case '/' : return divisionHelper(fp);
		case '*' : return timesHelper(fp);
		case '%' : return moduloHelper(fp);
		case '^' : return exponentHelper(fp);
		case '!' : return notHelper(fp);
		case '>' : return greaterHelper(fp);
        case '<' : return lessHelper(fp);
        case '&' : return andHelper(fp);
        case '|' : return orHelper(fp);
		default:
			if(isdigit(ch)) {
				ungetc(ch,fp);
				return lexNumber(fp);
			}
			else if(isalpha(ch)) {
				ungetc(ch,fp);
				return lexVariableOrKeyword(fp);
			}
			else if(ch == '\"') {
				return lexString(fp);
			}
			else if(ch == '\'') {
				return lexChar(fp);
			}
			fprintf(stderr, "line %d: UNKNOW_CHAR \'%c\'. \n",lineNum, ch); //fatal error in case of bad char.
            exit(1);
	}
}


LEXEME *lexNumber(FILE *fp) {
	int realCheck = 0;
    int len = 0;
    int cap = 8;
    char *numBuff = malloc(sizeof(char) * cap);
    char ch = fgetc(fp); //read Char
    while(isdigit(ch) || ch == '.'){
        numBuff[len++] = ch; //add char to buffer
        if(ch == '.' && realCheck) return newLexString(BAD_REAL, numBuff); //check for bad real
        if(ch == '.') realCheck = 1; //have a real so set real flag
       
        if(len == cap){ 
            cap *= 2;
            numBuff = realloc(numBuff, cap);
        }
       
        ch = fgetc(fp); //read next char after possible resize
    }
    ungetc(ch,fp); //return char that broke loop
   
    if(len == cap){//resize just in case to add '\0'
        cap += 1;
        numBuff = realloc(numBuff, cap);
    }
    
    numBuff[len] = '\0'; //append buffer with null char
   // printf("%s\n", numBuff); exit(1);
    
	if(realCheck)
		return newLexReal(REAL, strtod(numBuff, NULL));
	else
		return newLexInt(INTEGER, strtol(numBuff, NULL, 10));
}

LEXEME *lexVariableOrKeyword(FILE *fp){ //general same flow as lexNum
    int len = 0;
    int cap = 64;
    char *buffer = malloc(sizeof(char)*cap);
    
    char ch = fgetc(fp);
    while(isalpha(ch) || isdigit(ch) || ch == '_'){
        buffer[len++] = ch;
        if(len + 1 == cap){
            buffer[len] = '\0';
            return newLexString(BAD_VAR, buffer);
        }
        ch = fgetc(fp);
    }
    ungetc(ch,fp);
    buffer[len] = '\0';
    //checks for key words
    if(strcmp(buffer, "define") == 0)      return newLEXEME(VAR);
    if(strcmp(buffer, "DEFINE") == 0) return newLEXEME(FUNCTION);
    if(strcmp(buffer,"if") == 0)        return newLEXEME(IF);
    if(strcmp(buffer,"else") == 0)       return newLEXEME(ELSE);
    if(strcmp(buffer,"for") == 0)       return newLEXEME(FOR);
    if(strcmp(buffer,"while") == 0)     return newLEXEME(WHILE);
    if(strcmp(buffer,"class") == 0)     return newLEXEME(CLASS);
    if(strcmp(buffer, "CLASS") == 0)    return newLEXEME(CLASS);
    if(strcmp(buffer,"grape") == 0)    return newLEXEME(LAMBDA);
   // variable
    return newLexString(ID, buffer);
}

LEXEME *lexString(FILE *fp) {
	int len = 0;
    int cap = 64;
    char *buffer = malloc(sizeof(char)*cap);
    char ch = fgetc(fp);//past quote
 
    while(ch != '\"') {
    	if(len + 1 >= cap) {
    		cap *= 2;
            buffer = realloc(buffer,sizeof(char) * cap);
    	}
        if(ch == '\\') ch = escChar(fp,ch);//check for escape char in string
        buffer[len++] = ch; //add to buffer   
        if(ch == '\n') lineNum++;
        ch = fgetc(fp);
        escapeFlag = 0;
    }
    buffer[len] = '\0'; //append null char
    return newLexString(STRING, buffer);
}

LEXEME *lexChar(FILE *fp) {
	char ch = fgetc(fp);
	if(fgetc(fp) != '\'') {
		return newLexChar(BAD_CHAR, ch);
	}
    return newLexChar(CHARACTER, ch);
}

static LEXEME *plusHelper(FILE *fp){
    char ch= fgetc(fp);
    if(ch == '=') return newLEXEME(PLUS_EQUALS);
    if(ch == '+') return newLEXEME(PLUS_PLUS);
    ungetc(ch,fp);
    return newLEXEME(PLUS);
}

static LEXEME *minusHelper(FILE *fp){
    char ch= fgetc(fp);
    if(ch == '=') return newLEXEME(MINUS_EQUALS);
    if(ch == '-') return newLEXEME(MINUS_MINUS);
    ungetc(ch,fp);
    return newLEXEME(MINUS);
}

static LEXEME *timesHelper(FILE *fp){
    char ch= fgetc(fp);
    if(ch == '=') return newLEXEME(TIMES_EQUALS);
    ungetc(ch,fp);
    return newLEXEME(TIMES);
}

static LEXEME *divisionHelper(FILE *fp){
    char ch= fgetc(fp);
    if(ch == '=') return newLEXEME(DIVISION_EQUALS);
    ungetc(ch,fp);
    return newLEXEME(DIVISION);
}

static LEXEME *exponentHelper(FILE *fp){
    char ch= fgetc(fp);
    if(ch == '=') return newLEXEME(EXPONENT_EQUALS);
    ungetc(ch,fp);
    return newLEXEME(EXPONENT);
}

static LEXEME *moduloHelper(FILE *fp){
    char ch= fgetc(fp);
    if(ch == '=') return newLEXEME(MODULO_EQUALS);
    ungetc(ch,fp);
    return newLEXEME(MODULO);
}

static LEXEME *notHelper(FILE *fp){
    char ch= fgetc(fp);
    if(ch == '=') return newLEXEME(NOT_EQUAL_TO);
    ungetc(ch,fp);
    return newLEXEME(NOT);
}

static LEXEME *greaterHelper(FILE *fp){
    char ch= fgetc(fp);
    if(ch == '>') return newLEXEME(EQUALS);
    if(ch == '=') return newLEXEME(GREATER_THAN_OR_EQUAL_TO);
    ungetc(ch,fp);
    return newLEXEME(GREATER_THAN);
}

static LEXEME *lessHelper(FILE *fp){
    char ch= fgetc(fp);
    if(ch == '=') return newLEXEME(LESS_THAN_OR_EQUAL_TO);
    ungetc(ch,fp);
    return newLEXEME(LESS_THAN);
}

static LEXEME *andHelper(FILE *fp){
    char ch= fgetc(fp);
    if(ch == '&') return newLEXEME(AND);
    ungetc(ch,fp);
    return newLEXEME(AMPERSAND);
}

static LEXEME *orHelper(FILE *fp){
    char ch= fgetc(fp);
    if(ch == '|') return newLEXEME(OR);
    ungetc(ch,fp);
    return newLEXEME(NON_DEFINED_SINGLE_VERTICAL_BAR);
}

static char escChar(FILE *fp,char ch){ //return escape char inside string
    ch = fgetc(fp);
    escapeFlag = 1;
    switch(ch){
        case 'n' : return '\n';
        case 't' : return '\t';
        case 'b' : return '\b';
        case 'v' : return '\v';
        case 'f' : return '\f';
        case 'r' : return '\r';
        case 'a' : return '\a';
      //  case '"' : return '\"';
        //case '\\': return '\\';
        default : return ch;
    }
}
