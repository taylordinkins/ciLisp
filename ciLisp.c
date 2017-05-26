/*  Taylor Dinkins
    Comp 232 
    Task 08
    03/13/2014
*/

#include "ciLisp.h"

int integerFlag;

int main(void)
{
    yyparse();
    return 0;
}

void yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
    error();
}

int resolveFunc(char *func)
{
   char *funcs[] = {"neg", "abs", "exp", "sqrt", "add", "sub", "mult", "div", "remainder", "log", "pow", "max", "min", "exp2", "cbrt", "hypot", "print", "rand", "read", "" };
   
   int i = 0;
   while (funcs[i][0] !='\0')
   {
      if (!strcmp(funcs[i], func))
         return i;
         
      i++;
   }
   yyerror("invalid function"); // paranoic -- should never happen
   return -1;
}

double calc(char *func, double op1, double op2)
{
  double retVal = 0;
  int funcType = resolveFunc(func);
  switch(funcType)
  {
    case 0: retVal = -op1; break;
    case 1: retVal = abs(op1); break;
    case 2: retVal = exp(op1); break;
    case 3: retVal = sqrt(op1); break;
    case 4: retVal = op1 + op2; break;
    case 5: retVal = op1 - op2; break;
    case 6: retVal = op1 * op2; break;
    case 7: retVal = op1 / op2; break;
    case 8: retVal = remainder(op1, op2); break;
    case 9: 
      if(op1 == 10)
      {  
        retVal = log10(op2);
      }
      else if(op1 == 2)
      {
        retVal = log2(op2);
      }
      else
      {
        retVal = log(op1);
      }
      break;
    case 10: retVal = pow(op1, op2); break;
    case 11: retVal = fmax(op1, op2); break;
    case 12: retVal = fmin(op1, op2); break;
    case 13: retVal = exp2(op1); break;
    case 14: retVal = cbrt(op1); break;
    case 15: retVal = hypot ( op1, op2 ); break;
    default: break;
  }


  return retVal;

}  

