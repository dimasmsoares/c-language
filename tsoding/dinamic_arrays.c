#include <stdio.h>
#include <stdlib.h>

typedef struct Numbers{
    int *items;       // Ponteiro para a memória real (o array no Heap)
    size_t count;     // Quantos elementos JÁ existem no array (tamanho lógico)
    size_t capacity;  // Quantos elementos CABEM no total atual (tamanho físico)
} Numbers;

int main(int argc, char *argv[]){

    // INÍCIO: O estado inicial é tudo zerado (NULL, 0, 0)
    Numbers xs = {0};

    for(int x = 0; x <=256; ++x){ // O loop tenta inserir 10 números (0 a 9)
        // PASSO 1: Verificação de Capacidade
        // "O array está cheio? Se sim, precisamos de um maior."
        if(xs.count >= xs.capacity){
            // Lógica de crescimento geométrico (0 -> 256 -> 512...)
            size_t new_capacity = (xs.count == 0) ? 256 : xs.capacity * 2;

            int *temp_items = realloc(xs.items, new_capacity * sizeof(*xs.items));
            if (temp_items == NULL) { // Verificação de Erro Crítico
                fprintf(stderr, "Erro: Falha ao alocar memória na RAM!\n");
                // Em caso de erro, a memória antiga em 'xs.items' ainda existe.
                // Devemos liberá-la antes de abortar para não deixar lixo na RAM.
                free(xs.items); 
                return 1; // Encerra o programa indicando erro
            }
            // SUCESSO: Agora é seguro atualizar a estrutura
            xs.items = temp_items;
            xs.capacity = new_capacity;
            printf("[DEBUG] Capacidade expandida para: %zu slots\n", xs.capacity);
        }
        // PASSO 2: Inserção
        // O ponteiro já aponta para um bloco de memória válido e com espaço.
        xs.items[xs.count++] = x;
    }


    // PASSO 3: Uso dos Dados
    for (size_t i = 0; i < xs.count; i++) {
        printf("%d\n", xs.items[i]);
    }

    // PASSO 4: Limpeza de Memória
    free(xs.items);
    xs.items = NULL; // Boa prática: evita "Dangling Pointers" (ponteiros que apontam pro nada)
    xs.count = 0;
    xs.capacity = 0;

    return 0;
}