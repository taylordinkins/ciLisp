/*  Taylor Dinkins
    Comp 232 
    Task 08
    03/13/2014
*/

%option noyywrap

%{
    #include "ciLisp.h"
%}

digit [0-9]
number [+|-]?{digit}+(\.{digit}+)?
letter [a-zA-Z]
symbol {letter}+
type ""|"int"|"real"|"integer"
func "neg"|"abs"|"exp"|"sqrt"|"add"|"sub"|"mult"|"div"|"remainder"|"log"|"pow"|"max"|"min"|"exp2"|"cbrt"|"hypot"|"print"|"rand"|"read"

%%

{number} {
                yylval.dval = strtod(yytext, NULL);
                //printf("lex: NUMBER dval = %lf\n", yylval.dval);
                return NUMBER;
            }

"let"       { return LET; }

"quit"      { return QUIT; }

{type}      {   yylval.sval = (char *) malloc(strlen(yytext)*sizeof(char));
                strcpy(yylval.sval, yytext);

                return TYPE;
            }

{func}      {
                yylval.sval = (char *) malloc(strlen(yytext)*sizeof(char));
                strcpy(yylval.sval, yytext);
                //printf("lex: FUNC sval = %s\n", yylval.sval);
                return FUNC;
            }
{symbol}    {
                yylval.sval = (char *) malloc(strlen(yytext)*sizeof(char));
                strcpy(yylval.sval, yytext);
                return SYMBOL;                
            }

"("         { 
                //printf("lex: LPAREN\n"); 
                return LPAREN;
            }
")"         { 
                //printf("lex: RPAREN\n"); 
                return RPAREN;
            }

"\n"        {
                //printf("lex: RPAREN\n"); 
                return EOL;
            }

[ \t]       ; /* skip whitespace */

.           // yyerror("lex: invalid character");

%%
