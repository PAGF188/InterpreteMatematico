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

void* echoon(){
    echo=1;
    printf("\x1b[34mEco activado\x1b[0m\n");
    return((void*)0);
}

void* echooff(){
    echo=0;
    printf("\x1b[34mEco desactivado\x1b[0m\n");
    return((void*)0);
}

void* print(char *s){
    printf("\x1b[34mOut[%d]: ", yylineno-1);
    for(int i=0; i<strlen(s);i++){
        if(*(s+i) !='"')
            printf("%c", *(s+i));
    }
    printf("\x1b[0m\n");
    nuevaLinea();
    return((void*)0);
}

void* delete(tipoelem *elemento){
    //si no es miembro, imprimimos advertencia.
    if(!esMiembro(elemento->lexema)){
        codigoError = 11;
        yyerror(elemento->lexema);
    }else{
        nuevaLinea();
    }
    //como el lexico inserta preinserta siempre la VAR en la TS, la borramos.
    eliminar(*elemento);
    free(elemento);
    return((void*)0);
}

