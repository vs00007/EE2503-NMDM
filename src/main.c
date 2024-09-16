#include <stdio.h>
#include <include/linalg.h>

int main()
{
// if compiled for testing then run tests.
#ifdef RUN_TESTS
#include "test/test.h"
    run_all_tests();
    return 0;
#endif

    // 

}
