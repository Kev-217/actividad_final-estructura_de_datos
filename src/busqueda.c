// Incluimos la biblioteca estándar de C para poder utilizar el tipo de dato size_t, que es un entero sin signo utilizado para representar tamaños y conteos. También incluimos el archivo de encabezado busqueda.h, que contiene las declaraciones de las funciones de búsqueda lineal y binaria, así como la definición del tipo de dato unsigned long long para contar comparaciones.
#include <stddef.h>
#include "busqueda.h"

// Recibe como constante un arreglo de enteros, su tamaño n, un valor objetivo a buscar y un puntero a una variable de tipo unsigned long long para contar las comparaciones realizadas durante la búsqueda. La función realiza una búsqueda lineal recorriendo el arreglo de izquierda a derecha y devuelve el índice del elemento encontrado o -1 si no existe. Si el puntero comparaciones no es NULL, se incrementa en cada comparación realizada.
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

// Recibe como constante un arreglo de enteros, su tamaño n, un valor objetivo a buscar y un puntero a una variable de tipo unsigned long long para contar las comparaciones realizadas durante la búsqueda. La función realiza una búsqueda binaria dividiendo sucesivamente el arreglo a la mitad y devuelve el índice del elemento encontrado o -1 si no existe. Es importante que el arreglo esté ordenado de menor a mayor antes de llamar a esta función. Si el puntero comparaciones no es NULL, se incrementa en cada comparación realizada.
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
