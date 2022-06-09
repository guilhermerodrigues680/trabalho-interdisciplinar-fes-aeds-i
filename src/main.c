#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.c"

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
