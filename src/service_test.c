#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "service_test.h"
#include "client.h"
#include "location.h"
#include "vehicle.h"
#include "utils.h"
#include "munit.h"

/* Tests are functions that return void, and take a single void*
 * parameter.  We'll get to what that parameter is later. */
static MunitResult test_compare(const MunitParameter params[], void *data)
{
    printf("Rodando testes...\n");

    time_t epochWithdrawalDate = 1641034800L; // 1 de janeiro de 2022 às 08:00:00 GMT-03:00
    time_t epochReturnDate = 1641294000L;     // 4 de janeiro de 2022 às 08:00:00 GMT-03:00
    locationRepo.registerLocation(
        epochWithdrawalDate,
        epochReturnDate,
        1,
        0,
        0);

    /* We'll use these later */
    const unsigned char val_uchar = 'b';
    const short val_short = 1729;
    double pi = 3.141592654;
    char *stewardesses = "stewardesses";
    char *most_fun_word_to_type;

    /* These are just to silence compiler warnings about the parameters
     * being unused. */
    (void)params;
    (void)data;

    /* Let's start with the basics. */
    munit_assert(0 != 1);

    /* There is also the more verbose, though slightly more descriptive
       munit_assert_true/false: */
    munit_assert_false(0);

    /* You can also call munit_error and munit_errorf yourself.  We
     * won't do it is used to indicate a failure, but here is what it
     * would look like: */
    /* munit_error("FAIL"); */
    /* munit_errorf("Goodbye, cruel %s", "world"); */

    /* There are macros for comparing lots of types. */
    munit_assert_char('a', ==, 'a');

    /* Sure, you could just assert('a' == 'a'), but if you did that, a
     * failed assertion would just say something like "assertion failed:
     * val_uchar == 'b'".  µnit will tell you the actual values, so a
     * failure here would result in something like "assertion failed:
     * val_uchar == 'b' ('X' == 'b')." */
    munit_assert_uchar(val_uchar, ==, 'b');

    /* Obviously we can handle values larger than 'char' and 'uchar'.
     * There are versions for char, short, int, long, long long,
     * int8/16/32/64_t, as well as the unsigned versions of them all. */
    munit_assert_short(42, <, val_short);

    /* There is also support for size_t.
     *
     * The longest word in English without repeating any letters is
     * "uncopyrightables", which has uncopyrightable (and
     * dermatoglyphics, which is the study of fingerprints) beat by a
     * character */
    munit_assert_size(strlen("uncopyrightables"), >, strlen("dermatoglyphics"));

    /* Of course there is also support for doubles and floats. */
    munit_assert_double(pi, ==, 3.141592654);

    /* If you want to compare two doubles for equality, you might want
     * to consider using munit_assert_double_equal.  It compares two
     * doubles for equality within a precison of 1.0 x 10^-(precision).
     * Note that precision (the third argument to the macro) needs to be
     * fully evaluated to an integer by the preprocessor so µnit doesn't
     * have to depend pow, which is often in libm not libc. */
    munit_assert_double_equal(3.141592654, 3.141592653589793, 9);

    /* And if you want to check strings for equality (or inequality),
     * there is munit_assert_string_equal/not_equal.
     *
     * "stewardesses" is the longest word you can type on a QWERTY
     * keyboard with only one hand, which makes it loads of fun to type.
     * If I'm going to have to type a string repeatedly, let's make it a
     * good one! */
    munit_assert_string_equal(stewardesses, "stewardesses");

    /* A personal favorite macro which is fantastic if you're working
     * with binary data, is the one which naïvely checks two blobs of
     * memory for equality.  If this fails it will tell you the offset
     * of the first differing byte. */
    munit_assert_memory_equal(7, stewardesses, "steward");

    /* You can also make sure that two blobs differ *somewhere*: */
    munit_assert_memory_not_equal(8, stewardesses, "steward");

    /* There are equal/not_equal macros for pointers, too: */
    most_fun_word_to_type = stewardesses;
    munit_assert_ptr_equal(most_fun_word_to_type, stewardesses);

    /* And null/not_null */
    munit_assert_null(NULL);
    munit_assert_not_null(most_fun_word_to_type);

    /* Lets verify that the data parameter is what we expected.  We'll
     * see where this comes from in a bit.
     *
     * Note that the casting isn't usually required; if you give this
     * function a real pointer (instead of a number like 0xdeadbeef) it
     * would work as expected. */
    munit_assert_ptr_equal(data, (void *)(uintptr_t)0xdeadbeef);

    return MUNIT_OK;
}

/* The setup function, if you provide one, for a test will be run
 * before the test, and the return value will be passed as the sole
 * parameter to the test function. */
static void *test_compare_setup(const MunitParameter params[], void *user_data)
{
    (void)params;

    printf("\nPreparando ambiente de testes...\n");

    clientsDbFile = "clients_db_test.dat";
    locationDbFile = "leases_db_test.dat";
    vehicleDbFile = "vehicles_db_test.dat";
    FILE *fc = fopen(clientsDbFile, "w");
    FILE *fl = fopen(locationDbFile, "w");
    FILE *fv = fopen(vehicleDbFile, "w");
    fclose(fc);
    fclose(fl);
    fclose(fv);

    munit_assert_string_equal(user_data, "µnit");
    return (void *)(uintptr_t)0xdeadbeef;
}

/* To clean up after a test, you can use a tear down function.  The
 * fixture argument is the value returned by the setup function
 * above. */
static void test_compare_tear_down(void *fixture)
{
    printf("Limpando ambiente de testes...\n");

    munit_assert_false(remove("clients_db_test.dat"));
    munit_assert_false(remove("leases_db_test.dat"));
    munit_assert_false(remove("vehicles_db_test.dat"));
    munit_assert_ptr_equal(fixture, (void *)(uintptr_t)0xdeadbeef);
}

static MunitTest test_suite_tests[] = {
    {(char *)"/teste-cadastro-locacao",
     test_compare,
     test_compare_setup,
     test_compare_tear_down,
     MUNIT_TEST_OPTION_NONE,
     NULL},
    {(char *)"/teste-cadastro-locacao",
     test_compare,
     test_compare_setup,
     test_compare_tear_down,
     MUNIT_TEST_OPTION_NONE,
     NULL},
};

static const MunitSuite test_suite = {
    (char *)"guilherme -> ",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE};

/* Executa os testes no programa e finaliza a execução. */
void serviceRunTests(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    // https://github.com/nemequ/munit/blob/fbbdf1467eb0d04a6ee465def2e529e4c87f2118/example.c
    printf("[*] Iniciando testes\n");
    int testsExitCode = munit_suite_main(&test_suite, (void *)"µnit", argc, argv);
    printf("[*] Testes finalizados.\n");
    printf("[*] EXIT CODE: %d\n\n", testsExitCode);
    exit(testsExitCode);
}
