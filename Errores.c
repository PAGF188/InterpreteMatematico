/**
 * Archivo: Errores.c
 * Descripción: Centraliza la gestión de errores
 * Objetivos:
 *       - Mostrar el error correspondiente en función del valor de entrada.
 */


#include <stdio.h>
#include "./headerFiles/Errores.h"

//Las definiciones de las funciones públicas se encuentran en el .h

void imprimeError(int codigo, int linea, char * lexema, int mode){

    switch (codigo)
    {
    case 2: 
        printf("Semantic Error! : El archivo <%s> no es accesible.\n", lexema);
        break;
    case 6:
        if(mode==1)
            printf("\t(linea %d) Lexical Error! Caracter extraño: %s\n",linea,lexema);
        else
            printf("Lexical Error! Caracter extraño: %s\n",lexema);
        break;
    case 7:
        if(mode==1)
            printf("\t(linea %d) Lexical Error! Entero mal formado: %s\n",linea, lexema);
        else
            printf("Lexical Error! Entero mal formado: %s\n",lexema);
        break;
    case 8:
        if(mode==1)
            printf("\t(linea %d) Lexical Error! Flotante mal formado: %s\n",linea, lexema);
        else
            printf("Lexical Error! Flotante mal formado: %s\n", lexema);
        break;
     case 9:
     if(mode==1)
        printf("\t(linea %d) Lexical Error! Falta la \" de cierre del String: \" \n",linea);
    else
         printf("Lexical Error! Falta la \" de cierre del String: \" \n");
        break;
    case 10:
        if(mode==1)
            printf("\t(linea %d) Lexical Error! Falta cierre del comentario: +/\n",linea);
        else
            printf("Lexical Error! Falta cierre del comentario: +/\n");
        break;
    case 11:
        if(mode==1)
            printf("\t(linea %d) Semantic Error! La variable <%s> no fue inicializada\n",linea, lexema);
        else
            printf("Semantic Error! La variable <%s> no fue inicializada\n",lexema);
        break;
    case 12:
        if(mode==1)
            printf("\t(linea %d) Semantic Error! Intento de división entre 0 \n",linea);
        else
            printf("Semantic Error! Intento de división entre 0 \n");
        break;
    case 13:
        if(mode==1)
            printf("\t(linea %d) Semantic Error! Modificador invalido para el comando <%s> \n",linea, lexema);
        else
            printf("Semantic Error! Modificador invalido para el comando <%s> \n", lexema);
        break;
    case 14:
            printf("Catastrophic Error! El número de archivos en pila para cargar supera el máximo permitido.\n");
        break;
    case 15:
        if(mode==1)
            printf("\t(linea %d) Semantic Error! Error al cargar el modulo: %s \n",linea, lexema);
        else
            printf("Semantic Error! Error al cargar el modulo: %s \n",lexema);
        break;
    case 16:
        if(mode==1)
            printf("\t(linea %d) Semantic Error! Numero maximo de modulos importados. \n", linea);
        else
            printf("Semantic Error! Numero maximo de modulos importados.\n");
        break;
    default:
        break;
    }
}