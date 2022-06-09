#include "client.c"
#include <stdio.h>

// programa principal
int main(void)
{
    Client client;
    NewClient(&client, 1, "Cliente 1", "rua da saudade 123");

    int res;
    printf("ok");

    // // popula objeto
    // client.Valor1 = 10;
    // client.Valor2 = 20;
    // client.Valor3 = 30;
    // printf("\n\rValor 1 do objeto: %d\n", client.Valor1);
    // printf("\n\rValor 2 do objeto: %d\n", client.Valor2);
    // printf("\n\rValor 3 do objeto: %d\n", client.Valor3);

    // // chama metodo do objeto
    // res = client.mediaValores(client.Valor1, client.Valor2, client.Valor3);
    // printf("\n\rResultado do metodo: %d\n\n", res);
}