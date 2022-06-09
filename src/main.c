#include "client.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lastClientIdx = -1;
Client clients[100] = {};

void registerClient();

// programa principal
int main(void)
{
    registerClient();
    registerClient();
    registerClient();
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

void registerClient()
{
    const size_t maxStrLength = 100;
    char *name = malloc(maxStrLength);
    char *address = malloc(maxStrLength);
    printf("Por favor, informe o nome do cliente: ");
    fgets(name, maxStrLength, stdin);
    printf("Por favor, informe o endereço do cliente: ");
    fgets(address, maxStrLength, stdin);

    /* Remove trailing newline, if there. */
    if ((strlen(name) > 0) && (name[strlen(name) - 1] == '\n'))
        name[strlen(name) - 1] = '\0';
    if ((strlen(address) > 0) && (address[strlen(address) - 1] == '\n'))
        address[strlen(address) - 1] = '\0';

    lastClientIdx++;
    const int clientCod = lastClientIdx;
    Client client;
    NewClient(&client, clientCod, name, address);
    clients[lastClientIdx] = client;
    printf("Cliente %s cadastrado. Cod do cliente: %d\n", client.name, client.cod);
}