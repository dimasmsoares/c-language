#include <stdio.h>
#include <stdlib.h>

const int log_ctrl = 0;  // Para controle de LOG

// Estrutura de dado usada para a lista
typedef struct Node{
    int value;
    struct Node *next;
} Node;

// Protótipos das funções de manipulação da lista
Node *createNode(int value);
int insertEnd(Node **list, int value);
int insertBegin(Node **list, int value);
int printList(Node *list);
int removeBegin(Node **pp_list, int *removed_value);
int removeEnd(Node **pp_list, int *removed_value);
void freeList(Node *list);

int main(int argc, char *argv[]){
    printf("[%s] INÍCIO\n", argv[0]);
    Node *p_list = NULL;    // p_list é um ponteiro para o primeiro nó da lista. Ele é armazenado na STACK.
                            // OBS: Os nós da lista serão todos armazenados na HEAP.

    printf("ADICIONANDO VALORES\n");
    insertBegin(&p_list, 12);
    insertEnd(&p_list, 12);
    insertEnd(&p_list, 13);
    insertBegin(&p_list, 0);
    insertBegin(&p_list, 56);
    insertBegin(&p_list, -1);
    insertEnd(&p_list, 15);
    insertEnd(&p_list, 1);
    insertEnd(&p_list, 20);
    
    printf("IMPRIMINDO A LISTA\n");
    printList(p_list);

    printf("REMOVENDO VALORES DO INICIO\n");
    int removed_value = 0;
    removeBegin(&p_list, &removed_value);
    printf("Valor %d removido do inicio\n", removed_value);

    removeBegin(&p_list, &removed_value);
    printf("Valor %d removido do inicio\n", removed_value);

    removeEnd(&p_list, &removed_value);
    printf("Valor %d removido do fim\n", removed_value);

    removeBegin(&p_list, &removed_value);
    printf("Valor %d removido do inicio\n", removed_value);

    removeBegin(&p_list, &removed_value);
    printf("Valor %d removido do inicio\n", removed_value);

    removeEnd(&p_list, &removed_value);
    printf("Valor %d removido do fim\n", removed_value);

    printf("IMPRIMINDO A LISTA\n");
    printList(p_list);
    
    freeList(p_list);
    return 0;
}

Node *createNode(int value){
    Node *node = malloc(sizeof(Node));
    if(node == NULL){
        perror("Erro na alocacao dinamica (malloc)");
        return NULL;
    }
    // Inicialização segura dos campos
    node->value = value; // Se passarmos o valor
    node->next = NULL;
    return node;
}

int insertBegin(Node **pp_list, int value){
    /*Essa fução tem como objetivo incluir um novo nó no inicio da lista. Ela recebe como parâmetros:
    -> list: ponteiro para o ponteiro (p_list na main) que aponta para o primeiro nó da lista.
    -> value: valor inteiro que será armazenado no novo nó criado.
    
    Se a função recebesse p_list da main diretamente (Node *p_list, ao invés de Node **pp_list), no escopo desse função
    p_list seria uma variável local que armazenaria uma cópia do endereço do primeiro nó na lista. Alterar o valor dessa cópia
    não adiantaria nada. Por isso a função recebe Node **pp_list (&p_list da main), pois assim pp_list contém o endereço do ponteiro
    para o primeiro nó da lista (p_list) e consegue altera-lo.
    */

    // Cria o novo Nó e já inicializa o value e next = NULL
    if(log_ctrl) printf("\tIniciando insercao ...\n");
    Node *new_node = createNode(value);
    if(new_node == NULL){
        return 0;   // Erro de alocação
    }

    if(log_ctrl) printf("\tNó criado\n");
    
    // Se a lista estiver vazia
    if(*pp_list == NULL){      // Se p_list aponta para NULL
        *pp_list = new_node;   // p_list passa a apontar para o novo nó criado
        if(log_ctrl) printf("\tA lista estava vazia! Nó inserido\n");
        return 1;
    }
    // Se a lista NÃO está vazia
    if(log_ctrl) printf("\tA lista não está vazia!\n");
    new_node->next = *pp_list;
    *pp_list = new_node;       // p_list passa a apontar para o novo nó criado
    if(log_ctrl) printf(" Valor inserido!\n");
    return 1;
}

int removeBegin(Node **pp_list, int *removed_value){
    // Se a lista está vazia
    if(*pp_list == NULL){
        return 0;  // falha
    }
    // Se a lista não está vazia
    Node *aux = *pp_list;
    *removed_value = aux->value;

    *pp_list = aux->next;
    free(aux);
    return 1;  // sucesso
}

int removeEnd(Node **pp_list, int *removed_value){
    // lista vazia
    if (*pp_list == NULL) {
        return 0;
    }

    Node *atual = *pp_list;
    Node *anterior = NULL;

    // só existe um nó
    if (atual->next == NULL) {
        *removed_value = atual->value;
        free(atual);
        *pp_list = NULL;
        return 1;
    }

    // lista com 2 ou mais nós
    while (atual->next != NULL) {
        anterior = atual;
        atual = atual->next;
    }

    // 'atual' é o último nó
    // 'anterior' é o penúltimo nó

    *removed_value = atual->value;
    anterior->next = NULL;
    free(atual);

    return 1;
}

int insertEnd(Node **pp_list, int value){
    // Cria o novo Nó e já inicializa o value e next = NULL
    if(log_ctrl) printf("\tIniciando insercao ...\n");
    Node *new_node = createNode(value);
    if(new_node == NULL){
        return 0;   // Erro de alocação
    }

    if(log_ctrl) printf("\tNó criado\n");
    
    // Se a lista estiver vazia (O ponteiro principal aponta para NULL)
    if(*pp_list == NULL){      // Se p_list aponta para NULL
        *pp_list = new_node;   // p_list passa a apontar para o novo nó criado
        if(log_ctrl) printf("\tA lista estava vazia! Nó inserido\n");
        return 1;
    }
    if(log_ctrl) printf("\tA lista não estava!");

    // Se a lista NÃO está vazia, pecorre-se ela até o último nó
    Node *aux = *pp_list;  // aux aponta para o primeiro nó da lista
    while(aux->next != NULL){
        aux = aux->next;
    }

    // aux aqui aponta para o ultimo nó. Conecta o novo nó.
    aux->next = new_node;
    
    if(log_ctrl) printf(" Valor inserido!\n");
    return 1;
}

int insertAt(Node **pp_list, int position){
    
}

int printList(Node *p_list){
    int count = 0;
    if(p_list == NULL){
        printf("A lista está vazia\n");
        return count;
    }
    Node *aux = p_list;
    while(aux != NULL){
        printf("%d\n", aux->value);
        count ++;
        aux = aux->next;
    }
    printf("A lista tem %d valores\n", count);
    return count;
}

void freeList(Node *p_list){
    Node *aux = NULL;           // Ponteiro auxiliar para guardar o nó a ser liberado
    while(p_list != NULL){        // Enquanto houver um nó na lista
        aux = p_list;             // 'aux' guarda o nó atual
        p_list = p_list->next;    // 'p_list' avança para o PRÓXIMO nó
        free(aux);              // Libera o nó que estava em 'aux'
    }
}

/*
removeEnd()
removeBegin()
printList()
*/