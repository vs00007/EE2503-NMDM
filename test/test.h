#include <stdio.h>
#include "include/linalg.h"
#include "test/linalg/linalg.h"

int run_all_tests()
{
    printf("Running tests:\n");
    linalg_vec_test();
    linalg_matrix_test();
    return 0;
}

