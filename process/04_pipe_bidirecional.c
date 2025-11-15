#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]){
    printf("### INICIO COMUNICACAO BIDIRECIONAL\n");

    // pipe_p2c_fd[2]: Canal Pai (Escrita) -> Filho (Leitura)
    int pipe_p2c_fd[2]; 
    // pipe_c2p_fd[2]: Canal Filho (Escrita) -> Pai (Leitura)
    int pipe_c2p_fd[2]; 
    pid_t id;

    // 1. CRIAÇÃO DOS DOIS PIPES
    if(pipe(pipe_p2c_fd) == -1 || pipe(pipe_c2p_fd) == -1){
        perror("Erro na criação de um ou ambos os pipes");
        return 1;
    }

    // 2. BIFURCAÇÃO
    id = fork();
    
    if(id < 0){
        perror("Erro no fork");
        // É importante fechar pipes abertos antes de sair em caso de erro.
        close(pipe_p2c_fd[0]); close(pipe_p2c_fd[1]);
        close(pipe_c2p_fd[0]); close(pipe_c2p_fd[1]);
        return 2;
    }
    
    // --- LÓGICA DO PROCESSO FILHO (RECEBE e RESPONDE) ---
    if(id == 0){
        char nome[] = "Filho";
        char buffer[100];
        ssize_t n_bytes;
        int num;
        
        // --- ETAPA 1: FECHAR EXTREMIDADES NÃO UTILIZADAS NO FILHO ---
        // P2C Pipe: Filho vai ler, então FECHA a escrita P2C.
        close(pipe_p2c_fd[1]); 
        // C2P Pipe: Filho vai escrever, então FECHA a leitura C2P.
        close(pipe_c2p_fd[0]);

        // --- ETAPA 2: LEITURA DA MENSAGEM DO PAI ---
        n_bytes = read(pipe_p2c_fd[0], buffer, sizeof(buffer) - 1);
        if(n_bytes <= 0){
            perror("Filho: Erro ou EOF na leitura do Pai");
            return 3;
        }
        close(pipe_p2c_fd[0]); // Fecha leitura P2C após terminar
        
        buffer[n_bytes] = '\0';
        printf("[%s] Recebeu do Pai: '%s'. Calculando resposta...\n", nome, buffer);

        // Processamento: Converte para int e adiciona 5
        num = atoi(buffer);
        num += 5;

        // --- ETAPA 3: ESCRITA DA RESPOSTA PARA O PAI ---
        // Corrigido: Declara um buffer local para sprintf
        char response_msg[12];
        int len = sprintf(response_msg, "%d", num); 

        if(write(pipe_c2p_fd[1], response_msg, len) < 0){
            perror("Filho: Erro na escrita para o Pai");
            return 4;
        }   
        close(pipe_c2p_fd[1]); // Fecha escrita C2P após terminar
        printf("[%s] Resposta '%s' enviada ao Pai. Encerrando.\n", nome, response_msg);
    }
    
    // --- LÓGICA DO PROCESSO PAI (ENVIA e RECEBE) ---
    else{
        char nome[] = "Pai";
        char buffer[100];
        ssize_t n_bytes;

        // --- ETAPA 1: FECHAR EXTREMIDADES NÃO UTILIZADAS NO PAI ---
        // P2C Pipe: Pai vai escrever, então FECHA a leitura P2C.
        close(pipe_p2c_fd[0]); 
        // C2P Pipe: Pai vai ler, então FECHA a escrita C2P.
        close(pipe_c2p_fd[1]); 

        // --- ETAPA 2: ESCRITA DA MENSAGEM PARA O FILHO ---
        char *msg = "12";
        if(write(pipe_p2c_fd[1], msg, strlen(msg)) < 0){
            perror("Pai: Erro na escrita para o Filho");
            return 5;
        }   
        close(pipe_p2c_fd[1]); // Fecha escrita P2C após terminar
        printf("[%s] Mensagem inicial '%s' enviada ao Filho.\n", nome, msg);

        // --- ETAPA 3: LEITURA DA RESPOSTA DO FILHO ---
        // O Pai BLOQUEIA aqui, esperando a resposta do Filho.
        n_bytes = read(pipe_c2p_fd[0], buffer, sizeof(buffer) - 1);
        close(pipe_c2p_fd[0]); // Fecha leitura C2P após terminar
        
        if(n_bytes <= 0){
            perror("Pai: Erro ou EOF na leitura da resposta");
            return 6;
        }
        
        buffer[n_bytes] = '\0';
        printf("[%s] Resposta final do Filho recebida: '%s'\n", nome, buffer);
        
        // PONTO DIDÁTICO: Espera pelo término do processo filho.
        wait(NULL);
        printf("[%s] Processo Pai encerrando.\n", nome);
    }

    return 0;
}
