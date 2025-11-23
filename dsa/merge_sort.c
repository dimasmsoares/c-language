#include <stdio.h>
#include <stdlib.h>

// Passamos o tamanho como parametro para evitar dependencia de globais
void printArray(int *array, int tam);
void bubble_sort(int *array, int tam);

int main(int argc, char *argv[]){
    int tam = 6;
    
    // Alocação Dinâmica (Heap)
    int *p_array = malloc(tam * sizeof(int));
    
    // Verificação de segurança (Sempre faça isso em sistemas reais)
    if(p_array == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        return 1;
    }

    // Preenchendo com valores invertidos (Pior caso)
    for (int i = 0; i < tam; i++){
        p_array[i] = 10 - i; 
    }

    printf("Estado Inicial:\n");
    printArray(p_array, tam);

    printf("\nIniciando Ordenacao...\n");
    // Passamos apenas o ponteiro (p_array), não o endereço do ponteiro (&p_array)
    bubble_sort(p_array, tam);

    printf("\nEstado Final:\n");
    printArray(p_array, tam);

    free(p_array); // Liberar memória é lei!
    return 0;
}

void printArray(int *array, int tam){
    for (int i = 0; i < tam; i++){
        if(i != (tam-1)) printf("%d | ", array[i]);
        else printf("%d\n", array[i]);
    }
}

// Recebendo ponteiro simples
void bubble_sort(int *array, int tam){
    int n = tam - 1;
    
    // CORREÇÃO: n > 0 para garantir a comparação dos índices [0] e [1]
    while(n > 0){
        for(int i = 0; i < n; i++){
            if(array[i] > array[i+1]){
                int aux = array[i];
                array[i] = array[i+1];
                array[i+1] = aux;
            }
        }
        // Opcional: Visualizar cada etapa macro
        printArray(array, tam); 
        n--;
    }
}