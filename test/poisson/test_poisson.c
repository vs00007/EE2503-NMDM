#include <include/linalg.h>
#include <include/poisson.h>
#include <assert.h>
#include <test/poisson/test_poisson.h>
#include <time.h>
#include <pyvisual.h>

#define LA_VIDX(vector, index) *(vector.x + vector.offset * index)
#define EPSILON 1e-6
#define NUM_TESTS 100

int comparelongdoubles(long double a, long double b, long double epsilon)
{
    return fabsl(a - b) < epsilon;
}

int qsort_compare(const void *a, const void *b) {
    return (*(long double*)a > *(long double*)b) - (*(long double*)a < *(long double*)b);
}

void randF(Vec param, long double min, long double max, int sort)
{
    srand(time(NULL));
    if (param.len == 0)
    {
        printf("ERROR: Length of input vector is 0.\n");
        return;
    }
    
    for (size_t i = 0; i < param.len; i++)
    {
        param.x[i] = min + (max - min) * ((long double)rand() / RAND_MAX);
    }

    if (sort)
    {
        qsort(param.x, param.len, sizeof(long double), qsort_compare);
    }
}

Vec getV(size_t len)
{
    Vec f_n = vecInitZerosA(len);
    randF(f_n, 1e-9, 1.0, 0);

    Vec d = vecInitZerosA(len);
    randF(d, 1e-9, 1e-6, 1);

    Vec result = vecInitZerosA(len);

    Vec sample_x = vecInitZerosA(len);

    for (size_t i = 0; i < len; i ++)
    {
        sample_x.x[i] = 1e-9 + (1e-6 - 1e-9) * i / len;
        result.x[i] = poissonSolveAnalytical(f_n, d, sample_x.x[i]);
    }


    printf("Charges: ");
    printVecUnits(f_n, 'q');

    printf("Distances: ");
    printVecUnits(d, 'm');

    printf("Resultant Voltage: ");
    printVecUnits(result, 'V');

    return result;

    freeVec(&f_n);
    freeVec(&d);
}

