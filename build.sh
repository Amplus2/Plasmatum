#!/bin/sh

cd src
flex lexer.l
bison -y parser.y 
bison -y -d parser.y

gcc -lfl functions.c lex.yy.c y.tab.c -lm -o ../plsm
