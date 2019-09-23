/*
    @author: Chas Shipman   Feb 2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pretty.h"
#include "lexeme.h"
#include "type.h"

static void  prettyProgram(LEXEME *tree);
static void  prettyVarDef(LEXEME *tree);
static void  prettyFuncDef(LEXEME *tree);
static void  prettyFuncBlock(LEXEME *tree);
static void  prettyClassDef(LEXEME *tree);
static void  prettyVarInit(LEXEME *tree);
static void  prettyStmntList(LEXEME *tree);
static void  prettyExpression(LEXEME *tree, char *operator);
static void  prettyExprComma(LEXEME *tree);
static void  prettyExprUop(LEXEME *tree);
static void  prettyParamList(LEXEME *tree);
static void  prettyIfStmnt(LEXEME *tree);
static void  prettyIfBlock(LEXEME *tree);
static void  prettyWhile(LEXEME *tree);
static void  prettyWhileBlock(LEXEME *tree);
static void  prettyFor(LEXEME *tree);
static void  prettyForExpr(LEXEME *tree);
static void  prettyForBlock(LEXEME *tree);
static void  prettyUnaryUop(LEXEME *tree);
static void  prettyPrintln(LEXEME *tree);
static void  prettyPrint(LEXEME *tree);
static void  prettyParenExpr(LEXEME *tree);
static void  prettyFuncCall(LEXEME *tree);
static void  prettyExpreList(LEXEME *tree);

static void prettyBlock(LEXEME *tree);
static void prettyOptParamList(LEXEME *tree);
static void prettyStatementSemi(LEXEME *tree);
static void prettyStatement(LEXEME *tree);
static void prettyAssign(LEXEME *tree);
static void prettyPParamList(LEXEME *tree);
static void prettyReturn(LEXEME *tree);
static void prettyUnary(LEXEME *tree);
static void prettyOP(LEXEME *tree);
static void prettyUOP(LEXEME *tree);
static void prettyOptElse(LEXEME *tree);
static void prettyOptExpr(LEXEME *tree);
static void prettyVarExpr(LEXEME *tree);
static void prettyOptExprList(LEXEME *tree);
static void prettyExprList(LEXEME *tree);
static void prettyClassCall(LEXEME *tree);
static void prettyCallClass(LEXEME *tree);



void  pp(LEXEME *tree){
        char *type = getType(tree);
        if(type == STRING){
            printf("\"");
            printf("%s", getString(tree));
            printf("\"");
        }
        else if(type == CHARACTER){
             printf("\'");
             printf("%s", getString(tree));
             printf("\'");
        }
        else if(type == ID)printf("%s", getString(tree));
        else if(type == INTEGER) printf("%d", getInt(tree));
        else if(type == REAL) printf("%f", getReal(tree));

        else if(type == EQUALS) prettyExpression(tree,"=");
        else if(type == GREATER_THAN) prettyExpression(tree,">");
        else if(type == GREATER_THAN_OR_EQUAL_TO) prettyExpression(tree,">=");
        else if(type == LESS_THAN) prettyExpression(tree,"<");
        else if(type == LESS_THAN_OR_EQUAL_TO) prettyExpression(tree,"<=");
        else if(type == EQUAL_TO) prettyExpression(tree,"==");
        else if(type == NOT_EQUAL_TO) prettyExpression(tree,"!=");
        else if(type == DOT) prettyExpression(tree,".");

        else if(type == PLUS) prettyExpression(tree,"+");
        else if(type == PLUS_EQUALS) prettyExpression(tree,"+=");
        else if(type == MINUS) prettyExpression(tree,"-");
        else if(type == MINUS_EQUALS) prettyExpression(tree,"-=");
        else if(type == TIMES) prettyExpression(tree,"*");
        else if(type == TIMES_EQUALS) prettyExpression(tree,"*=");
        else if(type == DIVISION) prettyExpression(tree,"/");
        else if(type == DIVISION_EQUALS) prettyExpression(tree,"/=");
        else if(type == EXPONENT) prettyExpression(tree,"^");
        else if(type == EXPONENT_EQUALS) prettyExpression(tree,"^=");
        else if(type == MODULO) prettyExpression(tree,"%%");
        else if(type == MODULO_EQUALS) prettyExpression(tree,"%%=");
        else if(type == PLUS_PLUS) prettyExpression(tree,"++");
        else if(type == MINUS_MINUS) prettyExpression(tree,"--");

        else if(type == AND) printf("&&");
        else if(type == NOT) printf("!");
        else if(type == OR)  printf("||");

        else if(type == PROGRAM) prettyProgram(tree);
        else if(type == VARDEF) prettyVarDef(tree);
        else if(type == FUNCDEF) prettyFuncDef(tree);
        else if(type == FUNCBLOCK) prettyFuncBlock(tree);
        else if(type == CLASSDEF) prettyClassDef(tree);
        //else if(type == ARRAYDEF) prettyArrayDef(tree);
        else if(type == VARINIT) prettyVarInit(tree);
        else if(type == STATEMENT_LIST) prettyStmntList(tree);
        else if(type == EXPR_COMMA) prettyExprComma(tree);
        else if(type == EXPR_UOP) prettyExprUop(tree);
        else if(type == PARAM_LIST) prettyParamList(tree);
        else if(type == IFSTATE) prettyIfStmnt(tree);
        else if(type == IFBLOCK) prettyIfBlock(tree);
        else if(type == WHILELOOP) prettyWhile(tree);
        else if(type == WHILEBLOCK) prettyWhileBlock(tree);
        else if(type == FORLOOP) prettyFor(tree);
        else if(type == FOREXPR) prettyForExpr(tree);
        else if(type == FORBLOCK) prettyForBlock(tree);
        else if(type == UNARY_UOP) prettyUnaryUop(tree);
        else if(type == PRINTLNSTATE) prettyPrintln(tree);
        else if(type == PRINTSTATE) prettyPrint(tree);
        else if(type == PAREN_EXPR) prettyParenExpr(tree);
        else if(type == FUNCCALL) prettyFuncCall(tree);
        else if(type == EXPR_LIST) prettyExpreList(tree);
        
        else if(type == PBLOCK) prettyBlock(tree);
        else if(type == POPTPARAMLIST) prettyOptParamList(tree);
        else if(type == STATEMENT) prettyStatementSemi(tree);
        else if(type == PSTATEMENT) prettyStatement(tree);
        else if(type == PASSIGN) prettyAssign(tree);
        else if(type == PEXPR) prettyExpression(tree,"");
        else if(type == PPARAMLIST) prettyPParamList(tree);
        else if(type == PRETURN) prettyReturn(tree);
        else if(type == PUNARY) prettyUnary(tree);
        else if(type == POPERATOR) prettyOP(tree);
        else if(type == PUOP) prettyUOP(tree);
        else if(type == POPTELSE) prettyOptElse(tree);
        else if(type == POPTEXPR) prettyOptExpr(tree);
        else if(type == PVAREXPR) prettyVarExpr(tree);
        else if(type == POPTEXPRLIST) prettyOptExprList(tree);
        else if(type == PEXPRLIST) prettyExprList(tree);
        else if(type == CLASSCALL) prettyClassCall(tree);
        else if(type == CALLCLASS) prettyCallClass(tree);
        else if(type == NULL){
            printf("RETURNING NULL IS THE ISSUE\n");
            exit(1);
        }
        else{
            printf("no pp for %s\n", getType(tree));
            exit(1);
        }
}


static void  prettyProgram(LEXEME *tree){
   // printf("IN PROGRAM\n");
   // exit(1);
    while(tree != NULL){
        pp(car(tree));//definition();
        tree = cdr(tree);// program() or NULL;
    }
}

static void  prettyVarDef(LEXEME *tree){
    printf("var "); //match(var)
    pp(car(tree)); //match(ID)
    pp(cdr(tree));//optInit
    printf(";\n");
}

static void  prettyFuncDef(LEXEME *tree){
    printf("function "); //match(func)
    pp(car(tree)); //ID
    printf("(");
    prettyFuncBlock(cdr(tree));
}

static void prettyFuncBlock(LEXEME *tree){
    pp(car(tree)); //optParam
    printf(")");
    pp(cdr(tree)); //block
    printf("\n");
}

static void  prettyClassDef(LEXEME *tree){
   // printf("IN CLASS\n");
    printf("class ");
    pp(car(tree)); //id
    pp(cdr(tree)); //block
    printf("\n");
    return;
}
//static void  prettyArrayDef(tree);
static void  prettyVarInit(LEXEME *tree) {
    if(car(tree) != NULL) {
        char *type = getType(car(tree));
        if(type == EQUALS) printf("=");
        else if(type == PLUS_EQUALS) printf("+=");
        else if(type ==  MINUS_EQUALS) printf("-=");
        else if(type ==  DIVISION_EQUALS) printf("/=");
        else if(type ==  TIMES_EQUALS) printf("*=");
        else if(type ==  EXPONENT_EQUALS) printf("^=");
        else if(type ==  MODULO_EQUALS) printf("%%=");
    }
    if(cdr(tree) != NULL)
        pp(cdr(tree)); //expression
    return;
}

static void  prettyStmntList(LEXEME *tree){
    pp(car(tree)); //statement()
    if(cdr(tree) != NULL)
        pp(cdr(tree)); //statementList() or NULL
    return;
}

static void prettyExpression(LEXEME *tree, char *operator) {
    if(car(tree) != NULL) {
        pp(car(tree));
    }
    printf("%s", operator);
    if(cdr(tree) != NULL) {
        pp(cdr(tree));
    }
    return;
}

static void  prettyExprComma(LEXEME *tree){
    pp(car(tree));//unary
    printf(", ");
    pp(cdr(tree));//expression
    return;
}

static void  prettyExprUop(LEXEME *tree){
    pp(car(tree));//unary
    pp(cdr(tree));//uop
    //printf(";\n");
    return;
}

static void  prettyParamList(LEXEME *tree){
    pp(car(tree)); // ID
    printf(", ");
    pp(cdr(tree)); // paramList() or NULL
    return;
}

static void  prettyIfStmnt(LEXEME *tree){
    printf("if");
    printf("( ");
    pp(car(tree));//expression
    printf(") ");
    prettyIfBlock(cdr(tree));
}

static void  prettyIfBlock(LEXEME *tree){
    pp(car(tree)); // block
    printf("\n");
    prettyOptElse(cdr(tree)); // optElse
    return;
}

static void  prettyWhile(LEXEME *tree){
    printf("while");
    printf("( ");
    pp(car(tree));//expression
    printf(") ");
    prettyWhileBlock(cdr(tree)); //block
    return;
}

static void prettyWhileBlock(LEXEME *tree){
    pp(car(tree));
    printf("\n");
    return;
}

static void  prettyFor(LEXEME *tree){
    printf("for");
    printf("( ");
    pp(car(tree));//expression
    printf("; ");
    prettyForExpr(cdr(tree));
}

static void  prettyForExpr(LEXEME *tree){
    pp(car(tree)); //optexpr
    printf("; "); 
    prettyForBlock(cdr(tree));
}

static void  prettyForBlock(LEXEME *tree){
    pp(car(tree)); //optexpr
    printf(") ");
    pp(cdr(tree));//block
    printf("\n");
    return;
}

static void  prettyUnaryUop(LEXEME *tree){
    pp(car(tree)); //uop
    pp(cdr(tree)); //unary
    //printf(";\n");
}

static void  prettyPrintln(LEXEME *tree){
    printf("println");
    printf(": ");
    pp(car(tree));
    //printf(";\n");
}

static void  prettyPrint(LEXEME *tree){
    printf("print");
    printf(": ");
    pp(car(tree));
   // printf(";\n");
}

static void  prettyParenExpr(LEXEME *tree){
    printf("(");
    pp(car(tree));
    printf(")");
}

static void  prettyFuncCall(LEXEME *tree){
    pp(car(tree));//ID
    printf("(");
    if(cdr(tree))
        pp(cdr(tree));//optExpr
    printf(")");
    return;
}

static void  prettyExpreList(LEXEME *tree){
    pp(car(tree)); //expression
    printf(", ");
    pp(cdr(tree)); //expressList();ß
}

static void prettyBlock(LEXEME *tree){
    printf(" {\n");
    pp(car(tree));
    printf("}");
}

static void prettyOptParamList(LEXEME *tree){
    if(car(tree) != NULL)
        pp(car(tree));
    return;
}

static void prettyStatementSemi(LEXEME *tree){
    pp(car(tree)); //exp
    printf(";\n");
}

static void prettyStatement(LEXEME *tree){
    pp(car(tree));
}

static void prettyAssign(LEXEME *tree){

    printf(" ");
    pp(car(tree));
    printf(" ");
}

static void prettyPParamList(LEXEME *tree){
    pp(car(tree));//ID;
}

static void prettyReturn(LEXEME *tree){
    printf("return");
    if(car(tree)){
        printf(" ");
        pp(car(tree));
    }
    printf(";\n");
}

static void prettyUnary(LEXEME *tree){
    pp(car(tree)); //unary
}

static void prettyOP(LEXEME *tree){
        if(getType(car(tree))== DOT){
            prettyClassCall(tree);
            return;
        }
        else{
            printf(" ");
            pp(car(tree));//op
            printf(" ");
        }
}

static void prettyUOP(LEXEME *tree){
    pp(car(tree));//uop
}

static void prettyOptElse(LEXEME *tree){
    if(tree != NULL){
        printf("else");
        if(getType(car(tree)) == PBLOCK){
            pp(car(tree));
            printf("\n");
        }
        else car(tree);
        return;
    }
}

static void prettyOptExpr(LEXEME *tree){
    pp(car(tree));//expression()
}

static void prettyVarExpr(LEXEME *tree){
    pp(car(tree));//ID
}

static void prettyOptExprList(LEXEME *tree){
    pp(car(tree));//expressionList()
}

static void prettyExprList(LEXEME *tree){
    pp(car(tree)); // express();
}

static void prettyClassCall(LEXEME *tree){
    pp(car(tree));
    printf(".");
    pp(cdr(tree));
}
static void prettyCallClass(LEXEME *tree){
    pp(car(tree)); //varexpr
    pp(cdr(tree)); //optinit
}
