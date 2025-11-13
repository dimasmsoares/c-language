/* 01_basic.c
Este programa demonstra a criação de um processo por meio da função fork() em C
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]){
    printf("### INICIO - %s\n", argv[0]);
    char nome[] = "DAD";
    pid_t id = fork();
    
    if(id < 0){
        perror("fork");
        exit(EXIT_FAILURE);     // equivale a exit(1)
    }

    if(id == 0){
        // Processo filho
        nome[0] = 'S';
        nome[1] = 'O';
        nome[2] = 'N';
        printf("[%s] Eu sou o processo PID = %d. Meu pai é o processo PID = %d\n",
               nome, getpid(), getppid());
        printf("[%s] wait() retornou %d (errno = %d)\n", nome, wait(0), errno);
    } else {
        // Processo pai
        printf("[%s] Eu sou o processo PID = %d. Meu pai é o processo PID = %d\n",
               nome, getpid(), getppid());
        int status;
        pid_t ret = wait(&status);
        printf("[%s] wait() retornou %d (status = %d)\n", nome, ret, WEXITSTATUS(status));
    }

    return 0;
}
