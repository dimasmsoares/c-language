#include <stdio.h>

long long adicionar(int a, int b);
long long multiplicar(int a, int b);

int main(int argc, char *argv[]){
    
    long long (*funcao)(int, int);
    
    funcao = &adicionar;
    printf("%lld\n", funcao(5,6));
    
    funcao = &multiplicar;
    printf("%lld\n", funcao(5,6));

    return 0;
}

long long adicionar(int a, int b){
    return a + b;
}
long long multiplicar(int a, int b){
    return a * b;
}