#define _GNU_SOURCE
#include <stdlib.h>
#include <dlfcn.h>
#include "./headerFiles/link.h"
#include "sintacticoSemantico.tab.c"
#include "./headerFiles/TablaSimbolos.h"

#define MAX_L 100

//Estructura para almacenar los handlers de las bibliotecas abiertas con dlopen().
//y poder cerrarlas al terminar ejecución.
void *handles[MAX_L];
int n_handles=0;


//COMANDOS DEL INTÉRPRETE. El valor de retorno se ignora.
//Se hizo asi para poder aprovechar el mismo campo que para funciones.

//salir limpiando memoria.
double salir(){
    destruirTablaSimbolos();
    exit(0);
}

//imprimir la ayuda general
double ayuda(){
    printf("Aqui irá la ayuda\n");
    return(0);
}

//imprimir todas las variables almacenadas en la TS
double workspace(){
    if(mode==1)
        printf("\t");
    printf("\x1b[34m[Workspace]:\n");
    consultarVariables();
    return(0);
}

//activar el echo. Es decir, imprimir el resultado de las operaciones
double echoon(){
    echo=1;
    printf("\x1b[34mEco activado\x1b[0m\n");
    return(0);
}

//desactivar el echo. Es decir, NO imprimir el resultado de las operaciones
double echooff(){
    echo=0;
    printf("\x1b[34mEco desactivado\x1b[0m\n");
    return(0);
}

/**
 * Impresión por pantalla de:
 * Expresiones, ej 2+2/1
 * Variables, ej a 
 * Strings, ej "hola"
 */
double print(char *s){
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
    return(0);

}

//eliminar un elemento (normalmente variable) de la TS.
double delete(tipoelem *elemento){
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
    return(0);
}

//cargar un archivo indicado por *path
double cargar(char *path){

    //comprobación inicial de que EL FICHERO EXISTE
    if(!fopen(path, "r")){
        codigoError = 2;
        yyerror(path);
        return(0);
    }

    echo=0;
    mode=1;
    //Si el número de punteros a archivos en pila supera el limite permitido: error catastrófico. 
    if(include_stack_ptr >= MAX_INCLUDE_DEPTH){
        codigoError = 14;
        yyerror("");
        salir();
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
    return(0);
}

static int callback(struct dl_phdr_info *info, size_t size, void *data){
    int j;

   printf("name=%s (%d segments)\n", info->dlpi_name,
        info->dlpi_phnum);

   for (j = 0; j < info->dlpi_phnum; j++)
         printf("\t\t header %2d: address=%10p\n", j,
             (void *) (info->dlpi_addr + info->dlpi_phdr[j].p_vaddr));
    return 0;
}

//include
double include(char *path){

    if(n_handles>=MAX_L){
        codigoError = 16;
        yyerror("");
        return(0);
    }
    void *handle;   //manejador de la biblioteca

    //intentamos cargar en memoria  la biblioteca compilada
    handle = dlopen(path, RTLD_LAZY);
    if(!handle){
        codigoError = 15;
        yyerror((char *)dlerror);
        return(0);
    }

    dl_iterate_phdr(callback, NULL);

    return(0);
}


