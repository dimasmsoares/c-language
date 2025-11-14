/* 02_mult_fork.c
Este programa demonstra a criação de multiplos processos por meio da função fork() em C
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]){
    printf("### INICIO %s\n", argv[0]);
    char nome[10] = "Pai";
    int resp_wait = 0;
    
    printf("[%s]Processo em execução: PID = %d\n", nome, getpid());

    printf("[%s] Chamando fork...\n", nome);
    pid_t id = fork();
    if(id == 0){
        nome[0] = 'F';
        nome[1] = 'i';
        nome[2] = 'l';
        nome[3] = 'h';
        nome[4] = 'o';

        printf("[%s] Chamando fork...\n", nome);
        id = fork();
        if(id == 0){
            nome[0] = 'N';
            nome[1] = 'e';
            nome[2] = 't';
            nome[3] = 'o';
            nome[4] = '\0';
        }
        else{
        resp_wait = wait(0);
        printf("[%s] Meu filho terminou: resp_wait = %d\n", nome, resp_wait);    
        }
    }
    else{
        resp_wait = wait(0);
        printf("[%s] Meu filho terminou: resp_wait = %d\n", nome, resp_wait);    
    }

    printf("[%s] Processo em execução: PID = %d | ID = %d\n", nome, getpid(), id);


    printf("[%s] FIM %s\n", nome, argv[0]);
    return 0;
}
