%{
#include <stdio.h>
#include <ctype.h>
#include "./headerFiles/Definiciones.h"
#include "./headerFiles/Errores.h"

int mode=0; //0-> lectura terminal, 1 lectura archivo  
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

/*IDENTIFICADORES (variables, constantes o funciones)*/
IDENTIFICADOR       [_A-Za-z][A-Za-z0-9_]*


/*COMENTARIOS*/
COMENTARIO_LINEA    \/\/.*
/*Referencia: https://blog.ostermiller.org/finding-comments-in-source-code-using-regular-expressions/ */
COMENTARIO_BLOQUE   \/\*([^*]|[\n]|(\*+([^*\/]|[\n])))*\*+\/
/*Comentario anidado no constituye un lenguaje regular -> no se puede resolver con autómata finito*/
/*solución-> rutina en c*/

/*TERMINACIONES INCORRECTAS PARA FLOTANTES Y INTEGERS (pueden existir más!!)*/
TERMINACIONES_INCORRECTAS_FI        [A-EgGhHjJkKm-tM-Tv-zV-Z]|[&|!\[{]

%%

{INTEGER_LITERAL}                                   {printf("\t100 <%s>", yytext);}

{INTEGER_LITERAL}{TERMINACIONES_INCORRECTAS_FI}     {   imprimeError(7,yylineno,yytext, mode);
                                                        /*desechamos toda la linea*/
                                                        char c;while((c = input()) !='\n');
                                                    }

{FLOAT_LITERAL}                                     {printf("\t101 <%s>", yytext);}

{FLOAT_LITERAL}{TERMINACIONES_INCORRECTAS_FI}       {   imprimeError(8,yylineno,yytext, mode);
                                                        /*desechamos toda la linea*/
                                                        char c;while((c = input()) !='\n');
                                                    }

    /*Identificadores y palabras reservadas!! (la tabla de símbolos resuelve el problema)*/        
{IDENTIFICADOR}         {printf("\t104 <%s>",yytext);}    

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
"="       printf("\t105 <%s>", yytext);

"/+" {
    void comentario(){
        char c1 = '+';
        char c2 = input();
        do{
            c1=c2;
            c2 = input();
            if(c1==(char)0 || c2==(char)0){
                imprimeError(10,yylineno,NULL,mode);
                return;
            }
            if(c1=='/' && c2=='+'){
                unput(c2);
                comentario();
            }
        }while(c1!='+' || c2!='/');
    }
    comentario();
}
{COMENTARIO_LINEA}  ;
{COMENTARIO_BLOQUE} ;

. { /*regla por defecto*/
        if(isascii((int)yytext[0])){
            imprimeError(6,yylineno,yytext,mode);
        }
        char c;
        while((c = input()) !='\n');
    }

<<EOF>> {   return(0);  }

%%


