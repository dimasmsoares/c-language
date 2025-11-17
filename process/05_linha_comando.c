#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]){
    
    char nome[30] = "Processo Pai"; // Declara um buffer local, não um ponteiro para literal.
    printf("[%s] Eu sou o processo Pai! Meu PID é: %d. Ainda não tenho filhos...\n", nome, getpid());
    
    int fd_pipe_A_B[2];
    pid_t id_a, id_b;
    
    if(pipe(fd_pipe_A_B) == -1){
        perror("Erro na criação do pipe");
        return 1;
    }
    id_a = fork();
    if(id_a == 0){  // Processo A: FILHO
        strcpy(nome, "Processo A");
        printf("[%s] Eu sou o filho A!\n", nome);
        /*Lógica do A*/
        close(fd_pipe_A_B[0]);  // A não vai ler na pipe, vai escrever
        if(dup2(fd_pipe_A_B[1], 1) == -1){    // Redireciona a saída padrão (1) para a extremidade de escrita do pipe
            perror("Erro ao redirecionar STDOUT");
            return 1;
        }
        close(fd_pipe_A_B[1]);   // Fechou o descritor original de escrita (fd_pipe_A_B[1]), que agora é redundante.


    }
    else if(id_a > 0){      // Processo PAI
        printf("[%s] Já tenho o filho A! PID de A é:%d\n", nome, id_a);
        id_b = fork();
        if(id_b == 0){      // Processo B: FILHO
            printf("[%s] Eu sou o filho B!\n", nome);
            /*Lógica do B*/
            close(fd_pipe_A_B[1]);  // B não vai escrever na pipe, vai ler
            if(dup2(fd_pipe_A_B[0], 0) == -1){    // Redireciona a entrada padrão (0) para a extremidade de leitura do pipe
                perror("Erro ao redirecionar STDIN");
                return 1;
            }
            close(fd_pipe_A_B[0]);   // Fecha o descritor original de leitura (fd_pipe_A_B[0]), que agora é redundante.





        }
        else if(id_a > 0){  // Processo PAI
            printf("[%s] Já tenho o filho B! PID de B é:%d\n", nome, id_b);
            /*Lógica do PAI*/
            close(fd_pipe_A_B[0]);  // Pai não vai ler na pipe  
            close(fd_pipe_A_B[1]);  // Pai não vai escrever na pipe




        }
        else{
            perror("Erro ao criar o processo filho B");
            close(fd_pipe_A_B[0]);
            close(fd_pipe_A_B[1]);
            return 1;
        }
    }
    else{
        perror("Erro ao criar o processo filho A");
        close(fd_pipe_A_B[0]);
        close(fd_pipe_A_B[1]);
        return 1;
    }

    

    return 0;
}