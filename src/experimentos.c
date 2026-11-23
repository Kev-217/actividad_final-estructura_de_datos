#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <direct.h>
#include <windows.h>
#include "busqueda.h"
#include "experimentos.h"
#include "ordenamiento.h"
#include "utilidades.h"
// Incluimos las bibliotecas necesarias para manejar errores, entrada/salida, memoria dinámica, crear directorios y medir el tiempo. También incluimos los archivos de encabezado que contienen las declaraciones de las funciones de búsqueda, experimentos, ordenamiento y utilidades.

// Definimos dos tipos de punteros a funciones: FuncionBusqueda para funciones de búsqueda y FuncionOrdenamiento para funciones de ordenamiento. Esto nos permite pasar diferentes algoritmos como argumentos a las funciones de experimentos.
typedef int (*FuncionBusqueda)(const int[], int, int, unsigned long long *);
typedef void (*FuncionOrdenamiento)(int[], int, unsigned long long *);

// Declaramos una variable global volátil para almacenar resultados de las pruebas y evitar que el compilador optimice su uso. Esto es útil para asegurar que las funciones de búsqueda y ordenamiento se ejecuten realmente durante los experimentos.
static volatile long long resultado_global = 0;

// Función para obtener el tiempo actual en segundos como un número de punto flotante. Utiliza diferentes métodos según el sistema operativo.
static double obtener_tiempo(void)
{
    // Usamos QueryPerformanceCounter para obtener un contador de alta resolución y QueryPerformanceFrequency para obtener la frecuencia del contador. Calculamos el tiempo en segundos dividiendo el valor del contador por la frecuencia.
    static LARGE_INTEGER frecuencia;
    LARGE_INTEGER contador;
    if (frecuencia.QuadPart == 0)
    {
        QueryPerformanceFrequency(&frecuencia);
    }
    QueryPerformanceCounter(&contador);
    return (double)contador.QuadPart / (double)frecuencia.QuadPart;
}

// Función para crear la carpeta "resultados" si no existe. Devuelve 1 si la carpeta se creó o ya existía, y 0 si hubo un error.
static int crear_carpeta_resultados(void)
{
    // Usamos _mkdir para crear la carpeta. Si la carpeta ya existe (errno == EEXIST), consideramos que la operación fue exitosa.
    if (_mkdir("resultados") == 0 || errno == EEXIST)
    {
        return 1;
    }
    return 0;
}

// Función para crear un archivo CSV único en la carpeta "resultados". Genera un nombre basado en la fecha y hora actual, y agrega un sufijo si el archivo ya existe. Devuelve 1 si el archivo se creó correctamente, y 0 si hubo un error.
static int crear_csv_unico(const char *base, char ruta[], size_t capacidad, FILE **archivo, const struct tm *fecha)
{
    char marca_tiempo[32];
    int sufijo = 0;

    strftime(marca_tiempo, sizeof(marca_tiempo), "%Y%m%d_%H%M%S", fecha);

    while (sufijo < 1000)
    {
        FILE *existente;

        if (sufijo == 0)
        {
            snprintf(ruta, capacidad, "resultados/%s_%s.csv", base, marca_tiempo);
        }
        else
        {
            snprintf(ruta, capacidad, "resultados/%s_%s_%02d.csv", base, marca_tiempo, sufijo);
        }

        existente = fopen(ruta, "r");
        if (existente != NULL)
        {
            fclose(existente);
            sufijo++;
            continue;
        }

        *archivo = fopen(ruta, "w");
        return *archivo != NULL;
    }

    return 0;
}

