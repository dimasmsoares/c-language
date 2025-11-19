#include <stdio.h>
#include <stdlib.h>

const int log_ctrl = 1;

typedef struct Node{
    int value;
    struct Node *next;
    struct Node *prior;
} Node;

typedef struct List{
    struct Node *first;
    struct Node *last;
    int lenth;
} List;

int insertEnd(List *p_list, int value);
void freeList(List lista);
void printList(List lista);

int main(int argc, char *argv[]){
    int value_insert = 0;

    if(log_ctrl) printf("[INICIO] %s\n", argv[0]);
    
    List lista = {NULL, NULL, 0};
    if(log_ctrl) printf("Lista vazia criada\n");

    value_insert = 100;
    insertEnd(&lista, value_insert);
    if(log_ctrl) printf("Valor %d inserido\n", value_insert);

    value_insert = 200;
    insertEnd(&lista, value_insert);
    if(log_ctrl) printf("Valor %d inserido\n", value_insert);
    

    value_insert = 300;
    insertEnd(&lista, value_insert);
    if(log_ctrl) printf("Valor %d inserido\n", value_insert);

    value_insert = 400;
    insertEnd(&lista, value_insert);
    if(log_ctrl) printf("Valor %d inserido\n", value_insert);

    value_insert = 500;
    insertEnd(&lista, value_insert);
    if(log_ctrl) printf("Valor %d inserido\n", value_insert);

    printList(lista);
    printf("Tamanho da lista %d\n", lista.lenth);

    freeList(lista);
    if(log_ctrl) printf("Lista liberada da memória\n");

    if(log_ctrl) printf("[FIM] %s\n", argv[0]);
    return 0;
}

int insertEnd(List *p_list, int value){
    // Criando um nó na HEAP e guardando o ponteiro para ele em p_node
    Node *p_node = malloc(sizeof(Node));
    if(p_node == NULL){
        if(log_ctrl) printf("[insertEnd]Erro na alocação do nó\n");
        return 0;
    }

    // Inicializando o nó recém-criado
    p_node->next = NULL;
    p_node->prior = NULL;
    p_node->value = value;

    // Caso a lista esteja vazia
    if(p_list->lenth == 0){
        p_list->first = p_node;
        p_list->last = p_node;
        p_list->lenth = 1;
        return 1;
    }

    // Caso a lista não esteja vazia
    Node *aux = p_list->last;   
    aux->next = p_node; 
    p_list->last = p_node; 
    p_node->prior = aux;
    p_list->lenth ++;

    return 1;
}

void printList(List lista){
    Node *aux = lista.first;
    int count = 0;
    while(aux != NULL){
        printf("[%d] %d\n", count, aux->value);
        aux = aux->next;
        count ++;
    }
}

void freeList(List lista){
    Node *aux1 = lista.first;
    Node *aux2 = NULL;
    while(aux1 != NULL){
        aux2 = aux1;
        aux1 = aux1->next;
        free(aux2);
    }
}

