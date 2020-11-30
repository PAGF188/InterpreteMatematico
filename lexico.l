%{
#include <stdio.h>
#include <ctype.h>
#include "./headerFiles/Definiciones.h"
#include "./headerFiles/Errores.h"

/*numero maximo de archivos a almacenar en la pila de archivos "por cargar"*/
#define MAX_INCLUDE_DEPTH 10 
/*pila de archivos a cargar*/
YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
/*puntero al elemento de la pila en el que estamos*/
int include_stack_ptr = 0;

//0-> estamos leyendo datos de teclado
//1-> estamos leyendo datos de archivos.
int mode=0; 

%}

%option noyywrap
%option yylineno


/*PATRONES PARA ENTEROS. Se aceptan sufijos.*/
INTEGER_LITERAL     {INTEGER}|({INTEGER}{INTEGER_SUFIX})
INTEGER_SUFIX       ([LuU]|(Lu)|(LU)|(uL)|(UL))
INTEGER             [0-9]+

/*PATRONES PARA FLOTANTES. Se aceptan sufijos. Se acepta notación científica.*/
FLOAT_LITERAL       {FLOAT}|({FLOAT}{SUFFIX})|({INTEGER}{FLOAT_SUFFIX})|({INTEGER}{IMAGINARY_SUFFIX})|({INTEGER}{FLOAT_SUFFIX}{IMAGINARY_SUFFIX})|({INTEGER}{REAL_SUFFIX}{IMAGINARY_SUFFIX})      
SUFFIX              {FLOAT_SUFFIX}|{REAL_SUFFIX}|{IMAGINARY_SUFFIX}|({FLOAT_SUFFIX}{IMAGINARY_SUFFIX})|({REAL_SUFFIX}{IMAGINARY_SUFFIX})
FLOAT_SUFFIX        [fF]
REAL_SUFFIX         L
IMAGINARY_SUFFIX    i
DECIMAL_INTEGER     [0-9][0-9_]*
FLOAT               ({INTEGER}".")|({INTEGER}"."{INTEGER})|({INTEGER}"."{INTEGER}{DECIMAL_EXPONENT})|("."{INTEGER})|("."{INTEGER}{DECIMAL_EXPONENT})|({INTEGER}{DECIMAL_EXPONENT})
DECIMAL_EXPONENT    ((e\+)|(E\+)|(e-)|(E-)|(e)|(E))(_?[0-9][0-9_]*)

/*IDENTIFICADORES (variables, constantes, comandos o funciones)*/
IDENTIFICADOR       [_A-Za-z][A-Za-z0-9_]*|\?

/*SOLO SE CONTEMPLA: DoubleQuotedString:*/
STRING              \"([^"\\]|\\.)*\"

/*archivo: PATH*/
ARCHIVO             [/._A-Za-z0-9]+\.(txt|so|mat)(,{IDENTIFICADOR})?

/*MODULO*/
MODULO              [_A-Za-z][A-Za-z0-9_]*\(\)

/*COMENTARIOS*/
COMENTARIO_LINEA    \/\/.*
/*Referencia: https://blog.ostermiller.org/finding-comments-in-source-code-using-regular-expressions/ */
COMENTARIO_BLOQUE   \/\*([^*]|[\n]|(\*+([^*\/]|[\n])))*\*+\/
/*Comentario anidado no constituye un lenguaje regular -> no se puede resolver con autómata finito*/
/*solución-> rutina en c*/

/*TERMINACIONES INCORRECTAS PARA FLOTANTES Y INTEGERS (pueden existir más!!)*/
TERMINACIONES_INCORRECTAS_FI        [A-EgGhHjJkKm-tM-Tv-zV-Z]|[&|!\[{]

%%

{INTEGER_LITERAL}                                   {yylval._double = (atof(yytext)); return(_NUM);}

{INTEGER_LITERAL}{TERMINACIONES_INCORRECTAS_FI}     {   imprimeError(7,yylineno,yytext, mode);
                                                        /*desechamos toda la linea*/
                                                        char c;while((c = input()) !='\n');
                                                    }

{FLOAT_LITERAL}                                     {yylval._double = (atof(yytext)); return(_NUM);}

{FLOAT_LITERAL}{TERMINACIONES_INCORRECTAS_FI}       {   imprimeError(8,yylineno,yytext, mode);
                                                        /*desechamos toda la linea*/
                                                        char c;while((c = input()) !='\n');
                                                    }

    /*constantes, variables, funciones, comandos (la tabla de símbolos resuelve el problema)*/        
{IDENTIFICADOR}         {   
                            tipoelem *e = insertarElemento(strdup(yytext), _VAR);
                            yylval.elementoTS = e;  
                            return(e->componenteLexico);
                        }  

{STRING}                {yylval._string = strdup(yytext); return(_STRING);}
{ARCHIVO}               {yylval._string = strdup(yytext); return(_ARCHIVO);}
{MODULO}                {yylval._string = strdup(yytext); return(_MODULO);}

    /*Caracteres que componen en si mismos un componente léxico*/
[ \t]+    /*ignoramos espacios*/;
"\n"      | 
"-"       |
">"       |
"<"       |
"+"       | 
"|"       |
"&"       |
"/"       |
"("       |
")"       |
"["       |
"]"       |
","       |
";"       |
"*"       |
"%"       |
"^"       |
"~"       |
"="       return((char) yytext[0]);

    /*Obviamos un comentario de tipo anidado*/
"/+" {
    void comentario(){
        char c1 = '+';
        char c2 = input();
        do{
            c1=c2;
            c2 = input();
            /*Si llegamos al final y no encontramos cierre -> error*/
            if(c1==(char)0 || c2==(char)0){
                imprimeError(10,yylineno,NULL,mode);
                return;
            }
            if(c1=='/' && c2=='+'){
                unput(c2);
                comentario();  //llamada recursiva
            }
        }while(c1!='+' || c2!='/');
    }
    comentario();
}
{COMENTARIO_LINEA}  ;
{COMENTARIO_BLOQUE} ;

. { /*regla por defecto -> caracter no reconocido por analizador léxico (no debería existir)*/
        if(isascii((int)yytext[0])){
            imprimeError(6,yylineno,yytext,mode);
        }
        /*desechamos línea*/
        char c;
        while((c = input()) !='\n');
        if(mode==0){
            printf("─────────────────────────────────────────────────────\n\n");
            printf("In [%d]:  ", yylineno);
        }
    }

<<EOF>> {   
            /*en principio aqui nunca se debería entrar porque en include_stack el primer
            puntero referencia a stdin (la entrada por teclado).*/
            if(--include_stack_ptr<0){
                yyterminate();
            }  
            // si se llego a un EOF, se elimina de la pila el archivo que se terminó de analizar
            // y se procede a analizar el siguiente elemento de la pila (teclado en caso de que no existan más).
            else{
                yy_delete_buffer(YY_CURRENT_BUFFER);
                yy_switch_to_buffer(include_stack[include_stack_ptr]);
                /*si el puntero a include_stack es 0, 
                estamos tomando datos de stdin -> mostrar resultados por pantalla*/
                if(include_stack_ptr==0){
                    mode=0;
                    yylineno = 1;
                    printf("─────────────────────────────────────────────────────\n\n");
                    printf("In [%d]:  ", yylineno);
                }
            }
        }

%%