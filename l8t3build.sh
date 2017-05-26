#!/bin/sh
bison -y -d ciLisp.y
flex ciLisp.l
clang -std=c99 -D_GNU_SOURCE=1 -D_POSIX_SOURCE=1 ciLisp.c lex.yy.c y.tab.c -o ciLisp -g -lm

