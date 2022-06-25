/* core_test.h
 *
 * Este arquivo representa as declarações de funções, macros e tipos do core_test.c.
 *
 *********************************************************************/

#ifndef HEADER_CORE_TEST_H
#define HEADER_CORE_TEST_H

#include "munit.h"

void core_test_run_tests(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]);

#endif