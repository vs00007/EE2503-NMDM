#include <stdio.h>
#include "include/linalg.h"

#include "linalg/vector.h"

void test_mat_impl()
{
    // Initialize matrices with zeros, ones, and a specific set of values
    Mat2d a = mat2DInitZerosA(3, 3);
    printf("Matrix a (zeros): "); mat2DPrint(a); printf("\n");

    Mat2d b = mat2DInitOnesA(3, 3);
    printf("Matrix b (ones): "); mat2DPrint(b); printf("\n");

    Mat2d c = mat2DConstruct((double[]){
        0.3, 0.1, 2.0,
        1.2, 0.4, 0.7,
        0.5, 0.9, 0.2
    }, 3, 3);
    printf("Matrix c (specific values): "); mat2DPrint(c); printf("\n");

    printf("\n");

    // Result matrix initialized to zeros
    Mat2d res = mat2DInitZerosA(3, 3);

    // Matrix addition tests
    mat2DAdd(a, b, &res); printf("a + b = "); mat2DPrint(res); printf("\n");
    mat2DAdd(b, c, &res); printf("b + c = "); mat2DPrint(res); printf("\n");
    mat2DAdd(a, c, &res); printf("a + c = "); mat2DPrint(res); printf("\n");

    printf("\n");

    // Matrix scaling tests
    mat2DScale(2.0, a, &res); printf("2 * a = "); mat2DPrint(res); printf("\n");
    mat2DScale(2.0, b, &res); printf("2 * b = "); mat2DPrint(res); printf("\n");
    mat2DScale(2.0, c, &res); printf("2 * c = "); mat2DPrint(res); printf("\n");

    printf("\n");

    // Maximum value in each matrix
    double maxVal = mat2DMax(a); printf("max of a = %f\n", maxVal);
    maxVal = mat2DMax(b); printf("max of b = %f\n", maxVal);
    maxVal = mat2DMax(c); printf("max of c = %f\n", maxVal);

    printf("\n");

    // Minimum value in each matrix
    double minVal = mat2DMin(a); printf("min of a = %f\n", minVal);
    minVal = mat2DMin(b); printf("min of b = %f\n", minVal);
    minVal = mat2DMin(c); printf("min of c = %f\n", minVal);

    printf("\n");

    // Free all allocated matrices
    freeMat2D(&a);
    freeMat2D(&b);
    freeMat2D(&res);
}

int run_all_tests()
{
    printf("Running tests:\n");
    linalg_vec_test();
    linalg_matrix_test();
    return 0;
}

