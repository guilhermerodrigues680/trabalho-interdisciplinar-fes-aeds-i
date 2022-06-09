#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.c"
#include "vehicle.c"

// programa principal
int main(void)
{
    int runnig = 1;
    int selecao = -1;

    printf("============== LocaMais ==================\n");
    printf("====== A melhor solução em locação! ======\n\n");
    while (runnig)
    {
        if (selecao != 0)
        {
            printf("## MENU ##\n");
            printf("1 - Cadastrar cliente\n");
            printf("2 - Cadastrar veiculo\n");
            printf("3 - Cadastrar locação\n");
            printf("4 - Listar clientes\n");
            printf("5 - Listar veiculos\n");
            printf("999 - SAIR\n");
            printf("\n>>> ");
        }

        char str_f[10];
        fgets(str_f, 10, stdin);
        selecao = strtof(str_f, NULL);
        switch (selecao)
        {
        case 999: // stopProgram
            runnig = 0;
            break;
        case 0:
            printf(">>> ");
            break;
        case 1: // screenRegisterClient
            printf(">>> screenRegisterClient %d\n\n", selecao);
            registerClient();
            break;
        case 2: // screenRegisterVehicle
            printf(">>> screenRegisterVehicle %d\n\n", selecao);
            registerVehicle();
            break;
            break;
        case 4: // listar clientes
            printf(">>> listar clientes %d\n\n", selecao);
            listClients();
            printf("\n");
            break;
        case 5: // listar veiculos
            printf(">>> listar veiculos %d\n\n", selecao);
            listVehicles();
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
