#include <stdio.h>
#include <include/larg.h>

void test_larg_vec_impl()
{
    Vec a = larg_vecInitZerosA(3);
    printf("a = "); larg_vecPrint(a); printf("\n");

    Vec b = larg_vecInitOnesA(3);
    printf("b = "); larg_vecPrint(b); printf("\n");

    Vec c = larg_vecConstruct((double[]){0.3, 0.1, 2}, 3);
    printf("c = "); larg_vecPrint(c); printf("\n");

    printf("\n");

    Vec res = larg_vecInitZerosA(3);

    double val = 0.0;

    larg_vecAdd(a, b, &res); printf("a + b = "); larg_vecPrint(res); printf("\n");
    larg_vecAdd(b, c, &res); printf("b + c = "); larg_vecPrint(res); printf("\n");
    larg_vecAdd(a, c, &res); printf("a + c = "); larg_vecPrint(res); printf("\n");
    printf("\n");
    larg_vecScale(2.0, a, &res); printf("2*a = "); larg_vecPrint(res); printf("\n");
    larg_vecScale(2.0, b, &res); printf("2*b = "); larg_vecPrint(res); printf("\n");
    larg_vecScale(2.0, c, &res); printf("2*c = "); larg_vecPrint(res); printf("\n");
    printf("\n");

    val = larg_vecDot(a, b); printf("a.b = %f\n", val);
    val = larg_vecDot(b, c); printf("b.c = %f\n", val);
    val = larg_vecDot(a, c); printf("a.c = %f\n", val);
    printf("\n");
    val = larg_vecMagnitude(a); printf("||a|| = %f\n", val);
    val = larg_vecMagnitude(b); printf("||b|| = %f\n", val);
    val = larg_vecMagnitude(c); printf("||c|| = %f\n", val);
    printf("\n");
    val = larg_vecMax(a); printf("max a = %f\n", val);
    val = larg_vecMax(b); printf("max b = %f\n", val);
    val = larg_vecMax(c); printf("max c = %f\n", val);
    printf("\n");
    val = larg_vecMin(a); printf("min a = %f\n", val);
    val = larg_vecMin(b); printf("min b = %f\n", val);
    val = larg_vecMin(c); printf("min c = %f\n", val);

    // free all memory
    larg_freeVecX(&a);
    larg_freeVecX(&b);
    larg_freeVecX(&res);
}

int main()
{
    test_larg_vec_impl();
    return 0;
}

