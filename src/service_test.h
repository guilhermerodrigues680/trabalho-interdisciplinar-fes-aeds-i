/* service_test.h
 *
 * Este arquivo representa as declarações de funções, macros e tipos do service_test.c.
 *
 *********************************************************************/

#ifndef HEADER_SERVICE_TEST_H
#define HEADER_SERVICE_TEST_H

#include "munit.h"

// Prototipos
void service_test_run_tests(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]);

#endif