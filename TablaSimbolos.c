/**
 * Archivo: TablaSimbolos.c
 * Descripción: Implementa la tabla de símbolos del compilador.
 * Funciones:
 *       - Almacenar las palabras reservadas del lenguaje.
 *       - Almacenar los identificadores insertados por el analizador léxico.
 *       - Permitir distinguir al analizador léxico si una cadena alfanumérica es un identificador
 *         o una palabra reservada. 
 *       - Devolver el componente léxico de cada lexema almacenado.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "./headerFiles/TablaSimbolos.h"
#include "./headerFiles/Definiciones.h"

//Nodo del árbol. Información y punteros a hijo derecho/izquierdo.
struct celda {
	tipoelem info;
	struct celda *izq, *der;
};
typedef struct celda *abb;

//Clave de búsqueda. ES EL LEXEMA. 
typedef char* tipoclave;

/////// VARIABLE GLOBAL CON LA TABLA DE SÍMBOLOS (nodo raíz)
abb tablaSimbolos;



/////////////////// DEFINICIÓN FUNCIONES PRIVADAS

//Compara las claves de dos nodos (dos lexemas). (0 iguales. -1 cl1<cl2. 1 cl1>cl2)
int _comparar_claves(tipoclave cl1, tipoclave cl2);
// Extraer la clave de una celda (el lexema de un nodo)
tipoclave _clave_elem(tipoelem * E);
// compara una clave (cl) con la clave del nodo E. [0 iguales. -1 cl1<cl2. 1 cl1>cl2]
int _comparar_clave_elem(tipoclave cl, tipoelem E);
// Devuelve 0 si A es nulo, 1 en caso contrario.
unsigned es_vacio(abb A);
// Devuelve la referencia al hijo derecho de A.
abb der(abb A);
// Devuelve la referencia al hijo izquierdo de A.
abb izq(abb A);
// Inserta en A el elemento E.
void insertar(abb *A, tipoelem E);
// Almacena en E la referencia a la información del nodo A. 
void info(abb A, tipoelem *E);
// 1 si el nodo con lexema cl está ya en el árbol. 0 caso contrario
unsigned es_miembro_clave(abb A, tipoclave cl);
// devuele en *nodo, el nodo cuya clave coincida con cl.
void buscar_nodo(abb A, tipoclave cl, tipoelem *nodo);
// eliminar el arbol y limpia la memoria.
void destruir_arbol(abb *A);
//Las dos siguientes permiten eliminar un nodo concreto del árbol.
void suprimir(abb *A, tipoelem E);
tipoelem _suprimir_min(abb *A);
//Recorre el árbol imprimiendo todos los elementos con componente léxico _VAR
void imprime_variables(abb A);


// BORRAR!!!!!! Función de comprobación. Imprime toda la tabla de símbolos. Recorrido inorden del árbol.
void imprime_tabla(abb A){
    tipoelem E;
    if (!es_vacio(A)){
        imprime_tabla(izq(A));
        info(A, &E);
        printf("%s, %d, %lf\n", E.lexema, E.componenteLexico,E.value.var);
        imprime_tabla(der(A));
    }
}

/////////////////// FUNCIONES PÚBLICAS (definición en .h)

void crearTablaSimbolos(){
	tablaSimbolos = NULL;
}

void destruirTablaSimbolos() {
    destruir_arbol(&tablaSimbolos);
}

int insertarReservados(tipoelem E){
    //Si no está activado el campo de palabra reservada
    // o lexema es nulo o palabra reservada ya insertada -> error.
    if(E.lexema==NULL || es_miembro_clave(tablaSimbolos,E.lexema)){
        return(-1);
    }
    insertar(&tablaSimbolos,E);
    return(1);
}

tipoelem * insertarElemento(char* lexema, int tipo){
    tipoelem *nodo=NULL;
    nodo = (tipoelem *) malloc(sizeof(tipoelem));
    //Si ya está el nodo en la tabla de símbolos
    if(es_miembro_clave(tablaSimbolos,lexema)){
        buscar_nodo(tablaSimbolos,lexema,nodo);
        free(lexema);
    }
    //si no está es un identificador -> lo insertamos y devolvemos id asignado.
    else{
        nodo->lexema = lexema; 
        nodo->componenteLexico = tipo;
        nodo->value.var = 0.0;
        nodo->inicializada = 0;
        insertar(&tablaSimbolos,*nodo);
    }
    return(nodo);
}

void imprimirArbol(){
    imprime_tabla(tablaSimbolos);
}

int _buscar_nodo(char * cl, tipoelem *nodo){
    if(!es_miembro_clave(tablaSimbolos, cl))
        return(-1);
    buscar_nodo(tablaSimbolos, cl, nodo);
    return(0);
}

void eliminar(tipoelem E){
    suprimir(&tablaSimbolos, E);
}

void modificar(tipoelem *E, double valor){
    tipoelem e;
    e.lexema = E->lexema;
    e.componenteLexico = E->componenteLexico;
    e.value.var = valor;
    e.inicializada = 1;
    eliminar(*E);
    insertar(&tablaSimbolos, e);
}

void consultarVariables(){
    imprime_variables(tablaSimbolos);
}

int esMiembro(char *s){
    tipoelem nodo;
    buscar_nodo(tablaSimbolos, s, &nodo);
    return(nodo.inicializada!=0);
}

/////////////////// IMPLEMENTACIÓN FUNCIONES PRIVADAS
int _comparar_claves(tipoclave cl1, tipoclave cl2){
    return strcmp(cl1,cl2)==0 ? 0 : strcmp(cl1,cl2)>0 ? 1 : -1; 
}

int _comparar_clave_elem(tipoclave cl, tipoelem E){
	return _comparar_claves(cl, _clave_elem(&E)); 
}

tipoclave _clave_elem(tipoelem * E){
	return (E->lexema);
}

unsigned es_vacio(abb A) {
	return A == NULL;
}

abb izq(abb A) {
    return A->izq;
}

abb der(abb A) {
    return A->der;
}

void insertar(abb *A, tipoelem E) {
    if (es_vacio(*A)) {
        *A = (abb) malloc(sizeof (struct celda));
        (*A)->info = E;
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return;
    } 
    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info );
    if (comp > 0 ) {
        insertar(&(*A)->der, E);
    } else {
        insertar(&(*A)->izq, E);
    }
}

void info(abb A, tipoelem *E){
    *E = A->info;
}

unsigned es_miembro_clave(abb A, tipoclave cl) {
    if (es_vacio(A)) {
        return 0;
    } 
    int comp = _comparar_clave_elem(cl, A->info);
    if (comp == 0) {
        return 1;
    } 
    if (comp > 0) {
        return es_miembro_clave(A->der, cl);
    } 
    return es_miembro_clave(A->izq, cl);
}

void buscar_nodo(abb A, tipoclave cl, tipoelem *nodo) {
    if (es_vacio(A)) {
        return;
    } 
    int comp = _comparar_clave_elem(cl, A->info);
    
	if (comp == 0) {
    	*nodo = A->info;    //aqui almacenamos referencia
	} else if (comp < 0) {
	    buscar_nodo(A->izq, cl, nodo);
	} else {
	    buscar_nodo(A->der, cl, nodo);
	}
}

void destruir_arbol(abb *A) {
	if (!es_vacio(*A)) {
		destruir_arbol(&((*A)->izq));
		destruir_arbol(&((*A)->der));
		free(*A);
		*A = NULL;
	}
}


//Las dos siguientes permiten eliminar un nodo concreto del árbol.
//Sin embargo, en esta práctica no se usan. Quedan hechas por si se necesitan en la siguiente.

void suprimir(abb *A, tipoelem E) {
    abb aux;
    if(es_vacio(*A)){
       return;
    }
    
    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if(comp < 0){ 
        suprimir(&(*A)->izq, E);
    } else if (comp > 0){ 
        suprimir(&(*A)->der, E);
    } else if (es_vacio((*A)->izq) && es_vacio((*A)->der)) {
        free(*A);
        *A = NULL;
    } else if (es_vacio((*A)->izq)) { 	// pero no es vacio derecha
        aux = *A;
        *A = (*A)->der;
        free(aux);
    } else if (es_vacio((*A)->der)) { 	//pero no es vacio izquierda
        aux = *A;
        *A = (*A)->izq;
        free(aux);
    } else { //ni derecha ni izquierda esta vacio
        (*A)->info = _suprimir_min(&(*A)->der);
    }
}

tipoelem _suprimir_min(abb *A) {
    abb aux;
    tipoelem ele;
    if (es_vacio((*A)->izq)) {
        ele = (*A)->info;
        aux = *A;
        *A = (*A)->der;
        free(aux);
        return ele;
    } else {
        return _suprimir_min(&(*A)->izq);
	}
}

void imprime_variables(abb A){
    tipoelem E;
    if (!es_vacio(A)){
        imprime_variables(izq(A));
        info(A, &E);
        if(E.componenteLexico == _VAR){
            printf("\x1b[34m\t-> %s = %.10g\n", E.lexema, E.value.var);
        }
        printf("\x1b[0m");
        imprime_variables(der(A));
    }
}
