#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "busqueda.h"
#include "experimentos.h"
#include "ordenamiento.h"
#include "utilidades.h"
// Incluimos las bibliotecas necesarias para manejar límites de enteros, entrada/salida, memoria dinámica y tiempo. También incluimos los archivos de encabezado que contienen las declaraciones de las funciones de búsqueda, experimentos, ordenamiento y utilidades.

// Definimos un valor máximo para la cantidad de elementos en el arreglo. Esto nos permite limitar la memoria utilizada y evitar problemas de rendimiento o errores de desbordamiento.
#define MAX_ELEMENTOS 5000

// Función para solicitar un número entero dentro de un rango específico. Muestra un mensaje al usuario y valida la entrada. Devuelve 1 si la lectura fue correcta y 0 si el usuario desea salir.
static int solicitar_entero_rango(const char *mensaje, int minimo, int maximo, int *valor)
{
    int resultado;

    do
    {
        resultado = leer_entero(mensaje, valor);
        if (resultado == -1)
        {
            return 0;
        }
        if (resultado == 0 || *valor < minimo || *valor > maximo)
        {
            printf("Entrada invalida. Introduce un valor entre %d y %d.\n", minimo, maximo);
        }
    } while (resultado == 0 || *valor < minimo || *valor > maximo);

    return 1;
}

// Función para crear un arreglo inicial. Solicita al usuario la cantidad de elementos y el método de creación (manual, pegado o automático). Devuelve 1 si el arreglo se creó correctamente y 0 si hubo algún error o el usuario decidió salir.
static int crear_arreglo_inicial(int **arreglo_actual, int *cantidad_actual)
{
    int cantidad;
    int metodo;
    int *nuevo_arreglo;

    printf("\n====== CREAR ARREGLO ======\n");
    if (!solicitar_entero_rango("Cantidad de elementos (1-5000): ", 1, MAX_ELEMENTOS, &cantidad))
    {
        return 0;
    }

    nuevo_arreglo = malloc((size_t)cantidad * sizeof(int));
    if (nuevo_arreglo == NULL)
    {
        printf("No fue posible reservar memoria.\n");
        return *arreglo_actual != NULL;
    }

    printf("\nComo deseas crear el arreglo?\n");
    printf("1. Introducir los elementos uno por uno\n");
    printf("2. Pegar todos los elementos en una sola linea\n");
    printf("3. Generar el arreglo automaticamente\n");

    if (!solicitar_entero_rango("Selecciona una opcion: ", 1, 3, &metodo))
    {
        free(nuevo_arreglo);
        return 0;
    }

    if (metodo == 1)
    {
        printf("\nIntroduce los elementos. Si te equivocas, podras intentar de nuevo ese elemento.\n");
        if (leer_arreglo_manual(nuevo_arreglo, cantidad) == -1)
        {
            free(nuevo_arreglo);
            return 0;
        }
    }
    else if (metodo == 2)
    {
        int resultado;
        do
        {
            resultado = leer_arreglo_pegado(nuevo_arreglo, cantidad);
            if (resultado == -1)
            {
                free(nuevo_arreglo);
                return 0;
            }
        } while (resultado == 0);
    }
    else
    {
        int tipo;
        printf("\nTipo de arreglo:\n");
        printf("1. Aleatorio\n");
        printf("2. Ordenado\n");
        printf("3. Inverso\n");
        printf("4. Casi ordenado\n");

        if (!solicitar_entero_rango("Selecciona el tipo: ", 1, 4, &tipo))
        {
            free(nuevo_arreglo);
            return 0;
        }

        generar_arreglo(nuevo_arreglo, cantidad, tipo);
        printf("Arreglo generado correctamente.\n");
    }

    free(*arreglo_actual);
    *arreglo_actual = nuevo_arreglo;
    *cantidad_actual = cantidad;

    printf("\nArreglo listo: %d elementos.\n", cantidad);
    printf("Vista del arreglo:\n");
    imprimir_arreglo(*arreglo_actual, *cantidad_actual);
    return 1;
}

