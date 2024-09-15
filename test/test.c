#include <stdio.h>
#include "include/linalg.h"

void test_vec_impl()
{
    Vec a = vecInitZerosA(3);
    printf("a = "); vecPrint(a); printf("\n");

    Vec b = vecInitOnesA(3);
    printf("b = "); vecPrint(b); printf("\n");

    Vec c = vecConstruct((double[]){0.3, 0.1, 2}, 3);
    printf("c = "); vecPrint(c); printf("\n");

    printf("\n");

    Vec res = vecInitZerosA(3);

    double val = 0.0;

    vecAdd(a, b, &res); printf("a + b = "); vecPrint(res); printf("\n");
    vecAdd(b, c, &res); printf("b + c = "); vecPrint(res); printf("\n");
    vecAdd(a, c, &res); printf("a + c = "); vecPrint(res); printf("\n");
    printf("\n");
    vecScale(2.0, a, &res); printf("2*a = "); vecPrint(res); printf("\n");
    vecScale(2.0, b, &res); printf("2*b = "); vecPrint(res); printf("\n");
    vecScale(2.0, c, &res); printf("2*c = "); vecPrint(res); printf("\n");
    printf("\n");

    val = vecDot(a, b); printf("a.b = %f\n", val);
    val = vecDot(b, c); printf("b.c = %f\n", val);
    val = vecDot(a, c); printf("a.c = %f\n", val);
    printf("\n");
    val = vecMagnitude(a); printf("||a|| = %f\n", val);
    val = vecMagnitude(b); printf("||b|| = %f\n", val);
    val = vecMagnitude(c); printf("||c|| = %f\n", val);
    printf("\n");
    val = vecMax(a); printf("max a = %f\n", val);
    val = vecMax(b); printf("max b = %f\n", val);
    val = vecMax(c); printf("max c = %f\n", val);
    printf("\n");
    val = vecMin(a); printf("min a = %f\n", val);
    val = vecMin(b); printf("min b = %f\n", val);
    val = vecMin(c); printf("min c = %f\n", val);

    // free all memory
    freeVec(&a);
    freeVec(&b);
    freeVec(&res);
}

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

int main()
{
    printf("Running Vector tests: \n\n");
    test_vec_impl();

    for(int i = 0; i < 50; i++) printf("-");
    printf("\n");

    printf("Running Matrix tests: \n\n");
    test_mat_impl();
    return 0;
}

