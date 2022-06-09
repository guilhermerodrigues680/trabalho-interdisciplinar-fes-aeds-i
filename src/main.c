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
    int runnig = 1;
    int selecao;

    printf("============== LocaMais ==================\n");
    printf("====== A melhor solução em locação! ======\n\n");
    while (runnig)
    {
        printf("## MENU ##\n");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Cadastrar veiculo\n");
        printf("3 - Cadastrar locação\n");
        printf("999 - SAIR\n");
        printf("\n>>> ");
        char str_f[10];
        fgets(str_f, 10, stdin);
        selecao = strtof(str_f, NULL);
        printf(">>> Selecionado %d\n\n", selecao);
        switch (selecao)
        {
        case 999: // stopProgram
            runnig = 0;
            break;
        case 1: // screenRegisterClient
            registerClient();
            break;
        case 2: // screenRegisterVehicle
            /* code */
            break;
        default:
            printf(">>> Opção inválida\n");
            break;
        }
    }

    printf("Até mais! Programa encerrado.");
    return 0;
}

void removeTrailingNewline(char *str)
{
    str[strcspn(str, "\r\n")] = 0;
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