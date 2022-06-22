#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "service.h"

// prototypes
void clearScreen();

// programa principal
int main(void)
{
    int runnig = 1;
    int selecao = -1;

    printf("============== LocaMais ==================\n");
    printf("====== A melhor solução em locação! ======\n\n");
    while (runnig)
    {
        printf("## MENU ##\n");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Cadastrar veiculo\n");
        printf("3 - Cadastrar locação\n");
        printf("4 - Listar clientes\n");
        printf("5 - Listar veiculos\n");
        printf("6 - Listar locações\n");
        printf("7 - Dar baixa em uma locação\n");
        printf("999 - SAIR\n");
        printf("\n>>> ");

        char str_f[10];
        fgets(str_f, 10, stdin);
        selecao = strtof(str_f, NULL);
        clearScreen();
        switch (selecao)
        {
        case 999: // stopProgram
            runnig = 0;
            break;
        case 0:
            // printf(">>> ");
            break;
        case 1: // screenRegisterClient
            printf(">>> screenRegisterClient %d\n\n", selecao);
            service.registerClient();
            break;
        case 2: // screenRegisterVehicle
            printf(">>> screenRegisterVehicle %d\n\n", selecao);
            service.registerVehicle();
            break;
        case 3: // registerLocation
            printf(">>> registerLocation %d\n\n", selecao);
            service.registerLocation();
            printf("\n\n");
            break;
        case 4: // listar clientes
            printf(">>> listar clientes %d\n\n", selecao);
            service.listClients();
            printf("\n");
            break;
        case 5: // listar veiculos
            printf(">>> listar veiculos %d\n\n", selecao);
            service.listVehicles();
            printf("\n");
            break;
        case 6: // listar locações
            printf(">>> Listar locações %d\n\n", selecao);
            service.listLocations();
            printf("\n");
            break;
        case 7: // baixa em locação
            printf(">>> Dar baixa em uma locação %d\n\n", selecao);
            service.endLocation();
            printf("\n");
            break;
        default:
            printf(">>> Opção inválida - Selecionado %d\n", selecao);
            break;
        }
    }

    printf("Até mais! Programa encerrado.");
    return 0;
}

void clearScreen()
{
    printf("\e[1;1H\e[2J");
}