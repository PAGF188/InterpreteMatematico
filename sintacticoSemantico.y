/*Inclusión de librerias necesarias para el análisis*/
%{
#include <math.h>
#include <stdio.h>
#include "./headerFiles/Errores.h"   /*centralización de la gestión de errores*/
/*#include "./lex.yy.c"   para demandar componentes léxicos*/
%}

%union {
    double _double;
    int _int;
    char * _string;
    /*tipoelem *elementoTS;   Puntero a un elemento de la tabla de símbolos*/
}

%token <_int>       _INTEGER
%token <_double>    _FLOAT
%token <_string>    _STRING
%token <_string>    _IDENTIFICADOR

%right '='
%left '-' '+'
%left '*' '/'
%left NEG
%right '^'

%type <_double> exp


%%

input:  /*vacio*/
        | input linea
;

linea: 
        '\n'
        | exp '\n' {printf("\t%.10g\n", $1);}
        | error '\n' {yyerrok;}
;

exp:    _INTEGER {$$=$1;}
        | exp '+' exp {$$ = $1 + $3;}
        | exp '-' exp {$$ = $1 - $3;}
        | exp '*' exp {$$ = $1 * $3;}
        | exp '/' exp {$$ = $1 / $3;}
        | '-' exp %prec NEG {$$=-$2;}
        | exp '^' exp   {$$ = pow($1,$3);}
        | '(' exp ')'   {$$=$2;}

%%

yyerror(s) 
    char*s;
{
    printf("%s\n", s);
}
