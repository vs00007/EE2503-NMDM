#include <stdio.h>
#include <include/linalg.h>
#include <include/poisson.h>

int main()
{
// if compiled for testing then run tests.
#ifdef RUN_TESTS
#include "test/test.h"
    run_all_tests();
    return 0;
#endif

/*
* TODO List and Implementation Status:
* ---------------------------------
* 1. Input Handler (File Manager) - TODO
*    - Read from data/input_params.txt
*    - Create include/inputs.h
*    - Implement src/input.c
*    - Add validation and error handling

* 2. Interpolation Module - TODO
*    - Interpolate potential V between grid points
*    - Required for continuous potential profile
*    - Methods: Lagrange

* 3. Steady State Solver - TODO
*    - Solve df/dt = 0 (Master equation)
*    - Handle boundary conditions
*    - Implement convergence criteria

* 4. Initial Value Problem - TODO
*    - Implement Master equation solver
*    - Time evolution of trap occupations
*    - Handle initial conditions

* 5. Coefficient Generator - TODO
*    - Generate rates and coefficients
*    - Physical parameters lookup
*    - Temperature dependence

* 6. Pseudo Poisson solver - DONE
*    - Check docs (design.md) for info.
*/
}
