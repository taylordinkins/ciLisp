/*  Taylor Dinkins
    Comp 232 
    Task 08
    03/13/2014
*/

#ifndef __ciLisp_h_
#define __ciLisp_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "y.tab.h"

int yyparse(void);
int yylex(void);
void yyerror(char *);
extern int integerFlag;

typedef enum { NUM_TYPE, FUNC_TYPE, SYMBOL_TYPE } AST_NODE_TYPE;

typedef struct
{
  double value;
} NUMBER_AST_NODE;

typedef struct 
{
  char *name; 
} SYMBOL_AST_NODE;

typedef struct
{
   char *name;
   struct ast_node *op1;
   struct ast_node *op2;
} FUNCTION_AST_NODE;

typedef struct ast_node
{
  AST_NODE_TYPE type;
  struct ast_node *parent;
  union
  {
     NUMBER_AST_NODE number;
     FUNCTION_AST_NODE function;
     SYMBOL_AST_NODE symbol;
  } data;
  struct st_node *symbolTable;
} AST_NODE;

typedef struct st_node
{
  char *type;
  char *symbol;
  struct st_node *next;
  struct ast_node *definition;
} ST_NODE;

int resolveFunc(char *func);

AST_NODE *number(double value);

AST_NODE *symbol(char *symbol);

AST_NODE *function(char *funcName, AST_NODE *op1, AST_NODE *op2);

ST_NODE *letList(char *type, char *symbolName, AST_NODE *op1);

double symbolLookup(AST_NODE *current, ST_NODE *p, char *toFind);

void createNext(ST_NODE *first, ST_NODE *second);

void printTree(AST_NODE *p);

void printST(ST_NODE *p);

void freeNode(AST_NODE *p);

void freeStNode(ST_NODE *p);

double evaluate(AST_NODE *ast);

double scanInput();

double calc(char *func, double op1, double op2);

void syntaxError();

void error();

#endif
