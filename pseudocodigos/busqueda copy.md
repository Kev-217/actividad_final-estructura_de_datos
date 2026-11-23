# Pseudocodigos de busqueda

## Busqueda lineal

```text
Funcion busqueda_lineal(arreglo, n, objetivo)
    comparaciones <- 0

    Para i <- 0 hasta n - 1 hacer
        comparaciones <- comparaciones + 1

        Si arreglo[i] = objetivo entonces
            Devolver i y comparaciones
        FinSi
    FinPara

    Devolver -1 y comparaciones
FinFuncion
```

## Busqueda binaria

**Precondicion:** el arreglo debe estar ordenado de menor a mayor.

```text
Funcion busqueda_binaria(arreglo, n, objetivo)
    izquierda <- 0
    derecha <- n - 1
    comparaciones <- 0

    Mientras izquierda <= derecha hacer
        medio <- izquierda + (derecha - izquierda) / 2

        comparaciones <- comparaciones + 1
        Si arreglo[medio] = objetivo entonces
            Devolver medio y comparaciones
        FinSi

        comparaciones <- comparaciones + 1
        Si arreglo[medio] < objetivo entonces
            izquierda <- medio + 1
        SiNo
            derecha <- medio - 1
        FinSi
    FinMientras

    Devolver -1 y comparaciones
FinFuncion
```
