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
    printInputData(&data);
    Vec mesh = generateMesh(data.locs, data.params);
    // printNL();
    // printf("V_top = %Lg\n", data.params.V_L);

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

    for(size_t i = 0; i < coefficientMatrix.rows; i++) *mat2DRef(coefficientMatrix, i, i) = 0.0L;

    // q1: What are R1 and R2??
    /*
        Mihir, I added code for transient analysis. Check if it is working once.
        (the solver function needs vec and mat, but the vec and mat dimensions cannot be figured out beforehand)
        (try not to change the master_eqn.c file, just make realllyyy big vector and matrix, and then push it to a new pyvi file) 
    */

    Vec delta_fn = vecInitZerosA(dim);
    Mat2d delta_E = mat2DInitZerosA(dim, dim);
    Vec V;
    for(size_t iter = 0; iter < ITER_MAX; iter++)
    {
        // set to prev iter values
        delta_fn = data.probs;
        delta_E = E_nm;

        // solve for fn
        data.probs = jacobianImplementationA(coefficientMatrix, R1, R2);
        V = poissonWrapper(data, mesh);

        vecPrint(V);
        pyviSectionPush(V_vi, V);

        printf("\nProbabilites[%zu]:", iter);
        vecPrint(data.probs);
        // printNL();

        E_nm = matrix_E_n(data, mesh);
        
        printf("\nEnergies[%zu]:", iter);
        mat2DPrint(E_nm);
        printNL();

        R = R_en(data, mesh);
        R1 = mat2DCol(R, 0);
        R2 = mat2DCol(R, 1);

        printf("Transmission Coefficients[%zu]:", iter);
        mat2DPrint(R);
        printf("\n");

        coefficientMatrix = matrix_r_nm(data, E_nm, d_nm);
        for(size_t i = 0; i < coefficientMatrix.rows; i++) *mat2DRef(coefficientMatrix, i, i) = 0.0L;
        
        printf("\nCoeffmatrix[%zu]:", iter);
        mat2DPrint(coefficientMatrix);
        printNL();

        printf("\ndelta distance[%zu]:", iter);
        mat2DPrint(d_nm);
        printNL();

        // d_m doesn't change? - - Of course it doesn't
        //d_nm = matrix_d_nm(data);
        
        mat2DSub(delta_E, E_nm, &delta_E);
        vecSub(delta_fn, data.probs, &delta_fn);

        long double error_fn = vecMax(delta_fn) / vecMax(data.probs);
        long double error_E = mat2DMaxAbs(delta_E) / mat2DMaxAbs(E_nm);

        pyviSectionPush(f_n, data.probs);

        if(mat2DContainsNan(E_nm))
        {
            printf("[Steady-State] Error: E_nm contains nan! Iter : %zu\n", iter);
            break;
        }
        if(vecContainsNan(delta_fn))
        {
            printf("[Steady-State] Error: f_n contains nan! Iter : %zu\n", iter);
            break;
        }

        if (error_E < TOL && error_fn < TOL)
        {

            // printf("YAY! Steady state converged ... in ... Iter count : %zu\n", iter);
            // break;
        }

        if (iter == ITER_MAX - 1) printf("Max Iterations reached!\n");
    }

    pyviWrite(vis);
    printNL();
/*
    data.params.V_0 = 1.0;

    RK45Config config;
    config.h = 1e-6;
    config.t_initial = 0.0;
    config.t_final = 1.0;
    config.tol = 1e-6;
    config.data = data;
    config.mesh = mesh;
    config.y_initial = data.probs;

    Vec timestamps = vecInitZerosA(100);
    Mat2d fn_t = mat2DInitZerosA(data.probs.len, 100);

    solver(config, timestamps, fn_t);

    size_t slen = 0;
    for (size_t i = 1; i < timestamps.len; i ++)
    {
        if (vecGet(timestamps, i) < vecGet(timestamps, i - 1)) slen = i; 
    } 

    PyVi trans_pyvi = pyviInitA("data/transient.pyvi");
    PyViBase x_pyvi = pyviCreateParameter(&trans_pyvi, "x", data.locs);
    
    for(size_t i = 0; i < data.probs.len; i++)
    {
        char buf[16];
        sscanf(buf, "f_n[%zu]", i);
        PyViSec f_n_pyvi = pyviCreateSection(&trans_pyvi, buf, x_pyvi);

        for(size_t j = 0; j < slen; j++)
        {
            //pyviSectionPush(f_n_pyvi, );
        }
    }
 */
    freePyVi(&vis);
    // int status = system("python3 visualise/visualise.py");
}
/*
* TODO List:
* ---------------------------------
* 1. Input Handler (File Manager) - DONE - 5/11/2024

* 2. Interpolation Module - DONE - 16/11/2024

* 3. Steady State Solver - DONE - 22/11/2024

* 4. Initial Value Problem - DONE - 13/11/2024

* 5. Coefficient Generator - DONE - 15/11/2024

* 6. Poisson solver - DONE - 14/11/2024
*/