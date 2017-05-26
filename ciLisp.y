/*  Taylor Dinkins
    Comp 232 
    Task 08
    03/13/2014
*/

/*

s-expressions calculator

program ::= program s-expr

s-expr ::= number | ( func s-expr ) | ( func s-expr s-expr )

number ::= [ + | - ] digit+ [ . digit+ ] 

digit ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

*/

%{
#include "ciLisp.h"
double retVal = 0;
%}

%union
{
   double dval;
   char *sval;
   struct ast_node *astNode;
   struct st_node *stNode;
};

%token <sval> FUNC
%token <sval> SYMBOL
%token <sval> TYPE
%token <dval> NUMBER
%token LPAREN RPAREN LET EOL QUIT

%type <astNode> s_expr
%type <stNode> let_elem
%type <stNode> let_list

%%

program:/* empty */ { 
                       printf("> ");
                    }
        | program s_expr EOL {
                              // printf("yacc: program expr\n"); 
                              printf("( ");
                              integerFlag = 0;
                              retVal = evaluate($2);
                              printf(" )");
                              freeNode($2);
                              printf("\n> ");
                           }
        ;

s_expr:
        NUMBER { 
                  //printf("yacc: NUMBER%lf", $1); 
                  $$ = number($1); 
               }
        | SYMBOL {
                  $$ = symbol($1);
                }
        | LPAREN FUNC RPAREN {
                              $$ = function($2, 0, 0);
                            }   
        | LPAREN FUNC s_expr RPAREN { 
                                     // printf("yacc: LPAREN FUNC expr RPAREN\n"); 
                                     $$ = function($2, $3, 0); 
                                     //printf("%s(%lf)", $2, $3);
                                  }
        | LPAREN FUNC s_expr s_expr RPAREN {
                                          // printf("LPAREN FUNC expr expr RPAREN\n"); 
                                          // $$ = calc($2, $3, $4); 
                                          $$ = function($2, $3, $4);
                                       }
        | LPAREN LPAREN LET let_list RPAREN s_expr RPAREN {                                                            
                                                            if($4 != NULL)
                                                              $6->symbolTable = $4;
                                                            $$ = $6;
                                                            // let (symbol s_expr)                                                           
                                                          }

        | QUIT { 
                  //printf("QUIT\n"); 
                  exit(0);
               }
        
        | error { 
                        printf("error\n");
                        //printf("> ");
                    }

        ;

let_list:
        let_elem
        | let_elem let_list {
                            createNext($1, $2);                        
                           }
        ;

let_elem:
        LPAREN TYPE SYMBOL s_expr RPAREN {
                                      if($2 == NULL)
                                      {
                                        printf("ERROR: Undeclared variable %s used", $3);
                                        exit(0);
                                      }
                                      $$ = letList($2, $3, $4);                                                                        
                                    }
        | LPAREN SYMBOL s_expr RPAREN {
                                      $$ = letList(0, $2, $3);
                                      }
        ;


%%

int integerFlag;



