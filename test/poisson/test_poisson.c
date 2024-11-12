#include <include/linalg.h>
#include <include/poisson.h>
#include <time.h>

#define LA_VIDX(vector, index) *(vector.x + vector.offset * index)
#define EPSILON 1e-6
#define NUM_TESTS 100

int compareDoubles(double a, double b, double epsilon)
{
    return fabs(a - b) < epsilon;
}

int qsort_compare(const void *a, const void *b) {
    return (*(double*)a > *(double*)b) - (*(double*)a < *(double*)b);
}

void randF(Vec param, double min, double max, int sort)
{
    if (param.len == 0)
    {
        printf("ERROR: Length of input vector is 0.\n");
        return;
    }
    
    for (size_t i = 0; i < param.len; i++)
    {
        param.x[i] = min + (max - min) * ((double)rand() / RAND_MAX);
    }

    if (sort)
    {
        qsort(param.x, param.len, sizeof(double), qsort_compare);
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

        double x = 0.2;
        double sol = poissonSolveAnalytical(f_n, d, x);

        printf("Exact solution : V(%lfm) = %e\n", x, sol);

        
        double expected = Q * K * (1.0/1.0 + 1.0/1.0 + 1.0/1.0 + 1.0/1.0); 
        total_tests++;
        if (compareDoubles(sol, expected, EPSILON)) {
            printf("Expected %e, got %e. Test case 1 passed.\n", expected, sol);
            test_passed++;
        } else {
            printf("Test case 1 failed. Expected %e, got %e\n", expected, sol);
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

        double x = 2.0;
        double sol = poissonSolveAnalytical(f_n, d, x);

        printf("Solution: V(%lfm) = %e\n", x, sol);


        double expected = Q * K * (1.0 / 1.0);
        total_tests++;
        if (compareDoubles(sol, expected, EPSILON)) {
            printf("Expected %e, got %e. Test case 2 passed\n", expected, sol);
            test_passed++;
        } else {
            printf("Test case 2 failed. Expected %e, got %e\n", expected, sol);
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

        double x = 4.0;
        double sol = poissonSolveAnalytical(f_n, d, x);

        printf("Solution: V(%lfm) = %e\n", x, sol);

        // Validate result
        double expected = Q * K * (1.0/3.0 - 2.0/2.0 + 1.5/1.0);
        total_tests++;
        if (compareDoubles(sol, expected, EPSILON)) {
            printf("Expected %e, got %e. Test case 3 passed\n", expected, sol);
            test_passed++;
        } else {
            printf("Test case 3 failed. Expected %e, got %e\n", expected, sol);
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
        double x = 2.0;
        double sol = poissonSolveAnalytical(f_n, d, x);
        printf("Solution: V(%lfm) = %e\n", x, sol);

        // Validate result
        double expected = Q * K * (1.0 / 1.0);
        total_tests++;
        if (compareDoubles(sol, expected, EPSILON)) {
            printf("Expected %e, got %e. Test case 4a passed\n", expected, sol);
            test_passed++;
        } else {
            printf("Test case 4a failed. Expected %e, got %e\n", expected, sol);
        }

        // Case 4b: Very large distance
        f_n.x[0] = 1.0;  f_n.x[1] = 1.0;
        d.x[0] = 1e6;    d.x[1] = 1e6;
        printf("\nVery large distance: ");
        printVecUnits(d, 'm');
        sol = poissonSolveAnalytical(f_n, d, x);
        printf("Solution: V(%lfm) = %e\n", x, sol);

        // Validate result (should be very close to zero)
        total_tests++;
        if (sol < EPSILON) {
            printf("Expected close to 0, got %e. Test case 4b passed.\n", sol);
            test_passed++;
        } else {
            printf("Test case 4b failed. Expected close to 0, got %e\n", sol);
        }

        // Test point very close to a charge (Outside Threshold)
        d.x[0] = 1.0;    d.x[1] = 2.0;
        x = 1.0 + 1e-9;
        printf("\nTest point very close to charge: Distance = %e\n", fabs(d.x[0] - x));
        sol = poissonSolveAnalytical(f_n, d, x);
        printf("Solution: V(%lfm) = %e\n", x, sol);

        total_tests++;
        expected = Q * K * (1.0 / 1e-9 + 1.0/ (1 - 1e-9));
        if (compareDoubles(sol, expected, EPSILON)) {
            printf("Expected %e, got %e, Test case 4c passed\n", expected, sol);
            test_passed++;
        } else {
            printf("Test case 4c failed. Expected %e, got %e\n", expected, sol);
        }

         // Test point very close to a charge (Crosses Threshold)
        d.x[0] = 1.0;    d.x[1] = 2.0;
        x = 1.0 + 1e-11;
        printf("\nTest point very close to charge: ");
        sol = poissonSolveAnalytical(f_n, d, x);
        printf("Solution: V(%lfm) = %e\n", x, sol);

        // Should ignore the charge at small distance
        total_tests++;
        expected = Q * K * (1.0/ (1 - 1e-11));
        if (compareDoubles(sol, expected, EPSILON)) {
            printf("Expected %e, got %e. Test case 4d passed\n", expected, sol);
            test_passed++;
        } else {
            printf("Test case 4d failed. Expected %e, got %e\n", expected, sol);
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
        double x = 1.0;
        double sol = poissonSolveAnalytical(f_n, d, x);
        
        total_tests++;
        if (isnan(sol) || isinf(sol)) {
            printf("Test case 5a passed (handled empty vectors)\n");
            test_passed++;
        } else {
            printf("Test case 5a failed. Expected NaN or Inf, got %e\n", sol);
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
            printf("Test case 5b failed. Expected NaN or Inf, got %e\n", sol);
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
    OxParams params;
    params.L = 3;
    Vec d = vecInitZerosA(2);
    for (size_t i = 0; i < d.len; i++)
    {
        d.x[i] = (double)(i + 1);
    }
    vecPrint(d);
    printf("\n");
    Vec mesh = generateMesh(d, params);
    vecPrint(mesh);
    // Vec h = generateStepSize(mesh);
    // vecPrint(h);
    // Mat2d jcob = generateJacobian(mesh, params);
}