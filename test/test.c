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
    linalg_freeVecX(&a);
    linalg_freeVecX(&b);
    linalg_freeVecX(&res);
}

int main()
{
    test_linalg_vec_impl();
    return 0;
}

