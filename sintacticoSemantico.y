/*Inclusión de librerias necesarias para el análisis*/
%{
#include <math.h>
#include <stdio.h>
#include "./headerFiles/TablaSimbolos.h"
#include "./headerFiles/Errores.h"   /*centralización de la gestión de errores*/
#include "./lex.yy.c"  /*demanda de componentes lexicos*/

/*Declaramos las funciones para evitar los warning por implicit_declaration*/
//para imprimir la salida tipificada
void nuevaLinea();  
//para imprimir la información de ayuda general.
void ayudaGeneral();
//funcion de error
void yyerror(char *s);

int codigoError=-1;   /*para hacerselo visible a la función de gestión de errores*/
%}

%union {
    double _double;
    int _int;
    tipoelem *elementoTS;   /*Puntero a un elemento de la tabla de símbolos (constante, variable, funcion)*/
}

/*TERMINALES*/
%token <_double>    _NUM
%token <elementoTS> _VAR _FUNCION _CONST   
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
        | exp '\n' {printf("\x1b[32mOut[%d]: %.10g\n\x1b[0m", yylineno-1,$1);nuevaLinea(); /*printf("\n");imprimirArbol();*/}
        | error '\n' {yyerrok;}
;

exp:    
        _NUM                {$$=$1;}
        |_VAR               {
                                /*Una expresión NUNCA podrá derivar en una variable si esta
                                no está inicializada. Error*/
                                if($1->inicializada == 0){
                                    eliminar(*$1);
                                    codigoError = 11;
                                    yyerror($1->lexema);
                                    YYERROR;
                                }
                                $$ = $1->value.var;}
        |_CONST             {$$ = $1->value.var;}
        | _VAR '=' exp      {
                                /*Notar que al lado izquierdo de la expresión SOLO puede aparecer
                                una variable. NUNCA una constante o un número*/
                                $$ = $3;
                                modificar($1, $3);
                            }
        | exp '+' exp       {$$ = $1 + $3;}
        | exp '-' exp       {$$ = $1 - $3;}
        | exp '*' exp       {$$ = $1 * $3;}
        | exp '/' exp       {$$ = $1 / $3;}
        | '-' exp %prec NEG {$$=-$2;}
        | exp '^' exp       {$$ = pow($1,$3);}
        | '(' exp ')'       {$$=$2;}
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

void yyerror(char *s){
    printf("\x1b[31mOut[%d]: ", yylineno-1);
    if(codigoError==-1){
        printf("%s\n",s);
    }
    else{
        imprimeError(codigoError,yylineno, s, 0);
        codigoError=-1;
    }
    printf("\x1b[0m");
    nuevaLinea();
}
