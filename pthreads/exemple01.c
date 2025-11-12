/* exemple01.c
Este código demonstra o processamento paralelo por meio de threads.

O programa cria duas thread: uma responsável por imprimir números e outra responsável por imprimir letras

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct RangeNumber{
    int start;
    int end;
} RangeNumber;

typedef struct RangeLetter{
    char start;
    char end;
} RangeLetter;

void *routine_t1(void *arg);
void *routine_t2(void *arg);

int main(){
    // Estruturas para id das threads
    pthread_t t1, t2;   

    // Argumentos para as threads
    RangeNumber rn; rn.start = 0; rn.end = 9;
    RangeLetter rl; rl.start = 'a'; rl.end = 'e';

    // Variáveis para armazenar o retorno das threads
    void *r_t1; // ponteiro para void*, que receberá o valor de retorno da thread t1
    void *r_t2; // ponteiro para void*, que receberá o valor de retorno da thread t2

    if(pthread_create(&t1, NULL, routine_t1, (void *)&rn) < 0){
        return 1;
    }
    if(pthread_create(&t2, NULL, routine_t2, (void *)&rl) < 0){
        return 1;
    }
    if(pthread_join(t1, &r_t1) < 0){ // r_t1 agora contém o mesmo endereço de memória da heap que foi retornado por routine_t1
        return 1;
    }
    if(pthread_join(t2, &r_t2) < 0){
        return 1;
    }

    printf("Thread 1 - Imprimiu %d números\n", *((int *)r_t1));
    printf("Thread 2 - Imprimiu %d letras\n", *((int *)r_t2));

    free(r_t1);
    free(r_t2);

    return 0;
}

void *routine_t1(void *arg){
    RangeNumber *aux = (RangeNumber *)arg;
    
    int *count_ptr = (int *)malloc(sizeof(int)); // Alocação dinâmica, pois a função retornará esse ponteiro, não pode ser alocado na stack
    if (count_ptr == NULL) return NULL;
    *count_ptr = 0;
    
    printf("Thread 1 - Imprimindo números de %d a %d\n", aux->start, aux->end);
    sleep(1);
    for(int i = aux->start; i <= aux->end; i++){
        printf("%d\n", i);
        sleep(1);
        (*count_ptr)++;
    }
    return (void *)count_ptr;   // Retorna o endereço alocado
}

void *routine_t2(void *arg){
    RangeLetter *aux = (RangeLetter *)arg;
    
    int *count_ptr = (int *)malloc(sizeof(int)); // Alocação dinâmica
    if (count_ptr == NULL) return NULL;
    *count_ptr = 0;

    printf("Thread 2 - Imprimindo letras de %c a %c\n", aux->start, aux->end);
    sleep(1);
    for(char c = aux->start; c <= aux->end; c++){
        printf("%c\n", c);
        sleep(1);
        (*count_ptr)++;
    }
    return (void *)count_ptr;   // Retorna o endereço alocado
}