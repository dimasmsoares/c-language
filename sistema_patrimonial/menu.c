#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int log_ctrl = 0;

typedef struct ConfigMenu
{
    char titulo[64];
    char opcoes[256];
    char separador[2];     // 1 caractere + '\0'
} ConfigMenu;

int menu(ConfigMenu cm);

int main(int argc, char *argv[])
{
    // ConfigMenu x = {"SISTEMA PATRIMONIAL", "Opção 1|Opção 2|Opção 3|Opção 4|Opção 5|Opção 6|Opção 7|Opção 8|Opção 9|Opção 10", "|"};
    ConfigMenu x = {"SISTEMA PATRIMONIAL", "Opção 1|Opção 2|Opção 3|Opção 4|Opção 5|Opção 6|Opção 7", "|"};
    menu(x);
    return 0;
}

int menu(ConfigMenu cm)
{
    // Variável que armazenará o valor retornado (o número da opção escolhida)
    int valorRetornado;

    // Variável que contará a quantidade de opções
    int countOpcoes;
    
    // Variável que indicará se a entrada é válida (=1)
    int entradaValida;
    
    // Variável usada para que a função strtok possa manipular as opções sem alterar cm.opcoes
    char temp[256];             

    // Variável que indicará o tamanho do buff
    int tam_buff;
    
    char *res = NULL;

    char *buff = NULL;

    do
    {
        valorRetornado = 0;
        countOpcoes = 0;
        entradaValida = 1;
        strcpy(temp, cm.opcoes);
        tam_buff = 0;
        res = NULL;
        buff = NULL;

        // Imprimindo o Menu
        system("clear");
        printf("%s\n", cm.titulo);
        res = strtok(temp, cm.separador);
        while (res != NULL)
        {
            printf("[%d] - %s\n", ++countOpcoes, res);
            res = strtok(NULL, cm.separador);
        }

        // Verificando quantas opções tem e qual o tamanho necessário para o buff (buff tem que capturar o '\n')
        tam_buff = (countOpcoes/10) + 3;    // digitos + '\n' + '\0'
        if(log_ctrl) printf("tam_buff = %d, pois temos %d opções.\n", tam_buff, countOpcoes);

        // Limpa o buff
        buff = (char *)malloc(tam_buff*sizeof(char));
        memset(buff, 0, tam_buff);

        if(log_ctrl)
        {
            for(int i = 0; i < tam_buff; i++)
            {   
                if(buff[i] != '\n' && buff[i] != '\0') printf("[%p] %c\n", &buff[i], buff[i]);
                else
                {
                    if(buff[i] == '\n') printf("[%p] ENTER\n", &buff[i]);
                    else printf("[%p] NULL\n", &buff[i]);
                }
            } // Fim do for
        }
        

        printf("Selecione a opção desejada: ");
        if(fgets(buff, tam_buff, stdin) == NULL)
        {
            if(log_ctrl) printf("[menu] Erro no fgets\n");
        }

        // Verifica se o ENTER ('\n') foi capturado
        int enter_position = (int)strcspn(buff,"\n");
        if(enter_position >= tam_buff-1)
        {
            if(log_ctrl) printf("O ENTER não foi capturado pelo buff\n");
            entradaValida = 0;
            while(getchar() != '\n');   // Limpa o buffer de entrada
        }
        else
        {
            if(log_ctrl) printf("O ENTER está na posição %d\n", enter_position);
            // Verifica se os caracteres antes do ENTER são numéricos
            for(int i = 0; i < enter_position; i++)
            {
                if(buff[i] < '0' || buff[i] > '9')
                {
                    entradaValida = 0;
                    if(log_ctrl) printf("O caractere na posição %d não é númerico\n", i);
                    break;
                }

            }
            if(entradaValida)
            {
                if(log_ctrl) printf("O(s) caracter(es) é(são) númerico(s)\n");
                valorRetornado = atoi(buff);
            }
            if(valorRetornado > countOpcoes || valorRetornado <= 0)
            {
                entradaValida = 0;
                valorRetornado = 0;
                printf("O valor da entrada tem que estar entre 0 e %d\n", countOpcoes);
            }
        }

        if(entradaValida)
        {
            if(log_ctrl) printf("Entrada Válida\n");
        } 
        else
        { 
            if(log_ctrl) printf("Entrada Inválida\n");
        }

        if(log_ctrl)
        {
            for(int i = 0; i < tam_buff; i++)
            {   
                if(buff[i] != '\n' && buff[i] != '\0') printf("[%p] %c\n", &buff[i], buff[i]);
                else
                {
                    if(buff[i] == '\n') printf("[%p] ENTER\n", &buff[i]);
                    else printf("[%p] NULL\n", &buff[i]);
                }
            }        
        }
    } while (entradaValida == 0);
    
    free(buff);
    if(log_ctrl) printf("valorRetornado = %d\n", valorRetornado);
    return valorRetornado;
}