// Función para preparar los archivos CSV de resultados. Crea cuatro archivos: detalle y promedios para búsqueda y ordenamiento. Devuelve 1 si todos los archivos se crearon correctamente, y 0 si hubo un error.
static int preparar_archivos(FILE **busqueda_detalle, FILE **busqueda_promedios, FILE **ordenamiento_detalle, FILE **ordenamiento_promedios)
{
    time_t ahora = time(NULL);
    struct tm *fecha = localtime(&ahora);
    char ruta[160];

    if (fecha == NULL || !crear_carpeta_resultados())
    {
        printf("No fue posible preparar la carpeta resultados.\n");
        return 0;
    }

    if (!crear_csv_unico("busqueda_detalle", ruta, sizeof(ruta), busqueda_detalle, fecha))
    {
        return 0;
    }
    printf("CSV creado: %s\n", ruta);

    if (!crear_csv_unico("busqueda_promedios", ruta, sizeof(ruta), busqueda_promedios, fecha))
    {
        fclose(*busqueda_detalle);
        return 0;
    }
    printf("CSV creado: %s\n", ruta);

    if (!crear_csv_unico("ordenamiento_detalle", ruta, sizeof(ruta), ordenamiento_detalle, fecha))
    {
        fclose(*busqueda_detalle);
        fclose(*busqueda_promedios);
        return 0;
    }
    printf("CSV creado: %s\n", ruta);

    if (!crear_csv_unico("ordenamiento_promedios", ruta, sizeof(ruta), ordenamiento_promedios, fecha))
    {
        fclose(*busqueda_detalle);
        fclose(*busqueda_promedios);
        fclose(*ordenamiento_detalle);
        return 0;
    }
    printf("CSV creado: %s\n", ruta);

    fprintf(*busqueda_detalle, "algoritmo,n,escenario,repeticion,consultas,tiempo_total_ms,tiempo_por_consulta_ms,comparaciones_totales,comparaciones_por_consulta\n");
    fprintf(*busqueda_promedios, "algoritmo,n,escenario,repeticiones,consultas_por_repeticion,tiempo_promedio_ms,tiempo_promedio_por_consulta_ms,comparaciones_promedio_por_consulta\n");
    fprintf(*ordenamiento_detalle, "algoritmo,n,tipo_arreglo,repeticion,tiempo_ms,comparaciones,resultado_ordenado\n");
    fprintf(*ordenamiento_promedios, "algoritmo,n,tipo_arreglo,repeticiones,tiempo_promedio_ms,comparaciones_promedio,resultado_correcto\n");
    return 1;
}

