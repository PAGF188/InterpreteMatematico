/**
 * Archivo: main.c
 * Descripción: Archivo principal del interprete matemático. Actúa a modo de disparador.
 * Objetivos:
 *       - Inicialización del intéprete.
 *       - Inicialización de la tabla de símbolos.
 *       - Invocación del analizador sintático/semántico.
 */
#include<stdio.h>
#include "./headerFiles/Definiciones.h"

//Para mostrar de forma bonita el inicio del programa.
//Inicamos la existencia de la funcion ayuda (?)
void presentacion(){
    printf("\n");
    printf(" _   _   _  ___  ___  _   _   _  ___  _   __   _  \n");
    printf("| \\_/ | / \\|_ _|| __|| \\_/ | / \\|_ _|| | / _| / \\ \n");
    printf("| \\_/ || o || | | _| | \\_/ || o || | | |( (_ | o | \n");
    printf("|_| |_||_n_||_| |___||_| |_||_n_||_| |_| \\__||_n_| \n");
    printf("===================================================\n");  
    printf("             REALIZA TUS OPERACIONES\n");  
    printf("===================================================\n\n"); 
    printf("[Ejecuta ? para la ayuda general]\n");
    printf("[<funcion>? para consultar la definición de <funcion>]\n\n");
    printf("In [1]:  ");
}

int main(int argc, char *argv[]){
    
    presentacion();
    //Invocamos al analizador sintáctico
    yyparse();
    //nota recoger los casos de error y finalización correcta
    return(0);
}
