#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilidades.h"
// Añadimos las librerías ctype.h, errno.h, limits.h, stdio.h, stdlib.h y string.h para poder usar funciones de manejo de caracteres, manejo de errores, límites de enteros, entrada/salida estándar, funciones de utilidad general y manipulación de cadenas. También incluimos la cabecera "utilidades.h" que contiene las declaraciones de las funciones implementadas en este archivo.

// Implementación de la función leer_entero. Esta función lee un número entero desde la entrada estándar, mostrando un mensaje al usuario. Recibe un mensaje y un puntero a un entero donde almacenar el valor leído. Devuelve 1 si la lectura fue correcta, 0 si hubo un error de formato y -1 si ocurrió un error de entrada/salida.
int leer_entero(const char *mensaje, int *valor)
{
    char linea[128];
    char *fin;
    long numero;

    // Mostramos el mensaje al usuario y leemos una línea de entrada. Si ocurre un error de entrada/salida, devolvemos -1.
    printf("%s", mensaje);
    if (fgets(linea, sizeof(linea), stdin) == NULL)
    {
        return -1;
    }

    // Comprobamos si la línea leída es demasiado larga. Si no contiene un salto de línea y no hemos llegado al final del archivo, descartamos el resto de la línea y devolvemos 0 indicando un error de formato.
    if (strchr(linea, '\n') == NULL && !feof(stdin))
    {
        int caracter;
        while ((caracter = getchar()) != '\n' && caracter != EOF)
        {
            /* Descarta el resto de una linea demasiado larga. */
        }
        return 0;
    }

    // Intentamos convertir la línea leída a un número entero. Comprobamos si la conversión fue exitosa y si el número está dentro del rango de un entero. Si no, devolvemos 0 indicando un error de formato.
    errno = 0;
    numero = strtol(linea, &fin, 10);
    if (fin == linea || errno == ERANGE || numero < INT_MIN || numero > INT_MAX)
    {
        return 0;
    }

    // Comprobamos si hay caracteres no numéricos después del número leído. Si los hay, devolvemos 0 indicando un error de formato.
    while (isspace((unsigned char)*fin))
    {
        fin++;
    }

    if (*fin != '\0')
    {
        return 0;
    }

    // Si todo es correcto, almacenamos el valor leído en la variable apuntada por valor y devolvemos 1 indicando éxito.
    *valor = (int)numero;
    return 1;
}

// Implementación de la función leer_arreglo_manual. Esta función permite al usuario ingresar manualmente los elementos de un arreglo de enteros. Recibe un arreglo y su tamaño, y devuelve 1 si la lectura fue correcta, 0 si hubo un error de formato y -1 si ocurrió un error de entrada/salida.
int leer_arreglo_manual(int arreglo[], int n)
{
    // Iteramos sobre cada posición del arreglo, solicitando al usuario que ingrese un número entero. Si ocurre un error de entrada/salida, devolvemos -1. Si el usuario ingresa un valor no válido, mostramos un mensaje de error y le pedimos que intente nuevamente.
    for (int i = 0; i < n; i++)
    {
        char mensaje[80];
        int resultado;

        do
        {
            // Formateamos el mensaje para indicar al usuario qué elemento del arreglo debe ingresar.
            snprintf(mensaje, sizeof(mensaje), "Elemento %d de %d: ", i + 1, n);
            resultado = leer_entero(mensaje, &arreglo[i]);

            if (resultado == -1)
            {
                return -1;
            }
            if (resultado == 0)
            {
                printf("Entrada invalida. Introduce un numero entero.\n");
            }
        } while (resultado == 0);
    }

    return 1;
}

