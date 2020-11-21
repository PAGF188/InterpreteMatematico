#include <stdlib.h>
#include "sintacticoSemantico.tab.c"
#include "TablaSimbolos.h"
//mantenemos el double para que concida con el tipo definido en TS.h
//pero ignoramos valor de retorno.

void* salir(){
    destruirTablaSimbolos();
    exit(0);
}

void* ayuda(){
    printf("Aqui irá la ayuda\n");
    return((void*)0);
}

void* workspace(){
    printf("\x1b[34m[Workspace]:\n");
    consultarVariables();
    return((void*)0);
}

void *echoon(){
    echo=1;
    printf("\x1b[34mEco activado\n \x1b[0m");
    return((void*)0);
}

void *echooff(){
    echo=0;
    printf("\x1b[34mEco desactivado\n \x1b[0m");
    return((void*)0);
}

