// Incluye <stddef.h> para disponer de NULL y "busqueda.h" con las declaraciones de búsqueda.
#include <stddef.h>
#include "busqueda.h"

// Busca objetivo en un arreglo de enteros de solo lectura mediante búsqueda lineal de izquierda
// a derecha. Si comparaciones no es NULL, reinicia el contador a 0 e incrementa por cada
// comparación arreglo[i] == objetivo. Devuelve el índice encontrado o -1 si no existe.
int busqueda_lineal(const int arreglo[], int n, int objetivo, unsigned long long *comparaciones)
{
    if (comparaciones != NULL)
    {
        *comparaciones = 0;
    }

    for (int i = 0; i < n; i++)
    {
        if (comparaciones != NULL)
        {
            (*comparaciones)++;
        }
        if (arreglo[i] == objetivo)
        {
            return i;
        }
    }

    return -1;
}
// Busca objetivo en un arreglo de enteros ordenado de menor a mayor mediante búsqueda binaria,
// ajustando los límites según el punto medio. Si comparaciones no es NULL, reinicia el contador
// a 0 e incrementa por cada comparación de elementos. Devuelve el índice o -1 si no existe.
int busqueda_binaria(const int arreglo[], int n, int objetivo, unsigned long long *comparaciones)
{
    int izquierda = 0;
    int derecha = n - 1;

    if (comparaciones != NULL)
    {
        *comparaciones = 0;
    }

    while (izquierda <= derecha)
    {
        int medio = izquierda + (derecha - izquierda) / 2;

        if (comparaciones != NULL)
        {
            (*comparaciones)++;
        }
        if (arreglo[medio] == objetivo)
        {
            return medio;
        }

        if (comparaciones != NULL)
        {
            (*comparaciones)++;
        }
        if (arreglo[medio] < objetivo)
        {
            izquierda = medio + 1;
        }
        else
        {
            derecha = medio - 1;
        }
    }

    return -1;
}