AST_NODE *number(double value)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE) + sizeof(NUMBER_AST_NODE) + sizeof(FUNCTION_AST_NODE) + sizeof(char*) + sizeof(AST_NODE*) + sizeof(AST_NODE*) + sizeof(AST_NODE*) + sizeof(ST_NODE*) + sizeof(AST_NODE_TYPE);
    if ((p = (AST_NODE*) malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = NUM_TYPE;
    p->data.number.value = value;

    return p;
}

AST_NODE *symbol(char *symbolName)
{
  AST_NODE *p;
  size_t nodeSize;

  nodeSize = sizeof(AST_NODE) + sizeof(SYMBOL_AST_NODE) + sizeof(FUNCTION_AST_NODE) + sizeof(char*) + sizeof(AST_NODE*) + sizeof(AST_NODE*) + sizeof(AST_NODE*) + sizeof(ST_NODE*) + sizeof(AST_NODE_TYPE);
  if ((p = (AST_NODE*) malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  p->type = SYMBOL_TYPE;
  p->data.symbol.name = symbolName;

  return p;
}

ST_NODE *letList(char *type, char *symbolName, AST_NODE *op1)
{
  ST_NODE *p;
  size_t nodeSize;

  nodeSize = sizeof(AST_NODE) + sizeof(FUNCTION_AST_NODE) + sizeof(char*) + sizeof(AST_NODE*) + sizeof(AST_NODE*) + sizeof(AST_NODE*) + sizeof(ST_NODE*) + sizeof(AST_NODE_TYPE) + sizeof(ST_NODE);
  if ((p = (ST_NODE *) malloc(nodeSize)) == NULL)
    yyerror("out of memory");
  if(op1 != NULL)
    p->definition = op1;
  p->symbol = symbolName;
  p->type = type;


  return p;
}

// create a node for a function
AST_NODE *function(char *funcName, AST_NODE *op1, AST_NODE *op2)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE) + sizeof(FUNCTION_AST_NODE) + sizeof(char*) + sizeof(AST_NODE*) + sizeof(AST_NODE*) + sizeof(AST_NODE*) + sizeof(ST_NODE*) + sizeof(AST_NODE_TYPE);
    if ((p = (AST_NODE*)malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = FUNC_TYPE;
    p->data.function.name = funcName;    
    if(op1 != NULL)
      op1->parent = p;
      p->data.function.op1 = op1; //Check for NULL
    if(op2 != NULL)
      op2->parent = p;
      p->data.function.op2 = op2;

    return p;
}



// free a node

void createNext(ST_NODE *current, ST_NODE *next)
{
  if (current != NULL)
  {
    if(current->next != NULL)
    {
      while(current->next != NULL)
      {
        current = current->next;
      }
      current->next = next;
    }
    else
    {
      current->next = next;
    }
  }  
  else
    error();
}



double evaluate(AST_NODE *p)
{
  double retVal = 0;
  if(!p)
    error();
  if(p->type == FUNC_TYPE)
  {
    int funcType = resolveFunc(p->data.function.name);
    switch(funcType)
    {
      case 0: retVal = -evaluate(p->data.function.op1); break;
      case 1: retVal = abs(evaluate(p->data.function.op1)); break;
      case 2: retVal = exp(evaluate(p->data.function.op1)); break;
      case 3: retVal = sqrt(evaluate(p->data.function.op1)); break;
      case 4: retVal = evaluate(p->data.function.op1) + evaluate(p->data.function.op2); break;
      case 5: retVal = evaluate(p->data.function.op1) - evaluate(p->data.function.op2); break;
      case 6: retVal = evaluate(p->data.function.op1) * evaluate(p->data.function.op2); break;
      case 7: retVal = evaluate(p->data.function.op1) / evaluate(p->data.function.op2); break;
      case 8: retVal = remainder(evaluate(p->data.function.op1), evaluate(p->data.function.op2)); break;
      case 9: retVal = log(evaluate(p->data.function.op1)); break;
      case 10: retVal = pow(evaluate(p->data.function.op1), evaluate(p->data.function.op2)); break;
      case 11: retVal = fmax(evaluate(p->data.function.op1), evaluate(p->data.function.op2)); break;
      case 12: retVal = fmin(evaluate(p->data.function.op1), evaluate(p->data.function.op2)); break;
      case 13: retVal = exp2(evaluate(p->data.function.op1)); break;
      case 14: retVal = cbrt(evaluate(p->data.function.op1)); break;
      case 15: retVal = hypot(evaluate(p->data.function.op1), evaluate(p->data.function.op2)); break;
      case 16: retVal = evaluate(p->data.function.op1); int toPrint;
              if(integerFlag == 1)
              {
                toPrint = (int)retVal;
                printf("Printing: %d", toPrint); 
              }
              else
              {
                printf("Printing: %.02f", retVal);
              }
              break;
      case 17: retVal = rand(); break;
      case 18: retVal = scanInput(); break;
      default: break;
    }
  }
  else if(p->type == NUM_TYPE)
  {
    retVal = p->data.number.value;    
  }
  else if(p->type == SYMBOL_TYPE)
  {
    retVal = symbolLookup(p->parent, p->parent->symbolTable, p->data.symbol.name);
  }
  else
  {
    error();
  }
  return retVal;
}

double scanInput()
{
  double retVal;
  scanf("%g", retVal);

  return retVal;
}

double symbolLookup(AST_NODE *current, ST_NODE *p, char *toFind)
{
  double retVal = 0;
  if(p == NULL)
  {
    if(current->parent != NULL)
    {
      retVal = symbolLookup(current->parent, current->parent->symbolTable, toFind);
    }
    else
    {
      error();
    }
  }
  else if(strcmp(p->symbol, toFind) == 0)
  {
    if(strcmp(p->type, "integer") == 0 || strcmp(p->type,"int") == 0)
    {
      integerFlag = 1;
      lround(retVal = evaluate(p->definition));
    }
    else if(strcmp(p->type, "real") == 0)
    {
      retVal = evaluate(p->definition);
    }
  }
  else
  {
    if(p->next != NULL)
    {
      retVal = symbolLookup(current, p->next, toFind);
    }
    else if(current->parent != NULL)
    {
      retVal = symbolLookup(current->parent, current->parent->symbolTable, toFind);
    }
    else
    {
      syntaxError();
    } 
  }
  return retVal;
}

void freeNode(AST_NODE *p)
{
    if (!p)
       return;
       
    if (p->type == FUNC_TYPE)
    {
        free(p->data.function.name);
        if(p->data.function.op1 != NULL)
          freeNode(p->data.function.op1);
        if(p->data.function.op2 != NULL)
          freeNode(p->data.function.op2);
        if(p->symbolTable != NULL)
          freeStNode(p->symbolTable);
    }
    if (p->type == SYMBOL_TYPE)
    {
      free(p->data.symbol.name);
    }
        
    free (p);
    p = NULL;
}

void freeStNode(ST_NODE *p)
{
  if(!p)
    return;
  free(p->type);
  free(p->symbol);
  if(p->definition != NULL)
    freeNode(p->definition);
  if(p->next != NULL)
    freeStNode(p->next);

  free(p);
  p = NULL;
}

/*void translate(AST_NODE *p)
{
    int needComma = 0;
    if (!p)
      return;
    if(p->type == FUNC_TYPE)
    {
      int funcType = resolveFunc(p->data.function.name);
      switch(funcType)
      {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7: needComma = 0; break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12: needComma = 1; break;
        case 13: 
        case 14: needComma = 0; break;
        case 15: needComma = 1; break;
      }
      if (funcType == 4 || funcType == 5 || funcType == 6 || funcType == 7)
      {
        if(p->data.function.op1 != NULL)
        {
          translate(p->data.function.op1);
        }
        switch(funcType)
        {
          case 4: printf(" + "); break;
          case 5: printf(" - "); break;
          case 6: printf(" * "); break;
          case 7: printf(" / "); break;
        }
        if(p->data.function.op2 != NULL)
        {
          translate(p->data.function.op2);
        }        
      }
      else
      {
        printf("%s (", p->data.function.name);
        if(p->data.function.op1 != NULL)
        {
          translate(p->data.function.op1);
        }
        if (needComma == 1)
        {
          printf(", ");
        }
        if(p->data.function.op2 != NULL)
        {
          translate(p->data.function.op2);
        }
        printf(") ");
      }     
    }

    else if(p->type == NUM_TYPE)
    {
      printf(" %g", p->data.number.value);
    }
    else
    {
      error();     
    } 
}*/

/*void printTree(AST_NODE *p)
{
  AST_NODE *current = p;
  size_t nodeSize;

  nodeSize = sizeof(AST_NODE) + sizeof(FUNCTION_AST_NODE);
  if ((malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  if(current->type == FUNC_TYPE)
  {
    printf("Name: %s\n", current->data.function.name);
    if(current->parent != NULL)
    {
      printf("Parent Name: %s\n", current->parent->data.function.name);
    }
    if(current->symbolTable != NULL)
    {
      printST(current->symbolTable);
    }
    if(current->data.function.op1 != NULL)
    {
      printf("OP1: ");
      printTree(current->data.function.op1);
    }
    if(current->data.function.op2 != NULL)
    {
      printf("OP2: ");
      printTree(current->data.function.op2);
    }

  }

  else if(current->type == NUM_TYPE)
  {
    printf("Number Value: %g\n", current->data.number.value);
  }

  else if(current->type == SYMBOL_TYPE)
  {
    printf("Symbol Name: %s\n", current->data.symbol.name);
  }

  else
  {
    printf("YOU HAVE A BAD AST NODE");
    exit(0);
  }


}

void printST(ST_NODE *p)
{
  ST_NODE *current = p;
  size_t nodeSize;

  nodeSize = sizeof(ST_NODE) + sizeof(FUNCTION_AST_NODE);
  if ((malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  if(current != NULL)
  {
    printf("Symbol: %s\n", current->symbol);
  }

  if(current->definition != NULL)
  {
    printf("Definition: ");
    printTree(current->definition);
  }

  if(current->next != NULL)
  {
    printf("Next: ");
    printST(current->next);
  }

  if(current == NULL)
  {
    printf("BAD ST NODE");
    exit(0);
  }
}*/

void syntaxError()
{
  printf("Syntax Error: Symbol Lookup Failed\n");
  exit(0);
}
void error()
{
  printf("\nError: NULL NODE ACCESS\n");
  exit(0);
}