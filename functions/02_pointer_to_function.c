#include <stdio.h>

long long adicionar(int a, int b);
long long multiplicar(int a, int b);
long long subtrair(int a, int b);
long long dividir(int a, int b);
void operacao(int a, int b, long long (*funcao)(int, int));

int main(int argc, char *argv[]){
    int opt;
    long long (*function)(int, int);

    printf("1 - Adição\n");
    printf("2 - Multiplicação\n");
    printf("3 - Subtração\n");
    printf("4 - Divisão\n");
    printf("Qual a operação desejada: ");
    scanf("%d", &opt);
    switch(opt){
        case 1:
            printf("Adição\n");
            function = &adicionar;
            break;
        case 2:
            printf("Multiplicação\n");
            function = &multiplicar;
            break;
        case 3:
            printf("Subtração\n");
            function = &subtrair;
            break;
        case 4:
            printf("Divisão\n");
            function = &dividir;
            break;
        default:
            return 1;
    }

    operacao(9, 3, function);

    return 0;
}

long long adicionar(int a, int b){
    return a + b;
}
long long multiplicar(int a, int b){
    return a * b;
}
long long subtrair(int a, int b){
    return a - b;
}
long long dividir(int a, int b){
    return a / b;
}
void operacao(int a, int b, long long (*funcao)(int, int)){
    long long res = funcao(a,b);
    printf("%lld\n", res);
}