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
    int componenteLexico;           //_FUNCION | _VAR | _CONST
    int tipo;                       // 0->int, 1-> double
    union{
        double var;                 //valor de la constante o variable
        double (* funcion_ptr)();   //puntero a la función a ejecutar
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
 * @name insertarVariable.
 * A este nivel aun no sabemos su valor. Lo va a insertar el lexico
 * @objective: Comportamiento:
 *           1) Si el lexema a insertar ya está dentro de la TS devolver entero 
 *             asociado al tipo.
 *           2) Si el lexema no está insertado en la tabla, insertarlo (como variable) y devolver entero
 *             asociado al tipo "variable"
 * @param E, lexema (identificador o palabra reservada).
 * @return int. Devuelve el valor entero asociado al componente léxico de "lexema".
 */
tipoelem * insertarElemento(char* lexema, int tipo);

/**
 * @name destruirTablaSimbolos
 * @objective: eliminar la tabla de símbolos liberando toda la memoria.
 */

//-1 no existe
int _buscar_nodo(char * cl, tipoelem *nodo);

void _suprimir(tipoelem E);

void destruirTablaSimbolos();

//borrar
void imprimirArbol();

#endif	/* __TABLA_S */