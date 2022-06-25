/* client.c
 *
 * Este arquivo representa a implementação de funções para manipulação
 * de clientes no sistema.
 *
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "utils.h"

static int get_last_id();

char *clients_db_file = "_db_clients.dat";

int client_register(const char *name, const char *address)
{
    Client client = {
        .cod = get_last_id() + 1,
    };

    strcpy(client.name, name);
    strcpy(client.address, address);

    FILE *f_ptr = fopen(clients_db_file, "a+");
    if (fwrite(&client, sizeof(Client), 1, f_ptr) != 1)
    {
        printf("erro interno ao cadastrar cliente.\n");
        fclose(f_ptr);
        return EXIT_FAILURE;
    }

    printf("Cliente %s cadastrado. Cod do cliente: %d\n", client.name, client.cod);
    fclose(f_ptr);
    return EXIT_SUCCESS;
}

void client_list()
{
    FILE *fClientsPtr = fopen(clients_db_file, "r");
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

int get_last_id()
{
    int lastId = -1;
    FILE *fClientsPtr = fopen(clients_db_file, "r");
    if (fClientsPtr == NULL) // Arquivo não existe
        return lastId;

    // lê todos os clientes
    Client client;
    while (fread(&client, sizeof(Client), 1, fClientsPtr))
        lastId = client.cod;

    fclose(fClientsPtr);
    return lastId;
}

int client_get_by_cod(int cod, Client *client)
{
    FILE *fClientsPtr = fopen(clients_db_file, "r");
    if (fClientsPtr == NULL) // Arquivo não existe, logo cliente não existe
        return EXIT_FAILURE;

    // https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(fClientsPtr, 0L, SEEK_END);
    long size = ftell(fClientsPtr);
    rewind(fClientsPtr);

    // como o id do cliente é incremental
    long offset = sizeof(Client) * cod;
    if (offset >= size) // se o offset é maior que o tamanho do arquivo, ou seja o client não existe
    {
        fclose(fClientsPtr);
        return EXIT_FAILURE;
    }

    fseek(fClientsPtr, offset, SEEK_SET);
    if (fread(client, sizeof(Client), 1, fClientsPtr) == 1 != 1)
    {
        fclose(fClientsPtr);
        return EXIT_FAILURE;
    }

    fclose(fClientsPtr);
    return EXIT_SUCCESS;
}
