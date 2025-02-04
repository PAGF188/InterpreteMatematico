#include <stdlib.h>
#include <dlfcn.h>
#include "sintacticoSemantico.tab.c"
#include "./headerFiles/TablaSimbolos.h"


/////// FUNCIONES PRIVADAS
//Recorre los handles y los cierra
void descargarModulos();

/////////COMANDOS DEL INTÉRPRETE. 
//El valor de retorno se ignora. Se hizo asi para poder aprovechar el mismo campo que para funciones.

//// SALIR
double salir(){
    destruirTablaSimbolos();
    exit(0);
}

//imprimir la ayuda (general y especifica en función del valor pasado como argumento).
//caracteres:  ┌ ┐ ─ ┘ └ │
double ayuda(char *tipo){
    //ayuda general.
    if(strcmp("vacio", tipo)==0 || strcmp("?", tipo)==0){
        printf("┌───────────────────────────────────────────────────┐\n");  
        printf("│ - Expresiones: (valor <operando> valor)           │\n");
        printf("│ - Asignaciones: variable = valor                  │\n");
        printf("│ - Funciones: func()                               │\n");
        printf("│ - Comandos: salir, workspace, echoon, echooff,    │\n");
        printf("│             print, delete, load, include          │\n");
        printf("│ - <comando> ? para acceder a su descripcion       │\n");
        printf("└───────────────────────────────────────────────────┘\n");  
    }
    //el resto son casos especificos para comandos
    else if(strcmp("salir", tipo)==0){
        printf("┌───────────────────────────────────────────────────┐\n");  
        printf("│ - Sintaxis: <salir>                               │\n");
        printf("│ - Permite abandonar el interprete                 │\n");
        printf("└───────────────────────────────────────────────────┘\n");  
    }
    else if(strcmp("workspace", tipo)==0){
        printf("┌───────────────────────────────────────────────────┐\n");  
        printf("│ - Sintaxis: <workspace>                           │\n");
        printf("│ - Imprime pares id:valor para todas las variables │\n");
        printf("│   definidas                                       │\n");
        printf("└───────────────────────────────────────────────────┘\n"); 
    }
    else if(strcmp("echoon", tipo)==0){
        printf("┌───────────────────────────────────────────────────┐\n");  
        printf("│ - Sintaxis: <echoon>                              │\n");
        printf("│ - Activa el modo echo. Impresion del resultado de │\n");
        printf("│   las instrucciones ejecutadas                    │\n");
        printf("└───────────────────────────────────────────────────┘\n"); 
    }
    else if(strcmp("echooff", tipo)==0){
        printf("┌───────────────────────────────────────────────────┐\n");  
        printf("│ - Sintaxis: <echooff>                             │\n");
        printf("│ - Desactiva el modo echo. No se imprime el        │\n");
        printf("│   resultado de las instrucciones ejecutadas.      │\n");
        printf("│   print no se ve afectado                         │\n");
        printf("└───────────────────────────────────────────────────┘\n"); 
    }
    else if(strcmp("print", tipo)==0){
        printf("┌───────────────────────────────────────────────────┐\n");  
        printf("│ - Sintaxis: <print> <texto>                       │\n");
        printf("│ - Imprime por pantalla el resultado de <texto>.   │\n");
        printf("│   <texto> puede ser un string, una expresion      │\n");
        printf("│   matematica o una variable.                      │\n");
        printf("└───────────────────────────────────────────────────┘\n"); 
    }
    else if(strcmp("delete", tipo)==0){
        printf("┌───────────────────────────────────────────────────┐\n");  
        printf("│ - Sintaxis: <delete> <var>                        │\n");
        printf("│ - Elimina de la sesion actual la variable <var>   │\n");
        printf("└───────────────────────────────────────────────────┘\n"); 
    }
    else if(strcmp("load", tipo)==0){
        printf("┌───────────────────────────────────────────────────┐\n");  
        printf("│ - Sintaxis: <load> <archivo>                      │\n");
        printf("│ - Carga en la sesión actual el contenido de       │\n");
        printf("│   <archivo>. <archivo> debe tener como terminacion│\n");
        printf("│   .txt o .mat                                      │\n");
        printf("└───────────────────────────────────────────────────┘\n"); 
    }
    else if(strcmp("include", tipo)==0){
        printf("┌───────────────────────────────────────────────────┐\n");  
        printf("│ - Sintaxis: <include> <archivo>.so,<funcion>      │\n");
        printf("│ - Carga en la sesión actual la <funcion> de       │\n");
        printf("│   <archivo>. <funcion> debe estar compilada.      │\n");
        printf("└───────────────────────────────────────────────────┘\n"); 
    }
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

//eliminar uan variable de la TS.
double delete(tipoelem *elemento){
    //si no es miembro, imprimimos advertencia.
    if(!esMiembro(elemento->lexema)){
        codigoError = 11;
        yyerror(elemento->lexema);
    }else{
        nuevaLinea();
    }
    //como el lexico preinserta siempre la VAR en la TS, la borramos.
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
        //almacenamos el buffer actual en la pila (para poder recuperarlo después)
        include_stack[include_stack_ptr++] = YY_CURRENT_BUFFER;
        //abrimos el archivo a cargar y se lo pasamos a léxico.
        yyin = fopen(path, "r");
        //si falla, deshacemos los cambios hechos en la pila
        if(!yyin){
            if(--include_stack_ptr<0){
                yyterminate();
            }  
            else{
                yy_delete_buffer(YY_CURRENT_BUFFER);
                yy_switch_to_buffer(include_stack[include_stack_ptr]);
            }
        //si la lectura funciona iniciamos en análisis del nuevo buffer.
        }else{
            yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
            BEGIN(INITIAL);
            printf("\n\x1b[33m<*>Cargando archivo <%s>\x1b[0m\n", path);
            //reseteamos numeor de lineas
            yylineno=0;
        }
    }
    echo=1;
    return(0);
}

//include
//include archivo.so, funcion
double include(char *path, char *func){

    void *handle;   //manejador de la biblioteca
    //intentamos cargar en memoria  la biblioteca compilada
    handle = dlopen(path, RTLD_LAZY);
    if(!handle){
        //si falle -> error
        codigoError = 15;
        yyerror((char *)dlerror());
        return(0);
    }
    //si tenemos éxito cargamos la función y almacenamos su referencia en la TS
    tipoelem aux;
    aux.lexema = func;
    aux.componenteLexico = _FUNCION;
    aux.value.funcion_ptr = dlsym(handle, func);   //la cargamos
    //chequeamos error durante la carga
    char *_error;
    if((_error = dlerror()) != NULL){
        codigoError = 15;
        yyerror(_error);
        return(0);
    }
    
    //si todo va bien la insertamos pero.....
    ////OJO!! Como func es un identificador ya fue insertado por la TS como variable.
    //Primero eliminar
    eliminar(aux);
    insertarReservados(aux);
    if(mode==0) nuevaLinea();
    return(0);
}

