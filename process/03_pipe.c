/* 03_pipe.c
Este programa demonstra como processos podem trocar informações entre si

A função 'pipe' cria um canal de comunicação unidirecional (fluxo em apenas uma direção) entre dois processos,
operando em um modelo simples de produtor-consumidor.

int pipe(int pipefd[2]);

Quando chamada, a função pipe cria um buffer temporário na memória RAM, que é gerenciado diretamente pelo kernel do SO.
O pipe recebe como argumento uma array de inteiros de tamanho dois e registra nessa array os descritores de arquivos (file descriptors) usados pelo SO para apontar para 
a região da memória onde estar esse buffer:
-> pipefd[0]: O final do pipe de onde os dados podem ser lidos.
-> pipefd[1]: O final do pipe para onde os dados podem ser escritos.

Para que a comunicação entre dois processos funcione, você geralmente chama pipe() no processo pai e, em seguida, chama fork().
O SO duplica a tabela de descritores de arquivo e processo filho herda cópias dos mesmos descritores de arquivo que aponta para o buffer na RAM. 
É crucial fechar a extremidade não utilizada em cada processo para garantir o comportamento correto do sistema.
O fechamento (close()) é importante, pois garante que o kernel saiba que não há mais processos ativos interessados naquela extremidade do pipe.

*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h> // Para a função wait()
#include <errno.h>

int main(int argc, char *argv[]){
    printf("### INICIO - %s\n", argv[0]);
    char nome[10] = "Pai";
    int fd[2];          // Descritores do pipe: fd[0] leitura, fd[1] escrita
    pid_t id;           // Para o gerenciamento dos processos (pid_t é o tipo correto para IDs)

    // 1. CRIAÇÃO DO PIPE
    if(pipe(fd) == -1){ // Usando -1 como retorno de erro, mais idiomático
        perror("Erro na criação do pipe");
        return 1;
    }

    // 2. BIFURCAÇÃO DO PROCESSO
    id = fork();
    
    if(id < 0){     // Erro no fork
        perror("Erro no fork");
        return 2;
    }
    
    // --- LÓGICA DO PROCESSO FILHO (CONSUMIDOR) ---
    if(id == 0){    
        strcpy(nome, "Filho"); // Forma mais limpa de alterar a string
        
        // CRÍTICO: Fecha o lado de escrita
        close(fd[1]); 
        
        char buffer[100];
        // Bloqueia e lê do lado fd[0]
        ssize_t n_bytes = read(fd[0], buffer, sizeof(buffer) - 1); 

        // CRÍTICO: Fecha o lado de leitura após o uso
        close(fd[0]);

        if(n_bytes < 0){
            perror("Erro na leitura");
            return 3;
        }
        
        // PONTO DIDÁTICO: Adiciona o terminador nulo para que o printf funcione
        buffer[n_bytes] = '\0'; 
        printf("[%s] Mensagem recebida: \"%s\"\n", nome, buffer);
    }
    
    // --- LÓGICA DO PROCESSO PAI (PRODUTOR) ---
    else{ 
        // CRÍTICO: Fecha o lado de leitura
        close(fd[0]); 
        
        char *msg = "Ola filho"; 
        
        // Escreve no lado fd[1]
        if(write(fd[1], msg, strlen(msg)) < 0){
            perror("Erro na escrita");
            return 4;
        }

        // CRÍTICO: Fecha o lado de escrita após o uso. Essencial para sinalizar EOF.
        close(fd[1]);

        printf("[%s] Mensagem enviada e pipe de escrita fechado.\n", nome);
        
        // PONTO DIDÁTICO: Espera pelo término do processo filho.
        wait(NULL); // wait(NULL) é o padrão para ignorar o status de saída.
        printf("[%s] Filho terminou, processo pai encerrando.\n", nome);
    }

    return 0;
}
