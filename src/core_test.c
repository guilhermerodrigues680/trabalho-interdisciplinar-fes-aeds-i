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
static MunitResult test_finalize_lease(const MunitParameter params[], void *data)
{
    const time_t epochWithdrawalDate = 1641034800L; // 1 de janeiro de 2022 às 08:00:00 GMT-03:00
    const time_t epochReturnDate = 1641294000L;     // 4 de janeiro de 2022 às 08:00:00 GMT-03:00

    /* Não executa nada, apenas silencia os avisos do compilador
    sobre os parâmetros sendo não utilizado. */
    (void)params;
    (void)data;

    // Prepara ambiente de testes

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

    // Executa Testes

    // Testa o user_data parameter do munit_suite_main
    munit_assert_string_equal(data, "µnit");
    // Cadastra o cliente, primeiro cliente -> Cod: 0
    munit_assert_false(core_register_client("client", "rua 123, carlos prates, BH"));
    // Cadastra o veiculo, primeiro veiculo -> Cod: 0
    munit_assert_false(core_register_vehicle("demo", "demo", "black", "aaaa-0000", 10.0, 6));
    // Cadastra locação, primeira locação -> Cod: 0
    munit_assert_false(core_register_lease(epochWithdrawalDate, epochReturnDate, 1, 0, 0));
    // Finaliza a locação
    munit_assert_false(core_finalize_lease(0, epochReturnDate));

    // munit_assert_double_equal(3.141592654, 3.141592653589793, 9);

    // Limpa ambiente de testes

    // Altera o path dos arquivos de dados
    munit_assert_false(remove("_db_clients_test.dat"));
    munit_assert_false(remove("_db_vehicles_test.dat"));
    munit_assert_false(remove("_db_lease_test.dat"));

    return MUNIT_OK;
}

static MunitResult test_vehicle_registration(const MunitParameter params[], void *data)
{
    char *original_vehicle_db_file = vehicle_db_file;

    (void)params;
    (void)data;

    // Prepara ambiente de testes

    // Altera o path do arquivo de dados
    vehicle_db_file = "_db_vehicles_test.dat";
    // Cria/limpa o arquivos de dados de teste
    FILE *f_ptr = fopen(vehicle_db_file, "w");
    munit_assert_not_null(f_ptr);
    munit_assert_false(fclose(f_ptr));

    // Executa Testes

    // Cadastra o veiculo, primeiro veiculo -> Cod: 0
    munit_assert_false(core_register_vehicle("demo", "demo", "black", "aaaa-0000", 10.0, 6));

    // Limpa ambiente de testes

    // Remove os arquivo após o teste
    munit_assert_false(remove("_db_vehicles_test.dat"));
    // Volta o path do arquivo de dados para path original
    vehicle_db_file = original_vehicle_db_file;

    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    // TESTE 1
    {(char *)"/serviço/baixa-em-locação",
     test_finalize_lease,
     NULL,
     NULL,
     MUNIT_TEST_OPTION_NONE,
     NULL},
    // TESTE 2
    {(char *)"/cadastro/veiculo",
     test_vehicle_registration,
     NULL,
     NULL,
     MUNIT_TEST_OPTION_NONE,
     NULL},
    /* FIM DOS TESTES
     * Para informar ao `test runner` que os testes acabaram,
     * é necessário adicionar uma entrada `NULL` no final.
     * Esquecer desse detalhe pode resultar no erro:
     * `child killed by signal 11 (Segmentation fault)` */
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
};

static const MunitSuite test_suite = {
    (char *)"LocaMais Testes -> ",
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
