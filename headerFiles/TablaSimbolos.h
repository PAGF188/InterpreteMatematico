#ifndef __TABLA_S
#define __TABLA_S
/**
 * Archivo: TablaSimbolos.h
 * Descripción: Exporta la parte pública de TablaSimbolos.c.
 * Objetivos:
 *       - Exportar las funciones de la tabla de símbolos que necesitarán invocar
 *         el resto de partes del interprete matematico.
 *       - Exportar los tipos de datos que necesitarán emplear el resto de partes
 *         para interactuar correctamente con la TS. 
 */

/**
 * Estructura elegida: Árbol binario de búsqueda.
 * Motivo: En principio un árbol binario de búsqueda puede ser ídoneo para garantizar 
 * un rendimiento "aceptable" y no desperdiciar el uso de memoria. También podríamos 
 * pensar en una tabla Hash; pero en este caso, al no contemplar ámbitos, tendría que 
 * almacenar todas las palabras reservadas/identificadores del código por lo que su 
 * tamaño puede incrementarse notoriamente. Al aumentar su tamaño también aumenta la cantidad
 * de memoria desperdiciada para evitar colisiones (que la función hash funcione mejor).
 * Es decir al empelar árboles binarios conseguimos una mayor eficiencia espacial, 
 * pero perdemos la eficiencia temporal que nos proporciona las 
 * búsquedas/insercciones/eliminaciones "directas" de las tablas Hash.
 * Aún así estas operaciones en los árboles binarios de búsqueda siguen siendo eficientes, 
 * concretamente tienen complejidad temporal logarítmica.
 */

/**
 * @name tipoelem
 * Encapsula uno de los elementos a ser insertados en la TS:
 * - Constate
 * - Funcion
 * - Variable
 */
typedef struct {
    char* lexema;                   //lexema (nombre de la constante, funcion o variable)
    int componenteLexico;           //_FUNCION | _VAR | _CONST | _COMANDO
    char inicializada;               //0-> variable no inicializada, 1 si inicializada
    union{
        double var;                 //valor de la constante o variable
        void* (* funcion_ptr)();   //puntero a la función a ejecutar (o comando)
    }value;
    
}tipoelem;


/**
 * @name crearTablaSimbolos.
 * @objective: Inicializar la tabla.
 */
void crearTablaSimbolos();

/**
 * @name insertarReservados.
 * @objective: introducir en la tabla de símbolos las palabras reservadas del lenguaje: constantes y funciones.
 * @param E, tipoelem
 * @return int. -1 fallo al insertar (ej, elemento ya insertado).
 */
int insertarReservados(tipoelem E);

/**
 * @name insertarElemento. (solo para VARIABLES)
 * A este nivel aun no sabemos su valor.
 * @objective: Comportamiento:
 *           1) Si la variable a insertar ya está dentro de la TS devolver el puntero a su nodo de TS
 *           2) Si la variable no está insertada en la tabla, insertarlo (como variable) y devolver puntero
 *              a nodo TS creado  
 * @param lexema, lexema identificador
 * @param tipo, componente lexico (no sería estrictamente necesario).
 * @return tipoelem. Devuelve el nodo en el que se encuentra el elemento introducido
 */
tipoelem * insertarElemento(char* lexema, int tipo);


/**
 * @name modificar
 * @objective: Modificar una varibale o constante de la tabla de símbolos. 
 * Se sobreescribe su valor por @param valor.
 * @param tipoelem referencia el elemento de la TS a modificar.
 * @param valor. Nuevo valor a insertar.
 */
void modificar(tipoelem *E, double valor);

void eliminar(tipoelem E);

/**
 * @name destruirTablaSimbolos
 * @objective: eliminar la tabla de símbolos liberando toda la memoria.
 */

void destruirTablaSimbolos();

/**
 * @name consultarVariables
 * @objective: imprime todos los elementos de la TS que comparten en componente léxico _VAR
 */
void consultarVariables();

//borrar
void imprimirArbol();

#endif	/* __TABLA_S */