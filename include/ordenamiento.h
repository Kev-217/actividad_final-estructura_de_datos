// Si ORDENAMIENTO_H no ha sido definido, entonces define ORDENAMIENTO_H y el contenido del archivo. Esto evita que el archivo se incluya múltiples veces en un proyecto, lo que podría causar errores de compilación.
// Define las funciones de ordenamiento_insercion y quick_sort, que se implementan en el archivo ordenamiento.c
#ifndef ORDENAMIENTO_H
#define ORDENAMIENTO_H

/* Ordena el arreglo de menor a mayor mediante insercion. */
void ordenamiento_insercion(int arreglo[], int n, unsigned long long *comparaciones);

/* Ordena el arreglo de menor a mayor mediante Quick Sort. */
void quick_sort(int arreglo[], int n, unsigned long long *comparaciones);

#endif
// Esto es un comentario que indica que el archivo de encabezado ha sido cerrado correctamente.