// Implementación de la función leer_arreglo_pegado. Esta función permite al usuario ingresar todos los elementos de un arreglo de enteros en una sola línea, separados por espacios. Recibe un arreglo y su tamaño, y devuelve 1 si la lectura fue correcta, 0 si hubo un error de formato y -1 si ocurrió un error de entrada/salida.
int leer_arreglo_pegado(int arreglo[], int n)
{
    // Calculamos la capacidad necesaria para almacenar la línea de entrada, considerando un tamaño máximo de 13 caracteres por número (incluyendo signo y espacio) y un carácter adicional para el salto de línea. Reservamos memoria para la línea y un arreglo temporal para almacenar los números leídos.
    size_t capacidad = (size_t)n * 13U + 4U;
    char *linea = malloc(capacidad);
    int *temporal = malloc((size_t)n * sizeof(int));
    char *actual;
    int cantidad = 0;
    int invalido = 0;

    // Comprobamos si la memoria fue asignada correctamente. Si no, mostramos un mensaje de error y liberamos la memoria asignada antes de devolver 0 indicando un error.
    if (linea == NULL || temporal == NULL)
    {
        printf("No hay memoria suficiente para leer los datos.\n");
        free(linea);
        free(temporal);
        return 0;
    }

    // Solicitamos al usuario que ingrese todos los elementos del arreglo en una sola línea, separados por espacios. Leemos la línea de entrada y comprobamos si ocurrió un error de entrada/salida. Si ocurre, liberamos la memoria y devolvemos -1.
    printf("Pega los %d enteros en una sola linea, separados por espacios:\n> ", n);
    if (fgets(linea, (int)capacidad, stdin) == NULL)
    {
        free(linea);
        free(temporal);
        return -1;
    }

    // Comprobamos si la línea leída es demasiado larga. Si no contiene un salto de línea y no hemos llegado al final del archivo, descartamos el resto de la línea, mostramos un mensaje de error y liberamos la memoria antes de devolver 0 indicando un error.
    if (strchr(linea, '\n') == NULL && !feof(stdin))
    {
        int caracter;
        while ((caracter = getchar()) != '\n' && caracter != EOF)
        {
            /* Descarta el resto de la linea. */
        }
        printf("La linea es demasiado larga. Intenta pegar los datos nuevamente.\n");
        free(linea);
        free(temporal);
        return 0;
    }

    // Procesamos la línea leída, separando los números por espacios y convirtiéndolos a enteros. Comprobamos si cada número es válido y si la cantidad de números ingresados coincide con el tamaño del arreglo. Si hay algún error, mostramos un mensaje de error y liberamos la memoria antes de devolver 0 indicando un error.
    actual = linea;
    while (*actual != '\0')
    {
        char *fin;
        long numero;

        while (isspace((unsigned char)*actual))
        {
            actual++;
        }
        if (*actual == '\0')
        {
            break;
        }

        errno = 0;
        numero = strtol(actual, &fin, 10);
        if (fin == actual || errno == ERANGE || numero < INT_MIN || numero > INT_MAX)
        {
            invalido = 1;
            break;
        }

        cantidad++;
        if (cantidad <= n)
        {
            temporal[cantidad - 1] = (int)numero;
        }
        actual = fin;

        if (*actual != '\0' && !isspace((unsigned char)*actual))
        {
            invalido = 1;
            break;
        }
    }

    // Comprobamos si hubo algún error durante la lectura de los números. Si hubo un dato inválido o si la cantidad de números ingresados no coincide con el tamaño del arreglo, mostramos un mensaje de error y liberamos la memoria antes de devolver 0 indicando un error.
    if (invalido)
    {
        printf("Se encontro un dato que no es un numero entero valido. Intenta nuevamente.\n");
    }
    else if (cantidad < n)
    {
        printf("Ingresaste %d elementos; faltan %d. Intenta nuevamente.\n", cantidad, n - cantidad);
    }
    else if (cantidad > n)
    {
        printf("Ingresaste %d elementos; sobran %d. Intenta nuevamente.\n", cantidad, cantidad - n);
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            arreglo[i] = temporal[i];
        }
        free(linea);
        free(temporal);
        return 1;
    }

    free(linea);
    free(temporal);
    return 0;
}

// Implementación de la función generar_arreglo. Esta función genera un arreglo de enteros con valores del 0 al n-1 y lo modifica según el tipo especificado. Recibe un arreglo, su tamaño y un tipo que indica cómo se debe generar el arreglo: 1 para aleatorio, 2 para ordenado, 3 para inversamente ordenado y 4 para casi ordenado.
void generar_arreglo(int arreglo[], int n, int tipo)
{
    for (int i = 0; i < n; i++)
    {
        arreglo[i] = i;
    }

    if (tipo == 1)
    {
        /* Aleatorio: mezcla los valores sin repetirlos. */
        for (int i = n - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            int temporal = arreglo[i];
            arreglo[i] = arreglo[j];
            arreglo[j] = temporal;
        }
    }
    else if (tipo == 3)
    {
        for (int i = 0; i < n / 2; i++)
        {
            int temporal = arreglo[i];
            arreglo[i] = arreglo[n - 1 - i];
            arreglo[n - 1 - i] = temporal;
        }
    }
    else if (tipo == 4 && n > 1)
    {
        int intercambios = n / 100;
        if (intercambios < 1)
        {
            intercambios = 1;
        }

        for (int i = 0; i < intercambios; i++)
        {
            int a = rand() % n;
            int b = rand() % n;
            int temporal = arreglo[a];
            arreglo[a] = arreglo[b];
            arreglo[b] = temporal;
        }
    }

    // tipo 2 permanece ordenado.
}

// Implementación de la función imprimir_arreglo. Esta función muestra los elementos de un arreglo de enteros en la salida estándar. Si el tamaño del arreglo es mayor a 40, muestra los primeros 20 y los últimos 20 elementos, indicando cuántos elementos hay en total.
void imprimir_arreglo(const int arreglo[], int n)
{
    const int limite = 20;

    if (n <= limite * 2)
    {
        for (int i = 0; i < n; i++)
        {
            printf("%d", arreglo[i]);
            printf(i == n - 1 ? "\n" : " ");
        }
        return;
    }

    for (int i = 0; i < limite; i++)
    {
        printf("%d ", arreglo[i]);
    }
    printf("... (%d elementos) ... ", n);
    for (int i = n - limite; i < n; i++)
    {
        printf("%d", arreglo[i]);
        printf(i == n - 1 ? "\n" : " ");
    }
}

// Implementación de la función copiar_arreglo. Esta función copia los elementos de un arreglo de enteros en otro arreglo. Recibe un arreglo de origen, un arreglo de destino y el tamaño del arreglo.
void copiar_arreglo(const int origen[], int destino[], int n)
{
    for (int i = 0; i < n; i++)
    {
        destino[i] = origen[i];
    }
}

// Implementación de la función esta_ordenado. Esta función verifica si un arreglo de enteros está ordenado en orden ascendente. Recibe un arreglo y su tamaño, y devuelve 1 si está ordenado o 0 en caso contrario.
int esta_ordenado(const int arreglo[], int n)
{
    for (int i = 1; i < n; i++)
    {
        if (arreglo[i - 1] > arreglo[i])
        {
            return 0;
        }
    }
    return 1;
}
