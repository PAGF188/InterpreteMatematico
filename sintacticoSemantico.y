/*Inclusión de librerias necesarias para el análisis*/
%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/*para hacerselo visible a la función de gestión de errores*/
int codigoError=-1;   

/*Si echo=0, no mostrar dialogo de resultado.
  Si echo=1, si mostrarlo.
*/
int echo = 1;
%}

%union {
    char * _string;
    double _double;
    int _int;
    tipoelem *elementoTS;   /*Puntero a un elemento de la tabla de símbolos (constante, variable, funcion)*/
}

/*TERMINALES*/
%token <_string>    _MODULO
%token <_string>    _ARCHIVO    //path
%token <_string>    _STRING
%token <_double>    _NUM
%token <elementoTS> _VAR _FUNCION _CONST _COMANDO 
%token <_int>       _EOF 
/*el comando delete se trata por separado para poder diferenciar entre print a, en donde imprimimos
el contenido de la variable a. Y delete a, en donde la eliminamos */
%token <elementoTS> _DELETE _INCLUDE
%right '='
%left '-' '+'
%left '*' '/'
%left NEG
%right '^'

/*NO TERMINALES*/
%type <_double> exp
%type <_string> argumento

%%

input:  /*vacio*/
        | input linea
;

linea: 
        '\n'
        | exp '\n'              {   /*mdoe=0, estamos leyendo datos de teclado. Mostrar interfaz gráfica*/
                                    if(mode==0){
                                        if(echo)
                                            printf("\x1b[32mOut[%d]: %.10g\n\x1b[0m", yylineno-1,$1);
                                        nuevaLinea(); 
                                    }
                                    /*printf("\n");imprimirArbol();*/
                                }
        | error '\n'            {yyerrok;}
        | _COMANDO '\n'         { 
                                    /*si no son print load o include*/
                                    if(strcmp("print", $1->lexema)!=0 && strcmp("load", $1->lexema)!=0){
                                        if(strcmp("?", $1->lexema)==0){
                                            $1->value.funcion_ptr("vacio"); 
                                        }
                                        else{
                                            $1->value.funcion_ptr(); 
                                        }
                                    }
                                    if(mode==0)    
                                        nuevaLinea();
                                }
        | _COMANDO argumento '\n' {
                                    /*esta derivación solo es valida con los comandos: print, load */
                                    if(strcmp("print", $1->lexema)==0 || strcmp("load", $1->lexema)==0){
                                        $1->value.funcion_ptr($2);
                                        free($2);
                                    }
                                    else{
                                        codigoError = 13;
                                        yyerror($1->lexema);
                                        YYERROR;
                                    }
                                }
        | _DELETE _VAR '\n'     {$1->value.funcion_ptr($2);}
        | _INCLUDE _ARCHIVO _VAR     {$1->value.funcion_ptr($2, $3->lexema);}
        | _COMANDO _COMANDO {  /*derivacion para ayuda especifica*/
                                if(strcmp("?", $2->lexema)==0)
                                    $2->value.funcion_ptr($1->lexema);
                                if(mode==0)    
                                        nuevaLinea();
                            }
;

argumento:      
        exp                     {   
                                    /*cast double to char* */
                                    char* _s = (char *)malloc(sizeof(char)*1000); /*definimos un tamaño maximo*/
	                                sprintf(_s,"%.10g",$1);
                                    $$ = _s;
                                }
        | _STRING               {$$=$1;}
        | _ARCHIVO              {$$=$1;}
        | argumento _STRING {$$=strcat($1,$2); free($2);}    
        | argumento exp         {
                                    /*cast double to char* y concatenación con textoImprimir*/
                                    char* _s = (char *)malloc(sizeof(char)*1000); /*definimos un tamaño maximo*/
	                                sprintf(_s,"%.10g",$2);
                                    $$=strcat($1,_s);
                                    free(_s);
                                }     
;

exp:    
        _NUM                    {$$=$1;}
        | _VAR '=' exp          {
                                    /*Notar que al lado izquierdo de la expresión SOLO puede aparecer
                                    una variable. NUNCA una constante o un número*/
                                    modificar($1, $3);
                                    $$=$3;
                                }
        |_VAR                   {
                                    /*Una expresión NUNCA podrá derivar en una variable si esta
                                    no está inicializada. Error*/
                                    if($1->inicializada == 0){
                                        eliminar(*$1);
                                        codigoError = 11;
                                        yyerror($1->lexema);
                                        free($1);
                                        YYERROR;
                                    }
                                    $$ = $1->value.var;
                                }
        | _FUNCION '(' exp ')'  { $$ = $1->value.funcion_ptr($3);}
        |_CONST                 {$$ = $1->value.var;}
        | exp '+' exp           {$$ = $1 + $3;}
        | exp '-' exp           {$$ = $1 - $3;}
        | exp '*' exp           {$$ = $1 * $3;}
        | exp '/' exp           {
                                    /*Error al intentar dividir entre 0*/
                                    if($3==0){
                                        codigoError = 12;
                                        yyerror("");
                                        YYERROR;
                                    }
                                    $$ = $1 / $3;
                                }
        | '-' exp %prec NEG     {$$=-$2;}
        | exp '^' exp           {$$ = pow($1,$3);}
        | '(' exp ')'           {$$=$2;}
;

%%

//DEFINICIONES AL INICIO DEL ARCHIVO

void nuevaLinea(){
    printf("─────────────────────────────────────────────────────\n\n");
    printf("In [%d]:  ", yylineno);
}

void yyerror(char *s){
    printf("\x1b[31m");

    //formatear la salida para interacción por teclado/archivo
    if(mode==0){
        printf("Out[%d]: ", yylineno-1);
    }
    //Si el error es sintáctico/semántico
    if(codigoError==-1){
        if(mode==1)
            printf("\t(linea %d) ", yylineno);
        printf("%s\n",s);
    }
    else{
        imprimeError(codigoError,yylineno, s, mode);
        codigoError=-1;
    }
    printf("\x1b[0m");
    if(mode==0)
        nuevaLinea();
}
