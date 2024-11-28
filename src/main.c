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

    long double v_0_actual = data.params.V_0;
    long double v_L_actual = data.params.V_L;
    printInputData(&data);

    printNL();

    data.params.V_0 = 0;
    data.params.V_L = 0;

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

    Vec delta_fn = vecInitZerosA(dim);
    Vec prev_fn = vecInitZerosA(dim);
    Mat2d delta_E = mat2DInitZerosA(dim, dim);
    Vec V;
    long double w = 0.05L;
    for(size_t iter = 0; iter < ITER_MAX; iter++)
    {
        // set to prev iter values
        prev_fn = data.probs;
        delta_fn = vecCopyA(prev_fn);
        delta_E = E_nm;

        // solve for fn
        data.probs = jacobianImplementationA(coefficientMatrix, R1, R2);
        vecScale(w, data.probs, &data.probs);
        vecScale(1L - w, prev_fn, &prev_fn);
        vecAdd(prev_fn, data.probs, &data.probs);
        V = poissonWrapper(data, mesh);

        // vecPrint(V);
        pyviSectionPush(V_vi, V);

        // printf("\nProbabilites[%zu]:", iter);
        // vecPrint(data.probs);
        // printNL();

        E_nm = matrix_E_n(data, mesh);
        
        // printf("\nEnergies[%zu]:", iter);
        // mat2DPrint(E_nm);
        // printNL();

        R = R_en(data, mesh);
        R1 = mat2DCol(R, 0);
        R2 = mat2DCol(R, 1);

        // printf("Transmission Coefficients[%zu]:", iter);
        // mat2DPrint(R);
        // printf("\n");

        coefficientMatrix = matrix_r_nm(data, E_nm, d_nm);
        for(size_t i = 0; i < coefficientMatrix.rows; i++) *mat2DRef(coefficientMatrix, i, i) = 0.0L;
        
        // printf("\nCoeffmatrix[%zu]:", iter);
        // mat2DPrint(coefficientMatrix);
        // printNL();

        // printf("\ndelta distance[%zu]:", iter);
        // mat2DPrint(d_nm);
        // printNL();

        // d_m doesn't change? - - Of course it doesn't
        //d_nm = matrix_d_nm(data);
        
        mat2DSub(delta_E, E_nm, &delta_E);
        vecSub(delta_fn, data.probs, &delta_fn);

        long double error_fn = vecMaxAbs(delta_fn) / vecMaxAbs(data.probs);
        long double error_E = mat2DMaxAbs(delta_E) / mat2DMaxAbs(E_nm);

        printf("Iteration[%4zu] Errors: Energy:%-25.17Lg Probability: %-25.3Lg\n", iter, error_E, error_fn);

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

            printf("YAY! Steady state converged ... in ... Iter count : %zu\n", iter);
            break;
        }

        if (iter == ITER_MAX - 1) printf("Max Iterations reached!\n");
    }

    pyviWrite(vis);
    printNL();

    data.params.V_0 = v_0_actual;
    data.params.V_L = v_L_actual;

    RK45Config config;
    config.h = 1e-10;
    config.t_initial = 0.0;
    config.t_final = 1e7;
    config.tol = 1e-12;
    config.data = data;
    config.mesh = mesh;
    config.y_initial = data.probs;

    Vec timestamps = vecInitZerosA(1000);
    Mat2d fn_t = mat2DInitZerosA(data.probs.len, 1000);

    solver(config, timestamps, fn_t);
    // mat2DPrint(fn_t);
    printNL();
    size_t slen = 0;    
    for (size_t i = 1; i < timestamps.len; i ++)
    {
        if (vecGet(timestamps, i) > vecGet(timestamps, i - 1)) slen = i;
    }

    // TODO: change this temp
    timestamps = vecConstruct(timestamps.x, slen+1);

    PyVi trans_pyvi = pyviInitA("data/transient.pyvi");
    PyViBase x_pyvi = pyviCreateParameter(&trans_pyvi, "t", timestamps);
    
    for(size_t i = 0; i < data.probs.len; i++)
    {
        char* buf = malloc(32*sizeof(char));
        snprintf(buf, 32, "f_n[%zu]", i); // Scanf reads from buf, my guy. You need to print to it
        PyViSec f_n_pyvi = pyviCreateSection(&trans_pyvi, buf, x_pyvi);

        Vec sec = mat2DRow(fn_t, i);
        sec.len = slen+1;
        pyviSectionPush(f_n_pyvi, sec);
    }
    V = poissonWrapper(data, mesh);
    PyViBase meshvis = pyviCreateParameter(&trans_pyvi, "mesh", mesh);
    PyViSec  V_vis   = pyviCreateSection(&trans_pyvi, "Voltage", meshvis);

    pyviSectionPush(V_vis, V);
    pyviWrite(trans_pyvi);

    PyVi evolve_pyvi = pyviInitA("data/transient.pyvi");
    PyViBase evol_traps_pyvi = pyviCreateParameter(&evolve_pyvi, "x-traps", data.locs);
    PyViBase evol_mesh_pyvi = pyviCreateParameter(&evolve_pyvi, "x-mesh", mesh);
    PyViSec f_n_evol_pyvi = pyviCreateSection(&evolve_pyvi, "f_n", evol_traps_pyvi);
    PyViSec V_evol_pyvi = pyviCreateSection(&evolve_pyvi, "V", evol_mesh_pyvi);

    for(size_t i = 0; i < slen+1; i++)
    {
        Vec sec = mat2DCol(fn_t, i);
        pyviSectionPush(f_n_evol_pyvi, sec);
        data.probs = sec;
        V = poissonWrapper(data, mesh);
        pyviSectionPush(V_evol_pyvi, V);
    }

    pyviWrite(evolve_pyvi);

    freePyVi(&vis);
    freePyVi(&trans_pyvi);
    freePyVi(&evolve_pyvi);

    return 0;
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