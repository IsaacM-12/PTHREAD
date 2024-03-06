#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NUM_THREADS 4

// Estructura para pasar argumentos a la función de ordenamiento de hilos
struct ThreadArgs {
    int *arr;
    int l;
    int r;
};

// Función de mezcla para Merge Sort
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Crear arrays temporales con asignación dinámica de memoria
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    // Copiar datos a los arrays temporales L[] y R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Mezclar los arrays temporales de vuelta en arr[l..r]
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copiar los elementos restantes de L[], si los hay
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copiar los elementos restantes de R[], si los hay
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    // Liberar memoria de los arrays temporales
    free(L);
    free(R);
}

// Función de Merge Sort iterativo
void *mergeSortIterative(void *args) {
    struct ThreadArgs *tArgs = (struct ThreadArgs *)args;
    int *arr = tArgs->arr;
    int l = tArgs->l;
    int r = tArgs->r;

    for (int curr_size = 1; curr_size <= r - l; curr_size = 2 * curr_size) {
        for (int left_start = l; left_start < r; left_start += 2 * curr_size) {
            int mid = left_start + curr_size - 1;
            int right_end = (left_start + 2 * curr_size - 1 < r) ? (left_start + 2 * curr_size - 1) : r;
            merge(arr, left_start, mid, right_end);
        }
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    int N;
    printf("Numero N: ");
    scanf("%d", &N);

    int *numeros = (int *)malloc(N * sizeof(int));

    for (int i = 0; i < N; ++i) {
        numeros[i] = rand() % 2001 - 1000;
    }
    printf("Vector original:\n");
    for (int i = 0; i < N; ++i) {
        printf("%d\n", numeros[i]);
    }

    int *numerosClonados = (int *)malloc(N * sizeof(int));
    for (int i = 0; i < N; ++i) {
        numerosClonados[i] = numeros[i];
    }

    // Medir el tiempo antes de ordenar el vector clonado
    clock_t inicio = clock();

    // Crear hilos para el ordenamiento paralelo
    pthread_t threads[NUM_THREADS];
    struct ThreadArgs tArgs[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
        tArgs[i].arr = numerosClonados;
        tArgs[i].l = i * (N / NUM_THREADS);
        tArgs[i].r = (i + 1) * (N / NUM_THREADS) - 1;

        pthread_create(&threads[i], NULL, mergeSortIterative, (void *)&tArgs[i]);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Mezclar los resultados de los hilos de manera iterativa
    for (int i = 1; i < NUM_THREADS; i++) {
        merge(numerosClonados, 0, i * (N / NUM_THREADS) - 1, (i + 1) * (N / NUM_THREADS) - 1);
    }

    // Medir el tiempo después de ordenar el vector clonado
    clock_t fin = clock();
    double tiempoEjecucion = ((double)(fin - inicio)) / CLOCKS_PER_SEC;

    printf("\nVector clonado:\n");
    for (int i = 0; i < N; ++i) {
        printf("%d\n", numerosClonados[i]);
    }

    // Liberar memoria
    free(numeros);
    free(numerosClonados);

    printf("\nTiempo de ejecucion para ordenar el vector clonado: %.6f segundos\n", tiempoEjecucion);
    return 0;
}

