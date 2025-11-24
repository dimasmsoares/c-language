#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node
{
    int value;
    struct Node *pNext;
} Node;

typedef struct
{
    Node *pHead;
    int lenght;
} List;

void printList(List list);
int listAppend(List *pList, int v);

int main(int argc, char *argv[])
{
    printf("# LINKED LIST\n");

    // Valores para compor a lista inicial
    int valores_inciais[10] = {75, 65, 80, 99, 15, 16, 17, 47, 6, 13};

    // Cria a lista inicialmente vazia
    List myList = {0};
    printList(myList);  
    
    // Inserindo os valores iniciais
    for(int i = 0; i < 10; i++){
        if(listAppend(&myList, valores_inciais[i]) == 0) return 1;
    }

    printList(myList);
    
    return 0;
}

void printList(List list)
{
    // Se a lista está vazia
    if(list.lenght == 0){
        printf("Lista Vazia\n");
    }
    else{
        Node *aux = list.pHead;
        while(aux != NULL){
            printf("[%02d|%p] \n", aux->value, aux->pNext);
            aux = aux->pNext;
        }
    }

}

int listAppend(List *pList, int v){
    // Alocando a um novo nó
    Node *newNode = (Node*)malloc(sizeof(Node));
    if(newNode == NULL){
        fprintf(stderr, "Erro na alocação de memória\n");
        return 0;
    }

    // Inicializando o novo nó
    newNode->value = v;
    newNode->pNext = NULL;

    // Se a lista está vazia
    if(pList->lenght == 0){
        pList->pHead = newNode;
    }
    else{
        Node *aux = pList->pHead;
        while(aux->pNext != NULL){
            aux = aux->pNext;
        }
        aux->pNext = newNode;
    }
    pList->lenght++;
    return 1;
}

void freeList(List list){
    Node *aux = list.pHead;
    while(list.pHead != NULL){
        free(aux);
        aux = 
    }
}