// Función para ejecutar las pruebas de búsqueda. Realiza experimentos con diferentes tamaños de arreglos y escenarios de búsqueda, midiendo el tiempo y las comparaciones realizadas. Los resultados se escriben en los archivos CSV proporcionados.
static void ejecutar_pruebas_busqueda(FILE *detalle, FILE *promedios)
{
    const int tamanos[] = {1000, 5000, 10000, 25000, 50000, 100000};
    const char *escenarios[] = {"primera_posicion", "posicion_central", "ultima_posicion", "no_encontrado", "consultas_aleatorias"};
    const int cantidad_tamanos = (int)(sizeof(tamanos) / sizeof(tamanos[0]));
    const int cantidad_escenarios = (int)(sizeof(escenarios) / sizeof(escenarios[0]));
    const int repeticiones = 30;
    const FuncionBusqueda funciones[] = {busqueda_lineal, busqueda_binaria};
    const char *nombres[] = {"lineal", "binaria"};

    printf("\n--- Experimentos de busqueda ---\n");

    for (int t = 0; t < cantidad_tamanos; t++)
    {
        int n = tamanos[t];
        int *arreglo = malloc((size_t)n * sizeof(int));
        int consultas;
        int *claves;

        if (arreglo == NULL)
        {
            printf("No hay memoria suficiente para pruebas de busqueda.\n");
            return;
        }
        for (int i = 0; i < n; i++)
        {
            arreglo[i] = i;
        }

        /* Se preparan hasta 10,000 claves para cronometrar tambien la busqueda binaria. */
        consultas = 10000;
        claves = malloc((size_t)consultas * sizeof(int));
        if (claves == NULL)
        {
            free(arreglo);
            printf("No hay memoria suficiente para las consultas.\n");
            return;
        }

        for (int e = 0; e < cantidad_escenarios; e++)
        {
            if (e == 0)
            {
                for (int q = 0; q < consultas; q++)
                    claves[q] = arreglo[0];
            }
            else if (e == 1)
            {
                for (int q = 0; q < consultas; q++)
                    claves[q] = arreglo[(n - 1) / 2];
            }
            else if (e == 2)
            {
                for (int q = 0; q < consultas; q++)
                    claves[q] = arreglo[n - 1];
            }
            else if (e == 3)
            {
                for (int q = 0; q < consultas; q++)
                    claves[q] = n;
            }
            else
            {
                for (int q = 0; q < consultas; q++)
                    claves[q] = rand() % n;
            }

            for (int a = 0; a < 2; a++)
            {
                int consultas_prueba = consultas;
                double suma_tiempos = 0.0;
                double suma_comparaciones_por_consulta = 0.0;

                // Se hacen 10,000 consultas por prueba de busqueda binaria. En busqueda lineal se limita el trabajo en escenarios distintos al primero, porque recorre mas elementos por consulta.
                if (a == 0 && e != 0)
                {
                    consultas_prueba = 100000 / n;
                    if (consultas_prueba > 100)
                        consultas_prueba = 100;
                    if (consultas_prueba < 5)
                        consultas_prueba = 5;
                }

                for (int r = 1; r <= repeticiones; r++)
                {
                    unsigned long long comparaciones_totales = 0;
                    long long suma_indices = 0;
                    double inicio = obtener_tiempo();

                    for (int q = 0; q < consultas_prueba; q++)
                    {
                        int posicion = funciones[a](arreglo, n, claves[q], NULL);
                        suma_indices += posicion;
                    }

                    double fin = obtener_tiempo();
                    double tiempo_ms = (fin - inicio) * 1000.0;

                    /* El conteo se realiza en una pasada separada, fuera del cronometro. */
                    for (int q = 0; q < consultas_prueba; q++)
                    {
                        unsigned long long comparaciones = 0;
                        funciones[a](arreglo, n, claves[q], &comparaciones);
                        comparaciones_totales += comparaciones;
                    }

                    resultado_global += suma_indices;
                    suma_tiempos += tiempo_ms;
                    suma_comparaciones_por_consulta += (double)comparaciones_totales / (double)consultas_prueba;

                    fprintf(detalle, "%s,%d,%s,%d,%d,%.9f,%.12f,%llu,%.6f\n", nombres[a], n, escenarios[e], r, consultas_prueba, tiempo_ms, tiempo_ms / consultas_prueba, comparaciones_totales, (double)comparaciones_totales / consultas_prueba);
                }

                fprintf(promedios, "%s,%d,%s,%d,%d,%.9f,%.12f,%.6f\n", nombres[a], n, escenarios[e], repeticiones, consultas_prueba, suma_tiempos / repeticiones, suma_tiempos / (repeticiones * consultas_prueba), suma_comparaciones_por_consulta / repeticiones);
            }
        }

        free(claves);
        free(arreglo);
        printf("Busqueda: n = %d completado.\n", n);
    }
}

