/**
 * Archivo: main.c
 * Descripción: Archivo principal del interprete matemático. Actúa a modo de disparador.
 * Objetivos:
 *       - Inicialización del intéprete.
 *       - Inicialización de la tabla de símbolos.
 *       - Invocación del analizador sintático/semántico.
 */
#include <stdio.h>
#include "./headerFiles/TablaSimbolos.h"
#include "./headerFiles/Definiciones.h"

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862
#define E 2.71828182845904523536028747135266249775724709369995957496696762772407663035354



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

/**
 * Crea las constantes y funciones para insertar de base en la TS
 */
void cargarElementosIniciales(){

    //constantes
    char * const_nombres[2] = {"e", "PI"};    
    double const_valores[2] = {E,PI};   
    for(int i=0; i<sizeof(const_valores)/sizeof(const_valores[0]); i++){
        tipoelem e;
        e.lexema = const_nombres[i];
        e.componenteLexico = _CONST;
        e.value.var = const_valores[i];
        insertarReservados(e);
    }

    //funciones
    char * funciones_nombres[6] = {"sin", "cos", "atan", "ln", "exp", "sqrt"};
    
}

int main(int argc, char *argv[]){

    
    //Iniciamos la tabla de símbolos con constantes y funciones reservadas
    crearTablaSimbolos();

    //cargamos los elementos iniciales en TS
    cargarElementosIniciales();

    
    presentacion();
    //Invocamos al analizador sintáctico
    yyparse();
    //nota recoger los casos de error y finalización correcta
    return(0);
}
