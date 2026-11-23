# Pseudocodigos de ordenamiento

## Ordenamiento por insercion

```text
Procedimiento ordenamiento_insercion(arreglo, n)
    comparaciones <- 0

    Para i <- 1 hasta n - 1 hacer
        clave <- arreglo[i]
        j <- i - 1

        Mientras j >= 0 hacer
            comparaciones <- comparaciones + 1

            Si arreglo[j] > clave entonces
                arreglo[j + 1] <- arreglo[j]
                j <- j - 1
            SiNo
                Salir del Mientras
            FinSi
        FinMientras

        arreglo[j + 1] <- clave
    FinPara
FinProcedimiento
```

## Quick Sort

Esta implementacion utiliza la particion de Lomuto y el ultimo elemento de cada particion como pivote.

```text
Funcion particionar(arreglo, bajo, alto)
    pivote <- arreglo[alto]
    i <- bajo - 1

    Para j <- bajo hasta alto - 1 hacer
        comparaciones <- comparaciones + 1

        Si arreglo[j] <= pivote entonces
            i <- i + 1
            Intercambiar arreglo[i] con arreglo[j]
        FinSi
    FinPara

    Intercambiar arreglo[i + 1] con arreglo[alto]
    Devolver i + 1
FinFuncion

Procedimiento quick_sort(arreglo, bajo, alto)
    Si bajo < alto entonces
        posicion <- particionar(arreglo, bajo, alto)
        quick_sort(arreglo, bajo, posicion - 1)
        quick_sort(arreglo, posicion + 1, alto)
    FinSi
FinProcedimiento
```

**Nota:** el contador de comparaciones se comparte entre las llamadas recursivas y aumenta cada vez que se compara un elemento con el pivote.