void test_poisson()
{
    int test_passed = 0;
    int total_tests = 0;

    printf("\n-----------Poisson Solver Tests-----------\n");
    // Test case 1: 
    {
        Vec f_n = vecInitA(0.1, 4);
        Vec d = vecInitA(0.1, 4);

        printf("Test case 1 - Original test:\n");
        printf("Charges are : ");
        printVecUnits(f_n, 'q');
        printf("Distances are : ");
        printVecUnits(d, 'm');

        long double x = 0.2;
        long double sol = poissonSolveAnalytical(f_n, d, x);

        printf("Exact solution : V(%Lfm) = %Le\n", x, sol);

        
        long double expected = Q * K * (1.0/1.0 + 1.0/1.0 + 1.0/1.0 + 1.0/1.0); 
        total_tests++;
        if (comparelongdoubles(sol, expected, EPSILON)) {
            printf("Expected %Le, got %Le. Test case 1 passed.\n", expected, sol);
            test_passed++;
        } else {
            printf("Test case 1 failed. Expected %Le, got %Le\n", expected, sol);
        }

        freeVec(&f_n);
        freeVec(&d);
    }

    // Test case 2: Single charge
    {
        Vec f_n = vecInitA(1.0, 1);
        Vec d = vecInitA(1.0, 1);

        printf("\nTest case 2 - Single charge:\n");
        printf("Charge: ");
        printVecUnits(f_n, 'q');
        printf("Distance: ");
        printVecUnits(d, 'm');

        long double x = 2.0;
        long double sol = poissonSolveAnalytical(f_n, d, x);

        printf("Solution: V(%Lfm) = %Le\n", x, sol);


        long double expected = Q * K * (1.0 / 1.0);
        total_tests++;
        if (comparelongdoubles(sol, expected, EPSILON)) {
            printf("Expected %Le, got %Le. Test case 2 passed\n", expected, sol);
            test_passed++;
        } else {
            printf("Test case 2 failed. Expected %Le, got %Le\n", expected, sol);
        }

        freeVec(&f_n);
        freeVec(&d);
    }

    // Test case 3: Multiple charges
    {
        Vec f_n = vecInitA(0.0, 3);
        Vec d = vecInitA(0.0, 3);

        f_n.x[0] = 1.0;  f_n.x[1] = -2.0;  f_n.x[2] = 1.5;
        d.x[0] = 1.0;    d.x[1] = 2.0;     d.x[2] = 3.0;

        printf("\nTest case 3 - Multiple charges:\n");
        printf("Charges: ");
        printVecUnits(f_n, 'q');
        printf("Distances: ");
        printVecUnits(d, 'm');

        long double x = 4.0;
        long double sol = poissonSolveAnalytical(f_n, d, x);

        printf("Solution: V(%Lfm) = %Le\n", x, sol);

        // Validate result
        long double expected = Q * K * (1.0/3.0 - 2.0/2.0 + 1.5/1.0);
        total_tests++;
        if (comparelongdoubles(sol, expected, EPSILON)) {
            printf("Expected %Le, got %Le. Test case 3 passed\n", expected, sol);
            test_passed++;
        } else {
            printf("Test case 3 failed. Expected %Le, got %Le\n", expected, sol);
        }

        freeVec(&f_n);
        freeVec(&d);
    }

    // Test case 4: Edge cases
    {
        Vec f_n = vecInitA(0.0, 2);
        Vec d = vecInitA(1.0, 2);

        printf("\nTest case 4 - Edge cases:\n");

        // Case 4a: Zero charge
        f_n.x[0] = 0.0;  f_n.x[1] = 1.0;
        printf("\n1.) Zero charge at some location: ");
        printVecUnits(f_n, 'q');
        long double x = 2.0;
        long double sol = poissonSolveAnalytical(f_n, d, x);
        printf("Solution: V(%Lfm) = %Le\n", x, sol);

        // Validate result
        long double expected = Q * K * (1.0 / 1.0);
        total_tests++;
        if (comparelongdoubles(sol, expected, EPSILON)) {
            printf("Expected %Le, got %Le. Test case 4a passed\n", expected, sol);
            test_passed++;
        } else {
            printf("Test case 4a failed. Expected %Le, got %Le\n", expected, sol);
        }

        // Case 4b: Very large distance
        f_n.x[0] = 1.0;  f_n.x[1] = 1.0;
        d.x[0] = 1e6;    d.x[1] = 1e6;
        printf("\nVery large distance: ");
        printVecUnits(d, 'm');
        sol = poissonSolveAnalytical(f_n, d, x);
        printf("Solution: V(%Lfm) = %Le\n", x, sol);

        // Validate result (should be very close to zero)
        total_tests++;
        if (sol < EPSILON) {
            printf("Expected close to 0, got %Le. Test case 4b passed.\n", sol);
            test_passed++;
        } else {
            printf("Test case 4b failed. Expected close to 0, got %Le\n", sol);
        }

        // Test point very close to a charge (Outside Threshold)
        d.x[0] = 1.0;    d.x[1] = 2.0;
        x = 1.0 + 1e-9;
        printf("\nTest point very close to charge: Distance = %Le\n", fabsl(d.x[0] - x));
        sol = poissonSolveAnalytical(f_n, d, x);
        printf("Solution: V(%Lfm) = %Le\n", x, sol);

        total_tests++;
        expected = Q * K * (1.0 / 1e-9 + 1.0/ (1 - 1e-9));
        if (comparelongdoubles(sol, expected, EPSILON)) {
            printf("Expected %Le, got %Le, Test case 4c passed\n", expected, sol);
            test_passed++;
        } else {
            printf("Test case 4c failed. Expected %Le, got %Le\n", expected, sol);
        }

         // Test point very close to a charge (Crosses Threshold)
        d.x[0] = 1.0;    d.x[1] = 2.0;
        x = 1.0 + 1e-11;
        printf("\nTest point very close to charge: ");
        sol = poissonSolveAnalytical(f_n, d, x);
        printf("Solution: V(%Lfm) = %Le\n", x, sol);

        // Should ignore the charge at small distance
        total_tests++;
        expected = Q * K * (1.0/ (1 - 1e-11));
        if (comparelongdoubles(sol, expected, EPSILON)) {
            printf("Expected %Le, got %Le. Test case 4d passed\n", expected, sol);
            test_passed++;
        } else {
            printf("Test case 4d failed. Expected %Le, got %Le\n", expected, sol);
        }

        freeVec(&f_n);
        freeVec(&d);
    }

    // Test 5: Error handling
    {
        printf("\nTest case 5 - Error handling:\n");

        // Empty vectors
        Vec f_n = vecInitA(0.0, 0);
        Vec d = vecInitA(0.0, 0);
        long double x = 1.0;
        long double sol = poissonSolveAnalytical(f_n, d, x);
        
        total_tests++;
        if (isnan(sol) || isinf(sol)) {
            printf("Test case 5a passed (handled empty vectors)\n");
            test_passed++;
        } else {
            printf("Test case 5a failed. Expected NaN or Inf, got %Le\n", sol);
        }

        // Mismatched vector lengths
        f_n = vecInitA(1.0, 2);
        d = vecInitA(1.0, 3);
        sol = poissonSolveAnalytical(f_n, d, x);
        
        total_tests++;
        if (isnan(sol) || isinf(sol)) {
            printf("Test case 5b passed (handled mismatched vector lengths)\n");
            test_passed++;
        } else {
            printf("Test case 5b failed. Expected NaN or Inf, got %Le\n", sol);
        }

        freeVec(&f_n);
        freeVec(&d);
    }

    printf("\nPoisson Solver Test Summary: %d out of %d tests passed.\n", test_passed, total_tests);
}

