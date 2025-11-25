#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

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
void freeList(List list);
void bubbleSort(List *pList);
void mergeSort(List *pList);

int main(int argc, char *argv[])
{
    int tam = 100;
    printf("# LINKED LIST\n");
    if(argc == 2){
        tam = atoi(argv[1]);
    }

    // Cria a lista inicialmente vazia
    List myList = {0};  
    
    // Inserindo os valores iniciais
    for(int i = 0; i < tam; i++){
        if(listAppend(&myList, tam - i) == 0) return 1;
    }
    //printf("ESTADO INICIAL\n");
    //printList(myList);

    clock_t inicio = clock();
    //bubbleSort(&myList);
    mergeSort(&myList);
    clock_t fim = clock();

    //printf("ESTADO FINAL\n");
    //printList(myList);

    double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("[BUBBLE SORT] Tempo de CPU: %f segundos\n", tempo_gasto);

    /*
    pid_t id = fork();
    if(id < 1)
    {
        fprintf(stderr, "Erro na criação do processo\n");
        freeList(myList);
        return 1;
    }

    if(id == 0)     // Processo filho
    {
        clock_t inicio = clock();
        bubbleSort();
        clock_t fim = clock();
        double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("[BUBBLE SORT]Tempo de CPU: %f segundos\n", tempo_gasto);
    }
    else            // Processo pai
    {
        clock_t inicio = clock();
        mergeSort();
        clock_t fim = clock();
        double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("[MERGE SORT]Tempo de CPU: %f segundos\n", tempo_gasto);
    }
    int resp_wait = wait(0);
    */

    freeList(myList);
    
    return 0;
}

void printList(List list)
{
    int count = 0;
    // Se a lista está vazia
    if(list.lenght == 0){
        printf("Lista Vazia\n");
    }
    else{
        Node *aux = list.pHead;
        while(aux != NULL && count < list.lenght){
            printf("[%02d|%p] \n", aux->value, aux->pNext);
            aux = aux->pNext;
            count ++;
        }
    }

}

int listAppend(List *pList, int v)
{
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

void freeList(List list)
{
    Node *aux = NULL;
    while(list.pHead != NULL){
        aux = list.pHead;
        list.pHead = list.pHead->pNext; 
        aux->value = 0;
        free(aux);
        //printf("Liberado %p\n", aux);
    }
}

void bubbleSort(List *pList)
{
    for(int p = 0; p < (pList->lenght - 1); p++)
    {
        Node *aux1 = pList->pHead;
        Node *aux2 = aux1->pNext;
        for(int c = 0; c < (pList->lenght - 1 - p); c++)
        {
            if(aux1->value > aux2->value)
            {
                int temp = aux1->value;
                aux1->value = aux2->value;
                aux2->value = temp;
            }
            aux1 = aux1->pNext;
            aux2 = aux2->pNext;
        }
        //printf("PASSADA %d\n", p);
        //printList(*pList);
    }   
}

void mergeSort(List *pList)
{
    if(pList->lenght > 1)
    {
        List listInf = {0};
        List listSup = {0};
        int i_inicio = 0;
        int i_fim = pList->lenght - 1;
        int i_metade = i_fim / 2;
        listInf.pHead = pList->pHead;
        listInf.lenght = i_metade + 1;
        
        Node *aux = pList->pHead;
        for(int i = 0; i < i_metade; i++) aux = aux->pNext;
        
        listSup.pHead = aux->pNext;
        listSup.lenght = i_fim - i_metade;
        
        printf("LISTA INFERIOR\n");
        printList(listInf);
        printf("LISTA SUPERIOR\n");
        printList(listSup);
        mergeSort(&listInf);
        mergeSort(&listSup);  
    }  
}


