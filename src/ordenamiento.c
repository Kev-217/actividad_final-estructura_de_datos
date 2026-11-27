// Incluye <stddef.h> para disponer de NULL y "ordenamiento.h" con las declaraciones de ordenamiento.
#include <stddef.h>
#include "ordenamiento.h"

// Esta función intercambia los valores de dos enteros utilizando punteros. Se utiliza en el algoritmo de ordenamiento rápido (quick sort) para mover elementos en el arreglo.
static void intercambiar(int *a, int *b)
{
    int temporal = *a;
    *a = *b;
    *b = temporal;
}

// Implementación del algoritmo de ordenamiento por inserción. +
// Este algoritmo recorre el arreglo y va insertando cada elemento en su posición correcta dentro de la parte ya ordenada del arreglo.
// Se utiliza un contador de comparaciones para medir la eficiencia del algoritmo.
void ordenamiento_insercion(int arreglo[], int n, unsigned long long *comparaciones)
{
    if (comparaciones != NULL)
    {
        *comparaciones = 0;
    }

    for (int i = 1; i < n; i++)
    {
        int clave = arreglo[i];
        int j = i - 1;

        while (j >= 0)
        {
            if (comparaciones != NULL)
            {
                (*comparaciones)++;
            }
            if (arreglo[j] > clave)
            {
                arreglo[j + 1] = arreglo[j];
                j--;
            }
            else
            {
                break;
            }
        }

        arreglo[j + 1] = clave;
    }
}

/* Particion: el ultimo elemento funciona como pivote. */
static int particionar(int arreglo[], int bajo, int alto, unsigned long long *comparaciones)
{
    int pivote = arreglo[alto];
    int i = bajo - 1;

    for (int j = bajo; j < alto; j++)
    {
        if (comparaciones != NULL)
        {
            (*comparaciones)++;
        }
        if (arreglo[j] <= pivote)
        {
            i++;
            intercambiar(&arreglo[i], &arreglo[j]);
        }
    }

    intercambiar(&arreglo[i + 1], &arreglo[alto]);
    return i + 1;
}
// Aplica Quick Sort recursivamente sobre el subarreglo dividiendo en torno al pivote.
static void quick_sort_recursivo(int arreglo[], int bajo, int alto, unsigned long long *comparaciones)
{
    if (bajo < alto)
    {
        int posicion_pivote = particionar(arreglo, bajo, alto, comparaciones);
        quick_sort_recursivo(arreglo, bajo, posicion_pivote - 1, comparaciones);
        quick_sort_recursivo(arreglo, posicion_pivote + 1, alto, comparaciones);
    }
}

// Reinicia el contador de comparaciones y ordena el arreglo completo
// mediante Quick Sort cuando tiene más de un elemento.
void quick_sort(int arreglo[], int n, unsigned long long *comparaciones)
{
    if (comparaciones != NULL)
    {
        *comparaciones = 0;
    }

    if (n > 1)
    {
        quick_sort_recursivo(arreglo, 0, n - 1, comparaciones);
    }
}
