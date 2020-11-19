#include <stdlib.h>
#include "sintacticoSemantico.tab.c"
#include "TablaSimbolos.h"
//mantenemos el double para que concida con el tipo definido en TS.h
//pero ignoramos valor de retorno.

double salir(){
    destruirTablaSimbolos();
    exit(0);
}

double ayuda(){
    printf("Aqui irá la ayuda\n");
    return(0.0);
}

double workspace(){
    consultarVariables();
    return(0.0);
}

