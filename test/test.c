#include <stdio.h>
#include "include/linarg.h"

void test_linarg_vec_impl()
{
    Vec a = linarg_vecInitZerosA(3);
    printf("a = "); linarg_vecPrint(a); printf("\n");

    Vec b = linarg_vecInitOnesA(3);
    printf("b = "); linarg_vecPrint(b); printf("\n");

    Vec c = linarg_vecConstruct((double[]){0.3, 0.1, 2}, 3);
    printf("c = "); linarg_vecPrint(c); printf("\n");

    printf("\n");

    Vec res = linarg_vecInitZerosA(3);

    double val = 0.0;

    linarg_vecAdd(a, b, &res); printf("a + b = "); linarg_vecPrint(res); printf("\n");
    linarg_vecAdd(b, c, &res); printf("b + c = "); linarg_vecPrint(res); printf("\n");
    linarg_vecAdd(a, c, &res); printf("a + c = "); linarg_vecPrint(res); printf("\n");
    printf("\n");
    linarg_vecScale(2.0, a, &res); printf("2*a = "); linarg_vecPrint(res); printf("\n");
    linarg_vecScale(2.0, b, &res); printf("2*b = "); linarg_vecPrint(res); printf("\n");
    linarg_vecScale(2.0, c, &res); printf("2*c = "); linarg_vecPrint(res); printf("\n");
    printf("\n");

    val = linarg_vecDot(a, b); printf("a.b = %f\n", val);
    val = linarg_vecDot(b, c); printf("b.c = %f\n", val);
    val = linarg_vecDot(a, c); printf("a.c = %f\n", val);
    printf("\n");
    val = linarg_vecMagnitude(a); printf("||a|| = %f\n", val);
    val = linarg_vecMagnitude(b); printf("||b|| = %f\n", val);
    val = linarg_vecMagnitude(c); printf("||c|| = %f\n", val);
    printf("\n");
    val = linarg_vecMax(a); printf("max a = %f\n", val);
    val = linarg_vecMax(b); printf("max b = %f\n", val);
    val = linarg_vecMax(c); printf("max c = %f\n", val);
    printf("\n");
    val = linarg_vecMin(a); printf("min a = %f\n", val);
    val = linarg_vecMin(b); printf("min b = %f\n", val);
    val = linarg_vecMin(c); printf("min c = %f\n", val);

    // free all memory
    linarg_freeVecX(&a);
    linarg_freeVecX(&b);
    linarg_freeVecX(&res);
}

int main()
{
    test_linarg_vec_impl();
    return 0;
}