void test_getGridV()
{
    printf("\nTesting getGridV function:\n");

    // Test case 1: Normal case
    {
        Vec f_n = vecInitA(0.0, 3);
        Vec d = vecInitA(0.0, 3);
        
        f_n.x[0] = 1.0;  f_n.x[1] = -2.0;  f_n.x[2] = 1.5;
        d.x[0] = 1.0;    d.x[1] = 2.0;     d.x[2] = 3.0;
        
        Vec result = getGridV(f_n, d);
        
        printf("Test case 1 - Normal case:\n");
        printf("Input charges: ");
        printVecUnits(f_n, 'q');
        printf("Input distances: ");
        printVecUnits(d, 'm');
        printf("Result: ");
        printVecUnits(result, 'V');
        
        // Basic validation
        if (!isnan(result.x[0]) && !isinf(result.x[0])) {
            printf("Test case 1 passed.\n");
        } else {
            printf("Test case 1 failed.\n");
        }
        
        freeVec(&result);
    }

    // Test case 2: Edge case - single charge
    {
        Vec f_n = vecInitA(1.0, 1);
        Vec d = vecInitA(1.0, 1);
        
        Vec result = getGridV(f_n, d);
        
        printf("Test case 2 - Single charge:\n");
        printf("Input charge: ");
        printVecUnits(f_n, 'q');
        printf("Input distance: ");
        printVecUnits(d, 'm');
        printf("Result: ");
        printVecUnits(result, 'V');
        
        if (!isnan(result.x[0]) && !isinf(result.x[0])) {
            printf("Test case 2 passed.\n");
        } else {
            printf("Test case 2 failed.\n");
        }
        
        freeVec(&result);
    }

    // Test case 3: Invalid input
    {
        Vec f_n = vecInitA(0.0, 2);
        Vec d = vecInitA(0.0, 3);  // Mismatched lengths
        
        Vec result = getGridV(f_n, d);
        
        printf("Test case 3 - Invalid input:\n");
        if (result.x == NULL && result.len == 0) {
            printf("Test case 3 passed (correctly handled invalid input).\n");
        } else {
            printf("Test case 3 failed (did not correctly handle invalid input).\n");
        }
        
        // No need to free result as it should be a null vector
    }
}

// void test_poisson_full()
// {
//     static int seeded = 0;
//     if (!seeded)
//     {
//         srand(rand()*time(NULL));
//         seeded = 1;
//     }

//     size_t len = (rand() % 15) + 1;
//     Vec V = vecInitZerosA(len);

//     for (size_t i = 0; i < NUM_TESTS; i ++)
//     {
//         printf("TEST %zu : \n", i);
//         V = getV(len);
//     }    
//     freeVec(&V);
// }


void testMeshGen()
{
    PyVi vis = pyviInitA("visualise/data.pyvi");
    Vec d = vecInitZerosA(10);
    for (size_t i = 0; i < d.len; i++)
    {
        d.x[i] = (long double)(i + 1) / 11;
    }

    // vecScale(1.0/3 *1e-9, d, &d);
    // vecPrint(d);
    // printf("\n");

    // size_t chunk_size = 1000;


    Vec f_n = vecInitOnesA(d.len);
    randF(f_n, 0, 0, 0);

    InputData data = getInput("data/input-params.toml");
    // *vecRef(d, 0) = 0.33 * data.params.L;
    // *vecRef(d, 1) = 0.66 * data.params.L;

    // printNL();
    // vecPrint(d);
    // printNL();
    OxParams params;
    params.L = 1;
    params.V_0 = 1;
    params.eps_r = 11.7;
    params.chunk_size = 100;
    printNL();
    vecPrint(d);
    printNL();
    
    Vec mesh = generateMesh(d, params);
    
    // printNL();
    // vecPrint(mesh);
    // printNL();

    PyViBase param = pyviCreateParameter(&vis, "x", mesh);
    PyViSec sec = pyviCreateSection(&vis, "Voltage", param);
    // PyViSection * analytical = pyviCreateSection(&vis, "Analytical Solution", param);

    // printNL();
    // vecPrint(data.probs);
    // printNL();
    // vecPrint(data.locs);
    // printNL();

    Vec sol = poissonWrapper(data, mesh);
    // pyviSectionPush(analytical, solA);

    pyviSectionPush(sec, sol);
    pyviWrite(vis);
    // system("python3 visualise/visualise.py");
    // freeVec(&sol), freeVec(&sol2), freeVec(&mesh), freeVec(&f_n), freeVec(&d);
    freePyVi(&vis);
}

void testSolver()
{

    // Class problem just to make sure it works

    MatTD mat = matTDinitA(6);
    size_t dim = 6;

    *vecRef(mat.main, 0) = 1;
    *vecRef(mat.main, dim - 1) = 1;

    for (size_t i = 1; i < dim - 1; i ++)
    {
        *vecRef(mat.main, i) = -2;
        *vecRef(mat.sup, i) = 1;
        *vecRef(mat.sub, i) = 1;
    }
    Vec b = vecInitZerosA(dim);
    for (size_t i = 0; i < dim; i ++)
    {
        *vecRef(b, i) = 0;
    }
    *vecRef(b, 0) = 1.0;
    *vecRef(b, 2) = 3.0;
    *vecRef(b, 3) = 4.0;
    *vecRef(b, 4) = 5.0;

    Vec sol = numSolveV(mat, b);
    vecPrint(sol);
}
