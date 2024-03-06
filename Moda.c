#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

// Estructura para pasar argumentos a la función de conteo de moda de hilos
struct ThreadArgs {
    int *arr;
    int l;
    int r;
    int *frequencies; // Array para almacenar las frecuencias de cada elemento
};

// Función de conteo de frecuencias para la moda
void *countFrequencies(void *args) {
    struct ThreadArgs *tArgs = (struct ThreadArgs *)args;
    int *arr = tArgs->arr;
    int l = tArgs->l;
    int r = tArgs->r;
    int *frequencies = tArgs->frequencies;

    for (int i = l; i <= r; ++i) {
        frequencies[arr[i] + 1000]++;
    }

    pthread_exit(NULL);
}

// Función para encontrar la moda de las frecuencias
int findMode(int *frequencies, int size) {
    int maxFreq = 0;
    int mode = -1;

    for (int i = 0; i < size; ++i) {
        if (frequencies[i] > maxFreq) {
            maxFreq = frequencies[i];
            mode = i - 1000;
        }
    }

    return mode;
}

int main() {
    int N;
    printf("Numero N: ");
    scanf("%d", &N);

    int *numeros = (int *)malloc(N * sizeof(int));

    printf("Numeros generados:\n");
    for (int i = 0; i < N; ++i) {
        numeros[i] = rand() % 2001 - 1000;
        printf("%d ", numeros[i]);
    }

    int maxNumber = 10; // Suponemos que los números están en el rango [0, maxNumber)
    int frequencies[2001] = {0};

    // Crear hilos para el conteo paralelo de frecuencias
    pthread_t threads[NUM_THREADS];
    struct ThreadArgs tArgs[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
        tArgs[i].arr = numeros;
        tArgs[i].l = i * (N / NUM_THREADS);
        tArgs[i].r = (i + 1) * (N / NUM_THREADS) - 1;
        tArgs[i].frequencies = frequencies;

        pthread_create(&threads[i], NULL, countFrequencies, (void *)&tArgs[i]);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Calcular la moda a partir de las frecuencias
    int mode = findMode(frequencies, 2001);

    printf("\nLa moda del vector es: %d\n", mode);

    // Liberar memoria
    free(numeros);

    return 0;
}

