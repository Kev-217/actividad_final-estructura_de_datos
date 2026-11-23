// Si BUSQUEDA_H no ha sido definido, entonces define BUSQUEDA_H y el contenido del archivo. Esto evita que el archivo se incluya múltiples veces en un proyecto, lo que podría causar errores de compilación.
// Define las funciones de busqueda_lineal y busqueda_binaria, que se implementan en el archivo busqueda.c
#ifndef BUSQUEDA_H
#define BUSQUEDA_H

/*
 * Busca un valor recorriendo el arreglo de izquierda a derecha.
 * Devuelve el indice encontrado o -1 si no existe.
 * comparaciones debe apuntar a una variable donde se guardara el conteo.
 */
int busqueda_lineal(const int arreglo[], int n, int objetivo, unsigned long long *comparaciones);

/*
 * Busca un valor mediante division sucesiva a la mitad.
 * IMPORTANTE: el arreglo debe estar ordenado de menor a mayor.
 * Devuelve el indice encontrado o -1 si no existe.
 */
int busqueda_binaria(const int arreglo[], int n, int objetivo, unsigned long long *comparaciones);

#endif
// Esto es un comentario que indica que el archivo de encabezado ha sido cerrado correctamente.
