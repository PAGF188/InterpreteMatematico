/*Inclusión de librerias necesarias para el análisis*/
%{
#include <math.h>
#include <stdio.h>
#include "./headerFiles/Errores.h"   /*centralización de la gestión de errores*/
#include "./lex.yy.c"  /*para demandar componentes lexicos*/
%}

%union {
    double _double;
    /*tipoelem *elementoTS;   Puntero a un elemento de la tabla de símbolos*/
}

%token <_double>    _NUM

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

exp:    
        
        _NUM {$$=$1;}
        | exp '+' exp {$$ = $1 + $3; printf("%f, %f",$1,$3);}
        | exp '-' exp {$$ = $1 - $3;}
        | exp '*' exp {$$ = $1 * $3;}
        | exp '/' exp {$$ = $1 / $3;}
        | '-' exp %prec NEG {$$=-$2;}
        | exp '^' exp   {$$ = pow($1,$3);}
        | '(' exp ')'   {$$=$2;}
;

%%

yyerror(s) 
    char*s;
{
    printf("%s\n", s);
}