// Función para ejecutar las pruebas de ordenamiento. Realiza experimentos con diferentes tamaños de arreglos y tipos de datos, midiendo el tiempo y las comparaciones realizadas. Los resultados se escriben en los archivos CSV proporcionados.
static void ejecutar_pruebas_ordenamiento(FILE *detalle, FILE *promedios)
{
    const int tamanos[] = {10, 25, 50, 100, 250, 500, 1000, 2000};
    const int tipos[] = {1, 2, 3, 4};
    const char *nombres_tipos[] = {"aleatorio", "ordenado", "inverso", "casi_ordenado"};
    const FuncionOrdenamiento funciones[] = {ordenamiento_insercion, quick_sort};
    const char *nombres_algoritmos[] = {"insercion", "quick_sort"};
    const int repeticiones = 30;
    const int cantidad_tamanos = (int)(sizeof(tamanos) / sizeof(tamanos[0]));

    printf("\n--- Experimentos de ordenamiento ---\n");

    for (int t = 0; t < cantidad_tamanos; t++)
    {
        int n = tamanos[t];
        int *original = malloc((size_t)n * sizeof(int));
        int *trabajo = malloc((size_t)n * sizeof(int));

        if (original == NULL || trabajo == NULL)
        {
            printf("No hay memoria suficiente para pruebas de ordenamiento.\n");
            free(original);
            free(trabajo);
            return;
        }

        for (int tipo = 0; tipo < 4; tipo++)
        {
            generar_arreglo(original, n, tipos[tipo]);

            for (int a = 0; a < 2; a++)
            {
                double suma_tiempos = 0.0;
                double suma_comparaciones = 0.0;
                int todas_correctas = 1;

                for (int r = 1; r <= repeticiones; r++)
                {
                    unsigned long long comparaciones = 0;
                    int correcto;
                    double inicio;
                    double fin;
                    double tiempo_ms;

                    copiar_arreglo(original, trabajo, n);
                    inicio = obtener_tiempo();
                    funciones[a](trabajo, n, NULL);
                    fin = obtener_tiempo();
                    tiempo_ms = (fin - inicio) * 1000.0;
                    correcto = esta_ordenado(trabajo, n);

                    /* Se vuelve a ordenar una copia para contar comparaciones sin cronometrarlas. */
                    copiar_arreglo(original, trabajo, n);
                    funciones[a](trabajo, n, &comparaciones);

                    if (!correcto)
                    {
                        todas_correctas = 0;
                    }

                    suma_tiempos += tiempo_ms;
                    suma_comparaciones += (double)comparaciones;
                    fprintf(detalle, "%s,%d,%s,%d,%.9f,%llu,%s\n", nombres_algoritmos[a], n, nombres_tipos[tipo], r, tiempo_ms, comparaciones, correcto ? "si" : "no");
                }

                fprintf(promedios, "%s,%d,%s,%d,%.9f,%.3f,%s\n", nombres_algoritmos[a], n, nombres_tipos[tipo], repeticiones, suma_tiempos / repeticiones, suma_comparaciones / repeticiones, todas_correctas ? "si" : "no");
            }
        }

        free(original);
        free(trabajo);
        printf("Ordenamiento: n = %d completado.\n", n);
    }
}

// Función principal para ejecutar todos los experimentos de búsqueda y ordenamiento. Prepara los archivos CSV, ejecuta las pruebas y cierra los archivos al finalizar. Devuelve 1 si todo se ejecutó correctamente, y 0 si hubo algún error.
int ejecutar_experimentos(void)
{
    FILE *busqueda_detalle = NULL;
    FILE *busqueda_promedios = NULL;
    FILE *ordenamiento_detalle = NULL;
    FILE *ordenamiento_promedios = NULL;

    printf("\nSe ejecutaran 30 repeticiones por caso.\n");
    printf("Los experimentos utilizan datos propios y no modifican tu arreglo actual.\n");

    if (!preparar_archivos(&busqueda_detalle, &busqueda_promedios, &ordenamiento_detalle, &ordenamiento_promedios))
    {
        printf("No fue posible crear todos los archivos CSV.\n");
        return 0;
    }

    ejecutar_pruebas_busqueda(busqueda_detalle, busqueda_promedios);
    ejecutar_pruebas_ordenamiento(ordenamiento_detalle, ordenamiento_promedios);

    fclose(busqueda_detalle);
    fclose(busqueda_promedios);
    fclose(ordenamiento_detalle);
    fclose(ordenamiento_promedios);

    printf("\nExperimentos finalizados. Los CSV se guardaron en resultados/.\n");
    printf("Puedes abrirlos con Excel para elaborar las tablas y graficas.\n");
    return 1;
}
