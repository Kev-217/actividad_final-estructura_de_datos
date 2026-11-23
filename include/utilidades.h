// Si UTILIDADES_H no ha sido definido, entonces define UTILIDADES_H y el contenido del archivo. Esto evita que el archivo se incluya múltiples veces en un proyecto, lo que podría causar errores de compilación.
#ifndef UTILIDADES_H
#define UTILIDADES_H

/* leer_entero: 1 = correcto, 0 = entrada invalida, -1 = fin de entrada. */
int leer_entero(const char *mensaje, int *valor);

/* Lee n valores, uno por linea. 1 = correcto, -1 = fin de entrada. */
int leer_arreglo_manual(int arreglo[], int n);

/* Lee n valores en una sola linea separados por espacios. */
/* 1 = correcto, 0 = entrada invalida, -1 = fin de entrada. */
int leer_arreglo_pegado(int arreglo[], int n);

/* tipos: 1 aleatorio, 2 ordenado, 3 inverso, 4 casi ordenado. */
void generar_arreglo(int arreglo[], int n, int tipo);

/* Muestra los elementos de un arreglo. */
void imprimir_arreglo(const int arreglo[], int n);

/* Copia los elementos de un arreglo en otro. */
void copiar_arreglo(const int origen[], int destino[], int n);

/* Comprueba si el arreglo esta ordenado de menor a mayor. */
int esta_ordenado(const int arreglo[], int n);

/* Devuelve el tiempo actual en segundos usando un reloj de alta resolucion. */
double tiempo_actual_segundos(void);

#endif
// Esto es un comentario que indica que el archivo de encabezado ha sido cerrado correctamente.