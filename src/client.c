#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.c"

typedef struct
{
    int cod;
    char *name;
    char *address;
} Client;

// prototypes
void registerClient();
void listClients();
int getLastId();

void registerClient()
{
    const size_t maxStrLength = 100;

    Client client;
    client.cod = getLastId() + 1;
    client.name = malloc(maxStrLength);
    client.address = malloc(maxStrLength);

    printf("Por favor, informe o nome do cliente: ");
    fgets(client.name, maxStrLength, stdin);
    printf("Por favor, informe o endereço do cliente: ");
    fgets(client.address, maxStrLength, stdin);

    removeTrailingNewline(client.name);
    removeTrailingNewline(client.address);

    FILE *fClientsPtr = fopen("clients_db.txt", "a+");
    fprintf(fClientsPtr, "%d | %s | %s\n", client.cod, client.name, client.address);
    fclose(fClientsPtr);

    printf("Cliente %s cadastrado. Cod do cliente: %d\n", client.name, client.cod);
}

void listClients()
{
    FILE *fClientsPtr = fopen("clients_db.txt", "a+");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    printf("* COD - NOME -> ENDERECO\n");
    while ((read = getline(&line, &len, fClientsPtr)) != -1)
    {
        Client client;
        client.name = malloc(100);
        client.address = malloc(100);
        // https://stackoverflow.com/questions/2854488/reading-a-string-with-spaces-with-sscanf
        sscanf(line, "%d | %[^|] | %[^\t\n]", &client.cod, client.name, client.address);
        printf("* %d - %s -> %s\n", client.cod, client.name, client.address);
    }
    fclose(fClientsPtr);
}

int getLastId()
{
    FILE *fClientsPtr = fopen("clients_db.txt", "a+");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int lastId = -1;
    while ((read = getline(&line, &len, fClientsPtr)) != -1)
    {
        sscanf(line, "%d", &lastId);
    }
    fclose(fClientsPtr);

    return lastId;
}