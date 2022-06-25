/* core_test.c
 *
 * Este arquivo representa a implementação da suite de testes do sistema.
 * Os testes são realizados usando o munit.
 * Exemplo: https://github.com/nemequ/munit/blob/fbbdf1467eb0d04a6ee465def2e529e4c87f2118/example.c
 *
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "core_test.h"
#include "core.h"
#include "client.h"
#include "lease.h"
#include "vehicle.h"
#include "utils.h"
#include "munit.h"

/* Função de Tests. */
static MunitResult test_compare(const MunitParameter params[], void *data)
{
    const time_t epochWithdrawalDate = 1641034800L; // 1 de janeiro de 2022 às 08:00:00 GMT-03:00
    const time_t epochReturnDate = 1641294000L;     // 4 de janeiro de 2022 às 08:00:00 GMT-03:00

    /* Não executa nada, apenas silencia os avisos do compilador
    sobre os parâmetros sendo não utilizado. */
    (void)params;
    (void)data;

    printf("Rodando testes...\n");

    // Cadastra o cliente, primeiro cliente -> Cod: 0
    munit_assert_false(core_register_client("client", "rua 123, carlos prates, BH"));
    // Cadastra o veiculo, primeiro veiculo -> Cod: 0
    munit_assert_false(core_register_vehicle("demo", "demo", "black", "aaaa-0000", 10.0, 6));
    // Cadastra locação, primeira locação -> Cod: 0
    munit_assert_false(core_register_lease(epochWithdrawalDate, epochReturnDate, 1, 0, 0));

    // munit_assert_double_equal(3.141592654, 3.141592653589793, 9);
    return MUNIT_OK;
}

/* Função de configuração, essa função é executada antes de iniciar os testes.
 * É útil para preparar o ambiente. O retorno é passado como argumento para
 * a função de teste */
static void *test_compare_setup(const MunitParameter params[], void *user_data)
{
    (void)params;

    printf("\nPreparando ambiente de testes...\n");

    // Altera o path dos arquivos de dados
    clients_db_file = "_db_clients_test.dat";
    lease_db_file = "_db_lease_test.dat";
    vehicle_db_file = "_db_vehicles_test.dat";
    // Cria/Limpa os arquivos de dados de teste
    FILE *fc = fopen(clients_db_file, "w");
    FILE *fl = fopen(lease_db_file, "w");
    FILE *fv = fopen(vehicle_db_file, "w");
    fclose(fc);
    fclose(fl);
    fclose(fv);

    munit_assert_string_equal(user_data, "µnit");
    return (void *)(uintptr_t)0xdeadbeef;
}

/* Função para limpar após testes, essa função é executada após a execução dos testes.
 * É útil para limpar as alterações realizadas no ambiente. O argumento `fixture` é
 * o valor retornado pela funão de configuração */
static void test_compare_tear_down(void *fixture)
{
    printf("Limpando ambiente de testes...\n");
    // Altera o path dos arquivos de dados
    munit_assert_false(remove("_db_clients_test.dat"));
    munit_assert_false(remove("_db_vehicles_test.dat"));
    munit_assert_false(remove("_db_lease_test.dat"));
    munit_assert_ptr_equal(fixture, (void *)(uintptr_t)0xdeadbeef);
}

static MunitTest test_suite_tests[] = {
    // Teste 1
    {(char *)"/teste-cadastro-locacao",
     test_compare,
     test_compare_setup,
     test_compare_tear_down,
     MUNIT_TEST_OPTION_NONE,
     NULL},
    // Teste 2
    // {
    //     .name = (char *)"/teste-cadastro-locacao",
    //     .test = test_compare,
    //     .options = MUNIT_TEST_OPTION_NONE,
    //     .parameters = NULL,
    // },
};

static const MunitSuite test_suite = {
    (char *)"guilherme -> ",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE};

/* Executa os testes no programa e finaliza a execução. */
void core_test_run_tests(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    // https://github.com/nemequ/munit/blob/fbbdf1467eb0d04a6ee465def2e529e4c87f2118/example.c
    printf("[*] Iniciando testes\n");
    int testsExitCode = munit_suite_main(&test_suite, (void *)"µnit", argc, argv);
    printf("[*] Testes finalizados.\n");
    printf("[*] EXIT CODE: %d\n\n", testsExitCode);
    // exit(testsExitCode);
}
