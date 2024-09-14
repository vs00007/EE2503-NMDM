#include <stdio.h>
#include "include/linalg.h"

void test_linalg_vec_impl()
{
    Vec a = linalg_vecInitZerosA(3);
    printf("a = "); linalg_vecPrint(a); printf("\n");

    Vec b = linalg_vecInitOnesA(3);
    printf("b = "); linalg_vecPrint(b); printf("\n");

    Vec c = linalg_vecConstruct((double[]){0.3, 0.1, 2}, 3);
    printf("c = "); linalg_vecPrint(c); printf("\n");

    printf("\n");

    Vec res = linalg_vecInitZerosA(3);

    double val = 0.0;

    linalg_vecAdd(a, b, &res); printf("a + b = "); linalg_vecPrint(res); printf("\n");
    linalg_vecAdd(b, c, &res); printf("b + c = "); linalg_vecPrint(res); printf("\n");
    linalg_vecAdd(a, c, &res); printf("a + c = "); linalg_vecPrint(res); printf("\n");
    printf("\n");
    linalg_vecScale(2.0, a, &res); printf("2*a = "); linalg_vecPrint(res); printf("\n");
    linalg_vecScale(2.0, b, &res); printf("2*b = "); linalg_vecPrint(res); printf("\n");
    linalg_vecScale(2.0, c, &res); printf("2*c = "); linalg_vecPrint(res); printf("\n");
    printf("\n");

    val = linalg_vecDot(a, b); printf("a.b = %f\n", val);
    val = linalg_vecDot(b, c); printf("b.c = %f\n", val);
    val = linalg_vecDot(a, c); printf("a.c = %f\n", val);
    printf("\n");
    val = linalg_vecMagnitude(a); printf("||a|| = %f\n", val);
    val = linalg_vecMagnitude(b); printf("||b|| = %f\n", val);
    val = linalg_vecMagnitude(c); printf("||c|| = %f\n", val);
    printf("\n");
    val = linalg_vecMax(a); printf("max a = %f\n", val);
    val = linalg_vecMax(b); printf("max b = %f\n", val);
    val = linalg_vecMax(c); printf("max c = %f\n", val);
    printf("\n");
    val = linalg_vecMin(a); printf("min a = %f\n", val);
    val = linalg_vecMin(b); printf("min b = %f\n", val);
    val = linalg_vecMin(c); printf("min c = %f\n", val);

    // free all memory
    linalg_freeVec(&a);
    linalg_freeVec(&b);
    linalg_freeVec(&res);
}

void test_linalg_mat_impl()
{
    // Initialize matrices with zeros, ones, and a specific set of values
    Mat2d a = linalg_mat2DInitZerosA(3, 3);
    printf("Matrix a (zeros): "); linalg_mat2DPrint(a); printf("\n");

    Mat2d b = linalg_mat2DInitOnesA(3, 3);
    printf("Matrix b (ones): "); linalg_mat2DPrint(b); printf("\n");

    Mat2d c = linalg_mat2DConstruct((double[]){
        0.3, 0.1, 2.0,
        1.2, 0.4, 0.7,
        0.5, 0.9, 0.2
    }, 3, 3);
    printf("Matrix c (specific values): "); linalg_mat2DPrint(c); printf("\n");

    printf("\n");

    // Result matrix initialized to zeros
    Mat2d res = linalg_mat2DInitZerosA(3, 3);

    // Matrix addition tests
    linalg_mat2DAdd(a, b, &res); printf("a + b = "); linalg_mat2DPrint(res); printf("\n");
    linalg_mat2DAdd(b, c, &res); printf("b + c = "); linalg_mat2DPrint(res); printf("\n");
    linalg_mat2DAdd(a, c, &res); printf("a + c = "); linalg_mat2DPrint(res); printf("\n");

    printf("\n");

    // Matrix scaling tests
    linalg_mat2DScale(2.0, a, &res); printf("2 * a = "); linalg_mat2DPrint(res); printf("\n");
    linalg_mat2DScale(2.0, b, &res); printf("2 * b = "); linalg_mat2DPrint(res); printf("\n");
    linalg_mat2DScale(2.0, c, &res); printf("2 * c = "); linalg_mat2DPrint(res); printf("\n");

    printf("\n");

    // Maximum value in each matrix
    double maxVal = linalg_mat2DMax(a); printf("max of a = %f\n", maxVal);
    maxVal = linalg_mat2DMax(b); printf("max of b = %f\n", maxVal);
    maxVal = linalg_mat2DMax(c); printf("max of c = %f\n", maxVal);

    printf("\n");

    // Minimum value in each matrix
    double minVal = linalg_mat2DMin(a); printf("min of a = %f\n", minVal);
    minVal = linalg_mat2DMin(b); printf("min of b = %f\n", minVal);
    minVal = linalg_mat2DMin(c); printf("min of c = %f\n", minVal);

    printf("\n");

    // Free all allocated matrices
    linalg_freeMat2D(&a);
    linalg_freeMat2D(&b);
    linalg_freeMat2D(&res);
}

int main()
{
    printf("Running Vector tests: \n\n");
    test_linalg_vec_impl();

    for(int i = 0; i < 50; i++) printf("-");
    printf("\n");

    printf("Running Matrix tests: \n\n");
    test_linalg_mat_impl();
    return 0;
}

