# actividad_final-estructura_de_datos

En este repositorio cargamos todo lo relacionado a la actividad final de la materia de Estructura de Datos del tercer semestre de la carrera de Ingeniería en Computación Inteligente.  En este proyecto colaboramos: Kevin Barajas, Kevin López y Arturo Cortés.

## Estructura

actividad-final-estructura-datos/
│
├── README.md
│
├── src/
│   ├── main.c
│   ├── busqueda.c
│   ├── ordenamiento.c
│   ├── experimentos.c
│   └── utilidades.c
│
├── include/
│   ├── busqueda.h
│   ├── ordenamiento.h
│   ├── experimentos.h
│   └── utilidades.h
│
├── resultados/
│   ├── busqueda.csv
│   ├── ordenamiento.csv
│   └── resumen_resultados.csv
│
├── pseudocodigos/
│   ├── busqueda.md
│   └── ordenamiento.md
│
└── .gitignore

### Explicación

#### include/

Utilizaremos archivos *.h* (headers / cabeceras) en la carpeta *include/* para una mejor modularidad.
Estos  archivos funcionan como un índice o menú de cada módulo que contiene únicamente las cabeceras de las funciones o las definiciones de alguna estructura si fuera necesario, pero **no** la lógica interna.
Con esto mantenemos más ordenado el proyecto y podemos agregarlos en el main de una forma más limpia.

> Nota: compilar requiere escribir ***gcc -Iinclude src/*.c -O0 -o benchmark** para incluir la conexión a dichos headers (-Iinclude src/*.c) y medir sin optimizaciones del compilador (-O0).

#### src/

En cada archivo *.c* tendremos la programación real de su respectivo *.h*.

- **utilidades.c**: Tendrá las funciones de apoyo general que cualquier algoritmo podría utilizar.
- **busqueda.c**: La implementación de *Búsqueda Lineal* y *Búsqueda Binaria*.
- **ordenamiento.c**: La implementación de *Insertion Sort* y *Quick Sort*.
- **experimentos.c**: El motor de pruebas en el que se configurarán los tamaños de pruebas y se correrán los cronómetros de alta precisión.
- **main.c**: El punto de entrada del programa que tendrá como objetivo ser corto y limpio.

### resultados/

Únicamente tenemos 2 csv + 1 de resumen para facilitar el trabajo de análisis y graficación que solicita el reporte:

- **busqueda.csv** y **ordenamiento.csv**: Los separamos así porque las escalas y naturaleza de ambos grupos son muy distintas, pero su propósito sigue siendo el mismo: guardar cada vez que se corra el programa individualmente para respaldar las repeticiones y los datos a tomar en cuenta.
- **resumen_resultados.csv**: Aquí estarán los datos listos para graficarm ya que la sección de resultados exige tablas con tiempo promedio y gráficas comparativas.

### pseudocódigos/

Nuevamente agrupados por categoría:
    - **busqueda.md** contiene la búsqueda *Linal* y la *Binaria*
    - **ordenamiento.md** contiene *Inserción* y *Quick Sort*
