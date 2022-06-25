/* main.c
 *
 * Este arquivo representa o inicio do programa.
 *
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app.h"
#include "munit.h"
#include "core_test.h"
#include "utils.h"

// programa principal
int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
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
        printf("998 - Executar testes\n");
        printf("999 - SAIR\n");
        printf("\n>>> ");

        char str_f[10];
        fgets(str_f, 10, stdin);
        selecao = strtof(str_f, NULL);
        utils_clear_screen();
        switch (selecao)
        {
        case 998: // rodar testes
            printf(">>> service_test_run_tests %d\n\n", selecao);
            core_test_run_tests(argc, argv);
            runnig = 0;
            break;
        case 999: // stopProgram
            runnig = 0;
            break;
        case 0:
            // printf(">>> ");
            break;
        case 1: // screenRegisterClient
            printf(">>> screenRegisterClient %d\n\n", selecao);
            app_register_client();
            printf("\n\n");
            break;
        case 2: // screenRegisterVehicle
            printf(">>> screenRegisterVehicle %d\n\n", selecao);
            app_register_vehicle();
            printf("\n\n");
            break;
        case 3: // registerLocation
            printf(">>> registerLocation %d\n\n", selecao);
            app_register_lease();
            printf("\n\n");
            break;
        case 4: // listar clientes
            printf(">>> listar clientes %d\n\n", selecao);
            app_list_clients();
            printf("\n");
            break;
        case 5: // listar veiculos
            printf(">>> listar veiculos %d\n\n", selecao);
            app_list_vehicles();
            printf("\n");
            break;
        case 6: // listar locações
            printf(">>> Listar locações %d\n\n", selecao);
            app_list_leases();
            printf("\n");
            break;
        case 7: // baixa em locação
            printf(">>> Dar baixa em uma locação %d\n\n", selecao);
            app_finalize_lease();
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
