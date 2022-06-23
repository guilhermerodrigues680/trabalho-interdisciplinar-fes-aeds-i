#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "utils.h"

// prototypes
void registerClient();
void listClients();
int getLastId();
int clientExists(int cod, Client *client);

// global
char *clientsDbFile = "clients_db.dat";

void registerClient()
{
    const size_t maxStrLength = 100;
    Client client;
    client.cod = getLastId() + 1;

    printf("Por favor, informe o nome do cliente: ");
    fgets(client.name, maxStrLength, stdin);
    printf("Por favor, informe o endereço do cliente: ");
    fgets(client.address, maxStrLength, stdin);

    removeTrailingNewline(client.name);
    removeTrailingNewline(client.address);

    FILE *fClientsPtr = fopen(clientsDbFile, "a+");
    fwrite(&client, sizeof(Client), 1, fClientsPtr);
    if (fwrite == 0)
        printf("erro interno ao cadastrar cliente.\n");
    else
        printf("Cliente %s cadastrado. Cod do cliente: %d\n", client.name, client.cod);

    fclose(fClientsPtr);
}

void listClients()
{
    FILE *fClientsPtr = fopen(clientsDbFile, "r");
    if (fClientsPtr == NULL) // Arquivo não existe
    {
        printf("* Nenhum cliente cadastrado.\n");
        return;
    }

    printf("* COD - NOME -> ENDERECO\n");
    Client client;
    while (fread(&client, sizeof(Client), 1, fClientsPtr))
        printf("* %d - %s -> %s\n", client.cod, client.name, client.address);

    fclose(fClientsPtr);
}

int getLastId()
{
    int lastId = -1;
    FILE *fClientsPtr = fopen(clientsDbFile, "r");
    if (fClientsPtr == NULL) // Arquivo não existe
        return lastId;

    // lê todos os clientes
    Client client;
    while (fread(&client, sizeof(Client), 1, fClientsPtr))
        lastId = client.cod;

    fclose(fClientsPtr);
    return lastId;
}

int clientExists(int cod, Client *client)
{
    FILE *fClientsPtr = fopen(clientsDbFile, "r");
    if (fClientsPtr == NULL) // Arquivo não existe, logo cliente não existe
        return 0;

    // https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(fClientsPtr, 0L, SEEK_END);
    long size = ftell(fClientsPtr);
    rewind(fClientsPtr);

    // como o id do cliente é incremental
    long offset = sizeof(Client) * cod;
    if (offset >= size) // se o offset é maior que o tamanho do arquivo, ou seja o client não existe
        return 0;

    fseek(fClientsPtr, offset, SEEK_SET);
    int exits = fread(client, sizeof(Client), 1, fClientsPtr) == 1;
    fclose(fClientsPtr);
    return exits;
}

const ClientsRepo clientsRepo = {
    .listClients = &listClients,
    .registerClient = &registerClient,
    .clientExists = &clientExists};