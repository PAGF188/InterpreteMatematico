#ifndef __ERRORES
#define __ERRORES

/**
 * Archivo: Errores.h
 * Descripción: Exporta la parte pública de Errores.c.
 * Objetivos:
 *       - Exportar las funciones del gestor de errores que necesitarán invocar
 *         el resto de partes del intérprete.
 */

/**
 * @name imprimeError
 * @objective: imprimir el error correspondiente en función de su código.
 * @param codigo, valor identificativo del error. Puede ser:
 *                (*) 2 -> no se pudo acceder al archivo a compilar. 
 *                (*) 6-> caracter no conocido.
 *                (*) 7-> entero mal formado.
 *                (*) 8-> flotante mal formado.
 *                (*) 9-> falta la comilla de cierre del String
 *                (*) 10-> falta cierre de comentario +/
 *                (*) 11-> se uso una variable sin inicializar
 *                (*) 12 -> intento de deivisón entre 0
 *                (*) 13 -> el comando no acepta modificadores
 *                (*) 14 -> el tamaño de punteros a archivos en pila supera el maximo pertmitido
 *                (*) 15 -> error al cargar .so
 *                (*) 16 -> numero máximo de .so abiertas
 * @param linea del error.
 * @param lexema
 * @param mode, 0 no imprimir linea, 1 imprimir linea.
 */
void imprimeError(int codigo, int linea, char * lexema, int mode);


#endif	/* __ERRORES */