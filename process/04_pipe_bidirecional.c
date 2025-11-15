#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h> // Para a função wait()
#include <errno.h>


int main(int argc, char *argv[]){
    printf("### INICIO - %s\n", argv[0]);

    int pipe_p2c_fd[2]; // Parent to child
    int pipe_c2p_fd[2]; // Child to parent
    pid_t id;

    // 1. Criação dos pipes
    if(pipe(pipe_p2c_fd) == -1){
        perror("Erro no pipe p2c");
        return 1;
    }

    if(pipe(pipe_c2p_fd) == -1){
        perror("Erro no pipe c2p");
        return 1;
    }

    // 2.Bifurcaçãp dos processos
    id = fork();
    
    // 3. Processo filho
    if(id == 0){
        close(pipe_p2c_fd[1]);   // Fecha o de escrita, pois vai ler uma msg
        char buffer[5];
        ssize_t n_bytes = read(pipe_p2c_fd[0], buffer, sizeof(buffer) -1);    // Abre o de leitura e ler a mensagem
        close(pipe_p2c_fd[0]);
        if(n_bytes < 0){
            perror("Erro na leitura");
            return 1;
        }
        buffer[n_bytes] = '\0';
        printf("%s\n", buffer);

        int num = atoi(buffer);
        
    }
    
    // 4. Processo Pai
    else{
        close(pipe_p2c_fd[0]);   // Fecha o de leitura, pois vai enviar uma msg
        char *msg = "12";
        if(write(pipe_p2c_fd[1], msg, strlen(msg)) < 0){    // Abre o de escrita e manda a mensagem
            perror("Erro na escrita");
            return 1;
        }   
        close(pipe_p2c_fd[1]);   // Fecha o de escrita

        wait(NULL);
    }

    return 0;
}

char *divisores(int n){
    
}
    