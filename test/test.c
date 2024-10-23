#include <stdio.h>
#include "include/linalg.h"
#include "test/linalg/linalg.h"
#include <include/poisson.h>
#include <test/test.h>
#include <test/poisson/test_poisson.h>

int run_all_tests()
{
    printf("Running tests:\n");
    linalg_vec_test();
    linalg_matrix_test();
    test_poisson();
    test_getGridV();
    return 0;
}

