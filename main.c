/**
 * Archivo: main.c
 * Descripción: Archivo principal del interprete matemático. Actúa a modo de disparador.
 * Objetivos:
 *       - Inicialización del intéprete.
 *       - Inicialización de la tabla de símbolos.
 *       - Invocación del analizador sintático/semántico.
 */

#include "./headerFiles/Definiciones.h"
int main(int argc, char *argv[])
{
    
    //Invocamos al analizador sintáctico
    yyparse();

    //nota recoger los casos de error y finalización correcta
    return(0);
}
