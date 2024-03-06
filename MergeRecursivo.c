#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Función de mezcla para Merge Sort
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Crear arrays temporales
    int L[n1], R[n2];

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
}

// Función de Merge Sort
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        // Encuentra el punto medio del array
        int m = l + (r - l) / 2;

        // Ordena la primera mitad y la segunda mitad
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // Mezcla las mitades ordenadas
        merge(arr, l, m, r);
    }
}

int main() {
	srand(time(NULL));
	
	int N;
	printf("Numero N: ");
	scanf("%d", &N);
	
	int numeros[N];
		
	for( int i = 0; i < N; ++i) {
		numeros[i] = rand() % 2001 - 1000;
	}
	
	printf("Vector original:\n");
	for (int i = 0; i < N; ++i) {
		printf("%d\n", numeros[i]);
	}
	
	int numerosClonados[N];
	for (int i = 0; i < N; ++i) {
		numerosClonados[i] = numeros[i];
	}
	
	// Medir el tiempo antes de ordenar el vector clonado
	clock_t inicio = clock();

	// Ordenar el vector clonado usando Merge Sort
	mergeSort(numerosClonados, 0, N - 1);

	// Medir el tiempo después de ordenar el vector clonado
	clock_t fin = clock();
	double tiempoEjecucion = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
	
	printf("\nVector clonado:\n");
	for (int i = 0; i < N; ++i) {
        	printf("%d\n", numerosClonados[i]);
    	}
	
	printf("\nTiempo de ejecucion para ordenar el vector clonado: %.6f segundos\n", tiempoEjecucion);
	return 0;
}
