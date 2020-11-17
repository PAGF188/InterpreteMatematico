/*Inclusión de librerias necesarias para el análisis*/
%{
#include <math.h>
#include <stdio.h>
#include "./headerFiles/Errores.h"   /*centralización de la gestión de errores*/
#include "./lex.yy.c"  /*demanda de componentes lexicos*/
#include "./headerFiles/Definiciones.h"

/*Declaramos las funciones para evitar los warning por implicit_declaration*/
//para imprimir la salida tipificada
void nuevaLinea();  
//para imprimir la información de ayuda general.
void ayudaGeneral();
%}

%union {
    double _double;
    int _int;
    /*tipoelem *elementoTS;   Puntero a un elemento de la tabla de símbolos*/
}

/*TERMINALES*/
%token <_double>    _NUM
%right '='
%left '-' '+'
%left '*' '/'
%left NEG
%right '^'

/*NO TERMINALES*/
%type <_double> exp

%%

input:  /*vacio*/
        | input linea
;

linea: 
        '\n'
        | '?' '\n'  {ayudaGeneral(); nuevaLinea();}
        | exp '\n' {printf("Out[%d]:  %.10g\n", yylineno-1,$1); nuevaLinea();}
        | error '\n' {yyerrok;}
;

exp:    
        
        _NUM {$$=$1;}
        | exp '+' exp {$$ = $1 + $3;}
        | exp '-' exp {$$ = $1 - $3;}
        | exp '*' exp {$$ = $1 * $3;}
        | exp '/' exp {$$ = $1 / $3;}
        | '-' exp %prec NEG {$$=-$2;}
        | exp '^' exp   {$$ = pow($1,$3);}
        | '(' exp ')'   {$$=$2;}
;

%%

//DEFINICIONES AL INICIO DEL ARCHIVO

void nuevaLinea(){
    printf("____________________________________________________________\n\n");
    printf("In [%d]:  ", yylineno);
}

void ayudaGeneral(){
    printf("En el futuro aqui ira la ayuda general\n");
}

yyerror(s) 
    char*s;
{
    printf("%s\n", s);
}
