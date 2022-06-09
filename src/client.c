#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.c"

typedef struct
{
    int cod;
    char *name;
    char *address;

    int (*mediaValores)(int, int, int);
} Client;

// Global
int lastClientIdx = -1;
Client clients[100] = {};

// prototypes
void NewClient(Client *c, int cod, char *name, char *address);
int Client_MediaValores(int v1, int v2, int v3);
void registerClient();

void NewClient(Client *c, int cod, char *name, char *address)
{
    c->cod = cod;
    // TODO: strcpy fica dando warning
    // strcpy(c->name, name);
    // strcpy(c->address, address);

    c->name = name;
    c->address = address;
    c->mediaValores = Client_MediaValores;
}

int Client_MediaValores(int v1, int v2, int v3)
{
    return ((v1 + v2 + v3) / 3);
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

    removeTrailingNewline(name);
    removeTrailingNewline(address);

    lastClientIdx++;
    const int clientCod = lastClientIdx;
    Client client;
    NewClient(&client, clientCod, name, address);
    clients[lastClientIdx] = client;
    printf("Cliente %s cadastrado. Cod do cliente: %d\n", client.name, client.cod);
}