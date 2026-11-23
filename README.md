# actividad_final-estructura_de_datos

En este repositorio cargamos todo lo relacionado a la actividad final de la materia de Estructura de Datos del tercer semestre de la carrera de Ingeniería en Computación Inteligente.  

En este proyecto colaboramos:
    - Kevin Osvaldo Barajas Anaya
    - Kevin Guadalupe López Vega
    - Arturo Cortés García

## Descripcion

Programa en lenguaje C que implementa y permite probar dos algoritmos de busqueda y dos de ordenamiento:

- Busqueda lineal.
- Busqueda binaria.
- Ordenamiento por insercion.
- Quick Sort.

El programa muestra el resultado de cada operacion y cuenta las comparaciones entre los valores que participan en cada algoritmo.

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

## Especificaciones de el dispositivo

- **Fabricante**: Lenovo
- **Modelo**: ThinkBook 14
- **Procesador**: AMD Ryzen 5 7533HS with Radeon Graphics
- **Memoria RAM**: 14 gb
- **Sistema Operativo**: Microsoft Windows 11 Home Single Language 10.0.26300
- **Memoria Caché**:
    Level InstalledSize MaxCacheSize
    ----- ------------- ------------
        3           384          384
        4          3072         3072
        5         16384        16384
- **Compilador**: gcc.exe (Rev8, Built by MSYS2 project) 15.2.0
- **Banderas de compilación**: -std=c11 -O2 -Wall -Wextra -pedantic

> [!Note] Para obtener los datos se usaron los siguientes comandos en Power Shell
    - **Procesador**: Get-CimInstance Win32_Processor | Select-Object Name
    - **Memoria RAM**: Get-CimInstance Win32_ComputerSystem | Select-Object @{Name="RAM_GB";Expression={[math]::Round($_.TotalPhysicalMemory/1GB,2)}}
    - **Sistema Operativo**: Get-CimInstance Win32_OperatingSystem | Select-Object Caption, Version
    - **Memoria Caché**: Get-CimInstance Win32_CacheMemory | Select-Object Level, InstalledSize, MaxCacheSize
    - **Compilador**: gcc --version

## Compilacion

Abrir una terminal en la carpeta principal del proyecto y ejecutar:

```bash
gcc -std=c11 -O2 -Wall -Wextra -pedantic -Iinclude src/main.c src/busqueda.c src/ordenamiento.c src/utilidades.c src/experimentos.c -o actividad.exe
```

> -O2 activa optimizaciones del compilador y las demás opciones ayudan a mantener el código bajo advertencias y comprobaciones del estándar.

## Ejecucion

En PowerShell:

```powershell
.\actividad.exe
```

## Uso del programa

Al iniciar, se solicita la cantidad de elementos y se permite elegir:

1. Introducirlos uno por uno.
2. Pegarlos en una sola linea separados por espacios. El programa valida que la cantidad sea exacta y permite intentarlo de nuevo sin cerrar el programa.
3. Generarlos automaticamente: aleatorios, ordenados, inversos o casi ordenados.

El menu principal permite ejecutar los algoritmos, cambiar el arreglo o iniciar la bateria completa de experimentos. El arreglo original no se modifica cuando se prueba un algoritmo de ordenamiento.

Ejemplo de entrada pegada para un arreglo de cinco elementos:

```text
Cantidad de elementos: 5
Opcion: 2
Pega los 5 enteros en una sola linea:
> 18 4 -2 35 9
```

Si la cantidad no coincide, el programa indica cuantos elementos faltan o sobran y solicita la linea nuevamente. Se aceptan espacios o tabulaciones entre los enteros.

La busqueda binaria solo se ejecuta si el arreglo actual esta ordenado de menor a mayor.

## Experimentos y archivos CSV

La opcion **5. Ejecutar experimentos y generar CSV** crea automaticamente la carpeta `resultados/` y cuatro archivos:

- `busqueda_detalle_...csv`: datos de cada repeticion de busqueda.
- `busqueda_promedios_...csv`: tiempos y comparaciones promedio de busqueda.
- `ordenamiento_detalle_...csv`: datos de cada repeticion de ordenamiento.
- `ordenamiento_promedios_...csv`: tiempos y comparaciones promedio de ordenamiento.

Cada ejecucion utiliza una marca de fecha y hora en los nombres. Si ya existe un archivo con esa marca, se agrega un sufijo numerico. De esta forma, las ejecuciones nuevas no reemplazan los resultados anteriores.

### Configuracion inicial de los experimentos

- Repeticiones: 30 por combinacion de algoritmo, tamano y escenario.
- Busqueda: n = 1,000; 5,000; 10,000; 25,000; 50,000; 100,000.
- Escenarios de busqueda: primera posicion, posicion central, ultima posicion, elemento no encontrado y consultas aleatorias exitosas.
- Ordenamiento: n = 10; 25; 50; 100; 250; 500; 1,000; 2,000. Los tamaños pequeños ayudan a observar el umbral donde Quick Sort empieza a superar a Insercion en datos aleatorios.
- Tipos de arreglo para ordenamiento: aleatorio, ordenado, inverso y casi ordenado. Los arreglos generados para ordenamiento contienen valores unicos de 0 a n-1; el tipo aleatorio mezcla esos valores y el casi ordenado comienza ordenado y realiza algunos intercambios aleatorios.
- Quick Sort utiliza Lomuto y el ultimo elemento como pivote. Los arreglos ordenados e inversos permiten observar particiones muy desequilibradas.

En busqueda se ejecutan lotes de consultas para que el tiempo sea mas estable. La busqueda binaria utiliza 10,000 consultas por repeticion. La busqueda lineal utiliza 10,000 consultas en el escenario de primera posicion y una cantidad ajustada al tamano del arreglo en los demas escenarios para evitar un numero excesivo de recorridos completos. El CSV informa el numero real de consultas de cada prueba, el tiempo total, el tiempo por consulta y las comparaciones por consulta. Los promedios se normalizan por el numero de consultas, aunque el volumen de trabajo de ambas funciones no sea igual.

En ordenamiento, la copia del arreglo se realiza antes de iniciar el cronometro. La verificacion de que el arreglo quedo ordenado se realiza despues de detenerlo. El conteo de comparaciones se obtiene en una ejecucion independiente, para que los incrementos del contador no formen parte del tiempo medido. En la medicion cronometrada, el contador esta desactivado.

Los tiempos se miden con `QueryPerformanceCounter` en Windows. Los contadores registran comparaciones entre valores segun las definiciones de cada algoritmo.
