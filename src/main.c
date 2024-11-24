#include <include/inc.h>

int main()
{
// if compiled for testing then run tests.
#ifdef RUN_TESTS
#include "test/test.h"
    run_all_tests();
    return 0;
#endif
    InputData data = getInput("data/input-params.toml");
    Vec mesh = generateMesh(data.locs, data.params);
    // printNL();
    // vecPrint(mesh);

    PyVi vis = pyviInitA("data/visualise.pyvi");
    PyViBase x_vi = pyviCreateParameter(&vis, "d", data.locs);
    PyViSec  f_n  = pyviCreateSection(&vis, "f_n", x_vi);
    
    PyViBase meshvi = pyviCreateParameter(&vis, "mesh", mesh);
    PyViSec  V_vi   = pyviCreateSection(&vis, "Voltage", meshvi);

    Mat2d R = R_en(data, mesh);
    Vec R1 = mat2DCol(R, 0);
    Vec R2 = mat2DCol(R, 1);

    size_t dim = data.locs.len;

    Mat2d E_nm = matrix_E_n(data, mesh);
    Mat2d d_nm = matrix_d_nm(data);
    Mat2d coefficientMatrix = matrix_r_nm(data, E_nm, d_nm);

    Vec delta_fn = vecInitZerosA(dim);
    Mat2d delta_E = mat2DInitZerosA(dim, dim);
    Vec V = poissonWrapper(data, mesh);
    for(size_t iter = 0; iter < ITER_MAX; iter++)
    {
        V = poissonWrapper(data, mesh);
        pyviSectionPush(V_vi, V);

        // set to prev iter values
        delta_fn = data.probs;
        delta_E = E_nm;

        // solve for fn
        data.probs = jacobianImplementationA(coefficientMatrix, R1, R2);
        E_nm = matrix_E_n(data, mesh);
        
        // d_m doesn't change?
        //d_nm = matrix_d_nm(data);
        
        coefficientMatrix = matrix_r_nm(data, E_nm, d_nm);
        
        mat2DSub(delta_E, E_nm, &delta_E);
        vecSub(delta_fn, data.probs, &delta_fn);

        if(mat2DContainsNan(E_nm))
        {
            printf("[Steady-State] Error: E_nm contains nan!\n");
            break;
        }
        if(vecContainsNan(delta_fn))
        {
            printf("[Steady-State] Error: f_n contains nan!\n");
            break;
        }

        double error_fn = vecNorm(delta_fn, 2) / vecNorm(data.probs, 2);
        double error_E = mat2DMaxAbs(delta_E) / mat2DMaxAbs(E_nm);

        if (error_E < TOL && error_fn < TOL)
        {
            printf("Successful! Iter count : %zu\n", iter);
            break;
        }

        pyviSectionPush(f_n, data.probs);

        if (iter == ITER_MAX - 1) printf("Max Iterations reached!\n");
    }
    pyviWrite(vis);
    printNL();
    vecPrint(data.probs);

}
/*
* TODO List:
* ---------------------------------
* 1. Input Handler (File Manager) - DONE - 5/11/2024
*    - Read from data/input_params.txt
*    - Create include/inputs.h
*    - Implement src/input.c
*    - Add validation and error handling

* 2. Interpolation Module - DONE - 16/11/2024
*    - Interpolate potential V between grid points
*    - Required for continuous potential profile
*    - Methods: Lagrange

* 3. Steady State Solver - DONE - 22/11/2024
*    - Solve df/dt = 0 (Master equation)
*    - Handle boundary conditions
*    - Implement convergence criteria

* 4. Initial Value Problem - DONE - 13/11/2024
*    - Implement Master equation solver
*    - Time evolution of trap occupations
*    - Handle initial conditions

* 5. Coefficient Generator - DONE - 15/11/2024
*    - Generate rates and coefficients
*    - Physical parameters lookup
*    - Temperature dependence

* 6. Poisson solver - DONE - 14/11/2024
*    - Check docs (design.md) for info.
*/