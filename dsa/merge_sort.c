/* Algoritmo de Ordenação - MERGE SORT

A ideia central do Merge Sort é:

-> Dividir o array repetidamente em duas metades até que cada parte tenha 1 elemento.
-> Mesclar (merge) duas metades ordenadas em uma sequência ordenada maior.
-> O trabalho pesado é feito na fase de "merge", onde duas listas ordenadas 
   tornam-se uma única lista ordenada.

ANÁLISE DO TEMPO:
-----------------

O array é dividido ao meio em cada nível da recursão:

Nível 0: 1 divisão  -> array de tamanho n  
Nível 1: 2 divisões -> cada parte de n/2  
Nível 2: 4 divisões -> cada parte de n/4  
...
Nível k: 2^k divisões -> partes de tamanho n / (2^k)

A recursão termina quando n/(2^k) = 1 → k = log2(n)

Em cada nível da recursão, o merge processa todos os elementos uma vez (n operações).

Logo:

T(n) = n * log(n)

T(n) ∈ O(n log n)

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int log_ctrl = 0;
int tam = 100;

void printArray(int array[], int tam);
void mergeSort(int array[], int inicio, int fim);
void merge(int array[], int inicio, int meio, int fim);

int main(int argc, char *argv[]){
    printf("# MERGE SORT\n");

    if(argc > 2){
        fprintf(stderr, "Excesso de parâmetros\n");
        return 1;
    }
    if(argc == 2){
        tam = atoi(argv[1]);
    }

    printf("Ordenando uma array com %d elementos no pior caso\n", tam);

    int *array = (int*)malloc(tam * sizeof(int));
    if(array == NULL){
        fprintf(stderr, "Erro na alocação de memória\n");
        return 1;
    }

    // Preenchendo com valores invertidos (Pior caso)
    for (int i = 0; i < tam; i++){
        array[i] = tam - i; 
    }

    if(log_ctrl) printf("Estado Inicial:\n");
    if(log_ctrl) printArray(array, tam);

    clock_t inicio = clock();
    mergeSort(array, 0, tam - 1);
    clock_t fim = clock();

    double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo de CPU: %f segundos\n", tempo_gasto);

    if(log_ctrl) printf("\nEstado Final:\n");
    if(log_ctrl) printArray(array, tam);

    free(array);
    return 0;
}

void printArray(int array[], int tam){
    for (int i = 0; i < tam; i++){
        if(i != (tam-1)) printf("%d | ", array[i]);
        else printf("%d\n", array[i]);
    }
}

/* MERGE SORT RECURSIVO
   Divide o array em dois, ordena as partes e depois chama merge()
*/
void mergeSort(int array[], int inicio, int fim){
    if(inicio < fim){
        int meio = (inicio + fim) / 2;

        mergeSort(array, inicio, meio);     // ordena metade esquerda
        mergeSort(array, meio + 1, fim);    // ordena metade direita

        merge(array, inicio, meio, fim);    // mescla as duas partes

        if(log_ctrl) printArray(array, fim - inicio + 1);
    }
}

/* MERGE
   Mescla duas partes ordenadas:
   - Parte 1: inicio → meio
   - Parte 2: meio+1 → fim
*/
void merge(int array[], int inicio, int meio, int fim){
    int tamanho_esq = meio - inicio + 1;
    int tamanho_dir = fim - meio;

    int *esq = malloc(tamanho_esq * sizeof(int));
    int *dir = malloc(tamanho_dir * sizeof(int));

    if(esq == NULL || dir == NULL){
        fprintf(stderr, "Erro na alocação de memória no merge()\n");
        exit(1);
    }

    // Copia dados para arrays temporários
    for(int i = 0; i < tamanho_esq; i++)
        esq[i] = array[inicio + i];

    for(int j = 0; j < tamanho_dir; j++)
        dir[j] = array[meio + 1 + j];

    // Índices
    int i = 0, j = 0, k = inicio;

    // Mescla as duas metades
    while(i < tamanho_esq && j < tamanho_dir){
        if(esq[i] <= dir[j]){
            array[k++] = esq[i++];
        } else {
            array[k++] = dir[j++];
        }
    }

    // Copia o que sobrou (se sobrou)
    while(i < tamanho_esq){
        array[k++] = esq[i++];
    }
    while(j < tamanho_dir){
        array[k++] = dir[j++];
    }

    free(esq);
    free(dir);
}
