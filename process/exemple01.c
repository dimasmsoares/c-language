/* exemple01.c
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int var_global = 100;       // Variável global

int main(){
    pid_t var_id = 100;     // Variável local que receberá o valor retornado pela função fork()
    int var_local = 0;      // Variável local

    printf("INÍCIO DA EXECUÇÃO\n");
    printf("var_global = %d\n", var_global);
    printf("var_local = %d\n", var_global);
    printf("var_id = %d\n\n", var_id);
    
    printf("### CHAMANDO A FUNÇÃO FORK ###\n");

    var_id = fork();

    if(var_id == 0){    // Processo filho
        printf("# Processo Filho (PID = %d) (PPID = %d)\n", getpid(), getppid());
        printf("var_global = %d\n", var_global);
        printf("var_local = %d\n", var_local);
        printf("var_id = %d\n\n", var_id);
    }

    if(var_id != 0){    // Processo pai
        printf("# Processo Pai (PID = %d) (PPID = %d)\n", getpid(), getppid());
        printf("var_global = %d\n", var_global);
        printf("var_local = %d\n", var_local);
        printf("var_id = %d\n\n", var_id);
    }

    if(var_id == 0){    // Processo filho
        printf("### ALTERANDO O ESPAÇO DE MEMÓRIA DO FILHO ###\n");
        var_global = 123;
        var_local = 456;
        printf("# Processo Filho\n");
        printf("var_global = %d\n", var_global);
        printf("var_local = %d\n", var_local);
        printf("var_id = %d\n\n", var_id);
    }

    if(var_id != 0){    // Processo pai
        wait(0);        // Espera que o processo filho termine sua execução
        printf("# Processo Pai\n");
        printf("var_global = %d\n", var_global);
        printf("var_local = %d\n", var_global);
        printf("var_id = %d\n\n", var_id);
    }
    
    return 0;
}