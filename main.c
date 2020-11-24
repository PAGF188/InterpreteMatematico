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
#include "comandos.c"

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862
#define E 2.71828182845904523536028747135266249775724709369995957496696762772407663035354

/*Tipo de dato para encapsular la infocmación de los comandos que es estática*/
typedef struct {
    char * nombre;
    double (* funcion_ptr)();   //puntero a la función a ejecutar (o comando)
}comandos;

//Para mostrar de forma bonita el inicio del programa.
//Inicamos la existencia de la funcion ayuda (?)
void presentacion(){
    printf("\n");
    printf("\x1b[34m _   _   _  ___  ___  _   _   _  ___  _   __   _  \n");
    printf("\x1b[31m| \\_/ | / \\|_ _|| __|| \\_/ | / \\|_ _|| | / _| / \\ \n");
    printf("\x1b[31m| \\_/ || o || | | _| | \\_/ || o || | | |( (_ | o | \n");
    printf("\x1b[34m|_| |_||_n_||_| |___||_| |_||_n_||_| |_| \\__||_n_| \n");
    printf("\x1b[0m===================================================\n");  
    printf("\x1b[34m             REALIZA TUS OPERACIONES\n");  
    printf("\x1b[0m===================================================\n\n"); 
    printf("\x1b[0m[Ejecuta ? para la ayuda general]\n");
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

    //comandos
    comandos _comandos[] = {
        {"salir", salir},
        {"?", ayuda},
        {"workspace", workspace},
        {"echoon", echoon},
        {"echooff", echooff},
        {"print", print},
        {"delete", delete},
        {"load", cargar},
        {"include", include},
        {0, 0}
    };

    for (int i = 0; _comandos[i].nombre != 0; i++){
        tipoelem aux;
        aux.lexema = _comandos[i].nombre;
        aux.componenteLexico = _COMANDO;
        if(strcmp("delete", aux.lexema)==0)
            aux.componenteLexico = _DELETE;
        aux.value.funcion_ptr = _comandos[i].funcion_ptr;
        insertarReservados(aux);
    }

    //FUNCIONES ESTÁTICAS
    comandos _funciones[] = {
        {"acos",  acos},
        {"asin",  asin},
        {"atan",  atan},
        {"cos",  cos},
        {"cosh", cosh},
        {"sin",  sin},
        {"sinh", sinh},
        {"tanh", tanh},
        {"exp",  exp},
        {"log",  log},
        {"log10", log10},
        {"ceil",  ceil},
        {"fabs",  fabs},
        {"floor", floor},
        {0, 0}
    };

    for (int i = 0; _funciones[i].nombre != 0; i++){
        tipoelem aux;
        aux.lexema = _funciones[i].nombre;
        aux.componenteLexico = _FUNCION;
        aux.value.funcion_ptr = _funciones[i].funcion_ptr;
        insertarReservados(aux);
    }
}

int main(int argc, char *argv[]){

    
    //Iniciamos la tabla de símbolos con constantes y funciones reservadas
    crearTablaSimbolos();

    //cargamos los elementos iniciales en TS
    cargarElementosIniciales();
    //imprimirArbol();
    
    presentacion();
    //Invocamos al analizador sintáctico
    yyparse();
    //nota recoger los casos de error y finalización correcta
    destruirTablaSimbolos();
    return(0);
}
