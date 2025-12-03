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
Node* mergeLists(Node *a, Node *b);
Node* mergeSortRec(Node *head);
void mergeSort(List *pList);
Node* mergeSortRec(Node *head);

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

/* Merge duas listas ordenadas (a e b) e retorna a cabeça da lista resultante */
Node* mergeLists(Node *a, Node *b)
{
    if (a == NULL) return b;
    if (b == NULL) return a;

    Node *head = NULL;   // cabeça da lista mesclada
    Node *tail = NULL;   // último nó atual

    // inicializa head
    if (a->value <= b->value) {
        head = tail = a;
        a = a->pNext;
    } else {
        head = tail = b;
        b = b->pNext;
    }

    // enquanto existirem nós em ambas as listas
    while (a != NULL && b != NULL) {
        if (a->value <= b->value) {
            tail->pNext = a;
            tail = a;
            a = a->pNext;
        } else {
            tail->pNext = b;
            tail = b;
            b = b->pNext;
        }
    }

    // anexa o restante
    if (a != NULL) tail->pNext = a;
    else tail->pNext = b;

    return head;
}

/* Encontra o meio da lista e o separa.
   Retorna o ponteiro para a segunda metade (head da metade superior).
   Ex.: lista: head -> ... -> mid -> midNext -> ... -> NULL
   Depois a lista é quebrada em: head...mid -> NULL  e midNext... -> NULL
*/
Node* splitMiddle(Node *head)
{
    if (head == NULL || head->pNext == NULL) return NULL;

    Node *slow = head;
    Node *fast = head;
    Node *prev = NULL;

    while (fast != NULL && fast->pNext != NULL) {
        prev = slow;
        slow = slow->pNext;
        fast = fast->pNext->pNext;
    }

    // agora slow aponta para o início da metade superior (ou para o meio+1)
    // prev é o último nó da primeira metade
    Node *second = slow;
    if (prev) prev->pNext = NULL; // corta a lista
    return second;
}

/* mergeSort recursivo que recebe head e retorna head ordenado */
Node* mergeSortRec(Node *head)
{
    // base: 0 ou 1 elemento
    if (head == NULL || head->pNext == NULL) return head;

    // divide
    Node *second = splitMiddle(head);

    // ordena recursivamente cada metade
    Node *left = mergeSortRec(head);
    Node *right = mergeSortRec(second);

    // merge e retorna head ordenado
    return mergeLists(left, right);
}

/* Função pública que você chama com a List inteira */
void mergeSort(List *pList)
{
    if (pList == NULL || pList->pHead == NULL || pList->lenght < 2) return;

    pList->pHead = mergeSortRec(pList->pHead);

    // lenght permanece igual; caso queira, você pode recomputar para segurança,
    // mas não é necessário pois não criamos/removemos nós.
}