// Función principal del programa. Muestra un menú al usuario para realizar búsquedas, ordenamientos, ejecutar experimentos o crear un nuevo arreglo. Gestiona la memoria y las entradas del usuario.
int main(void)
{
    int *arreglo = NULL;
    int *copia = NULL;
    int cantidad = 0;
    int opcion;

    srand((unsigned int)time(NULL));

    printf("=============================================\n");
    printf("   ALGORITMOS DE BUSQUEDA Y ORDENAMIENTO\n");
    printf("=============================================\n");

    if (!crear_arreglo_inicial(&arreglo, &cantidad))
    {
        free(arreglo);
        printf("\nPrograma finalizado.\n");
        return 0;
    }

    copia = malloc((size_t)cantidad * sizeof(int));
    if (copia == NULL)
    {
        printf("No fue posible reservar memoria.\n");
        free(arreglo);
        return 1;
    }

    system("pause");
    do
    {
        unsigned long long comparaciones = 0;

        printf("\n============== MENU PRINCIPAL ==============\n");
        printf("1. Busqueda lineal\n");
        printf("2. Busqueda binaria\n");
        printf("3. Ordenamiento por insercion\n");
        printf("4. Quick Sort\n");
        printf("5. Ejecutar experimentos y generar CSV\n");
        printf("6. Crear o cambiar el arreglo\n");
        printf("0. Salir\n");

        if (!solicitar_entero_rango("Selecciona una opcion: ", 0, 6, &opcion))
        {
            break;
        }

        if (opcion == 1 || opcion == 2)
        {
            int objetivo;
            int posicion;
            double inicio;
            double fin;
            double tiempo_ms;

            if (opcion == 2 && !esta_ordenado(arreglo, cantidad))
            {
                printf("La busqueda binaria necesita un arreglo ordenado de menor a mayor.\n");
                continue;
            }

            if (!solicitar_entero_rango("Valor que deseas buscar: ", INT_MIN, INT_MAX, &objetivo))
            {
                break;
            }

            /* La medicion se hace sin contador para no sumar su sobrecarga. */
            inicio = tiempo_actual_segundos();
            if (opcion == 1)
            {
                posicion = busqueda_lineal(arreglo, cantidad, objetivo, NULL);
            }
            else
            {
                posicion = busqueda_binaria(arreglo, cantidad, objetivo, NULL);
            }
            fin = tiempo_actual_segundos();
            tiempo_ms = (fin - inicio) * 1000.0;

            /* Se repite la busqueda para contar comparaciones fuera del cronometro. */
            if (opcion == 1)
            {
                busqueda_lineal(arreglo, cantidad, objetivo, &comparaciones);
            }
            else
            {
                busqueda_binaria(arreglo, cantidad, objetivo, &comparaciones);
            }

            if (posicion >= 0)
            {
                printf("Elemento encontrado en el indice %d.\n", posicion);
            }
            else
            {
                printf("El elemento no se encontro.\n");
            }
            printf("Tiempo de ejecucion: %.9f ms (%.3f us)\n",
                   tiempo_ms, tiempo_ms * 1000.0);
            printf("Comparaciones: %llu\n", comparaciones);
            printf("Nota: tiempo orientativo de una ejecucion; para comparar rendimiento usa los experimentos.\n");
        }
        else if (opcion == 3 || opcion == 4)
        {
            double inicio;
            double fin;
            double tiempo_ms;
            copiar_arreglo(arreglo, copia, cantidad);

            /* Se cronometra el algoritmo sin el contador de comparaciones. */
            inicio = tiempo_actual_segundos();
            if (opcion == 3)
            {
                ordenamiento_insercion(copia, cantidad, NULL);
            }
            else
            {
                quick_sort(copia, cantidad, NULL);
            }
            fin = tiempo_actual_segundos();
            tiempo_ms = (fin - inicio) * 1000.0;

            /* Se vuelve a ordenar una copia para contar sin afectar el tiempo. */
            copiar_arreglo(arreglo, copia, cantidad);
            if (opcion == 3)
            {
                ordenamiento_insercion(copia, cantidad, &comparaciones);
                printf("\nResultado de ordenamiento por insercion:\n");
            }
            else
            {
                quick_sort(copia, cantidad, &comparaciones);
                printf("\nResultado de Quick Sort:\n");
            }

            imprimir_arreglo(copia, cantidad);
            printf("Tiempo de ejecucion: %.9f ms (%.3f us)\n",
                   tiempo_ms, tiempo_ms * 1000.0);
            printf("Comparaciones: %llu\n", comparaciones);
            printf("El arreglo original se conserva sin cambios.\n");
            printf("Nota: tiempo orientativo de una ejecucion; para comparar rendimiento usa los experimentos.\n");
        }
        else if (opcion == 5)
        {
            ejecutar_experimentos();
        }
        else if (opcion == 6)
        {
            if (!crear_arreglo_inicial(&arreglo, &cantidad))
            {
                break;
            }

            int *nueva_copia = realloc(copia, (size_t)cantidad * sizeof(int));
            if (nueva_copia == NULL)
            {
                printf("No fue posible ajustar la memoria del arreglo de trabajo.\n");
                break;
            }
            copia = nueva_copia;
        }

        // Pausa para que el usuario pueda ver los resultados antes de continuar.
        system("pause");
    } while (opcion != 0);

    free(arreglo);
    free(copia);
    printf("\nPrograma finalizado.\n");
    return 0;
}
