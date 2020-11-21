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
    if(mode==1)
        printf("\t");
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
    //Si la interacción actual es por teclado, mostramos los print con un formato especifico.
    if(mode==0){
        printf("\x1b[34mOut[%d]: ", yylineno-1);
        for(int i=0; i<strlen(s);i++){
            if(*(s+i) !='"')
                printf("%c", *(s+i));
        }
        printf("\x1b[0m\n");
        nuevaLinea();
    }//si la lectura se hace desde un archivo, no mostramos cabeceras.
    else{
        printf("\t\x1b[34m");
        for(int i=0; i<strlen(s);i++){
            if(*(s+i) !='"')
                printf("%c", *(s+i));
        }
        printf("\x1b[0m\n");
    }
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
    echo=0;
    mode=1;
    if(include_stack_ptr >= MAX_INCLUDE_DEPTH){
        //cambair a yyerror-> numero maximo de cargas recursivas
        printf("No puedes cargar recursivamente mas ficheros");
    }
    else{
        include_stack[include_stack_ptr++] = YY_CURRENT_BUFFER;
        yyin = fopen(path, "r");
        if(!yyin){
            if(--include_stack_ptr<0){
                yyterminate();
            }  
            else{
                yy_delete_buffer(YY_CURRENT_BUFFER);
                yy_switch_to_buffer(include_stack[include_stack_ptr]);
            }
        }else{
            yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
            BEGIN(INITIAL);
            printf("\n\x1b[33m<*>Cargando archivo <%s>\x1b[0m\n", path);
            //reseteamos numeor de lineas
            yylineno=0;
        }
    }
    return((void*)0);
}

