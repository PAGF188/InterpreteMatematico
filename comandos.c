#include <stdlib.h>
#include "sintacticoSemantico.tab.c"
#include "TablaSimbolos.h"

//COMANDOS DEL INTÉRPRETE.

//salir limpiando memoria.
void* salir(){
    destruirTablaSimbolos();
    exit(0);
}

//imprimir la ayuda general
void* ayuda(){
    printf("Aqui irá la ayuda\n");
    return((void*)0);
}

//imprimir todas las variables almacenadas en la TS
void* workspace(){
    printf("\x1b[34m[Workspace]:\n");
    consultarVariables();
    return((void*)0);
}

//activar el echo. Es decir, imprimir el resultado de las operaciones
void* echoon(){
    echo=1;
    printf("\x1b[34mEco activado\x1b[0m\n");
    return((void*)0);
}

//desactivar el echo. Es decir, NO imprimir el resultado de las operaciones
void* echooff(){
    echo=0;
    printf("\x1b[34mEco desactivado\x1b[0m\n");
    return((void*)0);
}

/**
 * Impresión por pantalla de:
 * Expresiones, ej 2+2/1
 * Variables, ej a 
 * Strings, ej "hola"
 */
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

//eliminar un elemento (normalmente variable) de la TS.
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

//cargar un archivo indicado por *path
void* cargar(char *path){
    printf("%s", path);
    FILE* aux = fopen(path,"r");
    if(!aux){
        printf("error");
    }
    /*if(include_stack_ptr >= MAX_INCLUDE_DEPTH){
        //cambair a yyerror-> numero maximo de cargas recursivas
        printf("No puedes cargar recursivamente mas ficheros");
    }
    else{
        include_stack[include_stack_ptr++] = YY_CURRENT_BUFFER;
        yyin = fopen(path, "r");
        if(!yyin){
            printf("error archivo no existe");
        }
        else{
            yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
            BEGIN(INITIAL);
        }
    }*/
    return((void*)0);
}

