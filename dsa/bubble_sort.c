/* Algoritmo de Ordenação - BUBBLE SORT

A ideia central do Bubble Sort é:
-> Percorrer o array várias vezes.
-> Comparar elementos adjacentes.
-> Trocar se estiverem fora de ordem.
-> A cada passada (percorrer todo o array), o maior elemento vai parar no final.

Se a cada passado o maior elemento vai para o final, em uma array de n elementos temos que:
-> Na Passada [0], são feitas  n-1 comparações 
-> Na Passada [1], são feitas  n-2 comparações
    .
    .
    .
-> Na Passada [i], são feitas (n - (i+1)) comparações
    .
    .
    .
-> Na Passada [n-2], é feita 1 comparação

Logo em uma array de n elementos, serão feitas n-1 passadas (0 a n-2). Em cada passada serão 
feitas (n - (i + 1)) comparações, onde i é o indice da passada.

Ao todo serão feitas (n-1) + (n-2) + ... + 1 = (n * (n-1))/2
T(n) = (n² - n)/2
T(n) ∈ O(n²)

Uma forma de otimizar o bub

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Para controle de log: 0 - Desativado | 1 - Ativado
int log_ctrl = 0;
// Define o tamanho padrão da array a ser ordenada, caso não seja passado outro valor
int tam = 100;

void printArray(int array[], int tam);
void bubble_sort(int array[], int tam);

int main(int argc, char *argv[]){
    printf("# BUBBLE SORT\n");
    if(argc > 2){
        fprintf(stderr, "Excesso de parâmetros\n");
        return 1;
    }
    if(argc == 2){
        tam = atoi(argv[1]);
    }
    printf("Ordenando uma array com %d elementos no pior caso\n", tam);
    
    // Alocação dinâmica para evitar problemas com arrays grandes na stack
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

    if(log_ctrl) printf("\nIniciando Ordenacao...\n");
    // Captura o momento inicial (em ticks de clock)
    clock_t inicio = clock();   
    bubble_sort(array, tam);  
    // Captura o momento final
    clock_t fim = clock();      

    //Calcula a diferença e converte para segundos
    double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    
    printf("Tempo de CPU: %f segundos\n", tempo_gasto);

    if(log_ctrl) printf("\nEstado Final:\n");
    if(log_ctrl) printArray(array, tam);

    free(array); // Agora sim, liberando memória alocada dinamicamente
    return 0;
}

void printArray(int array[], int tam){
    for (int i = 0; i < tam; i++){
        if(i != (tam-1)) printf("%d | ", array[i]);
        else printf("%d\n", array[i]);
    }
}

void bubble_sort(int array[], int tam){
    
    for(int j = 0; j < (tam - 1);j++){              // Passadas
        for(int i = 0; i < (tam - j - 1); i++){     // Comparações
            int aux = 0;
            if(array[i] > array[i + 1]){
                aux = array[i + 1];
                array[i+1] = array[i];
                array[i] = aux;
            }
        }
        if(log_ctrl) printArray(array, tam);
    }
}