#include <include/linalg.h>

#include <stdlib.h>
#include <memory.h>
#include <math.h>

// initialzie the vector on the heap with some initial value
Vec larg_vecInitA(double value, size_t len)
{
    Vec x = { (double*)calloc(len, sizeof(double)), len };
    for(size_t i = 0; i < x.len; i++) x.x[i] = value;
    
    return x;
}
// initialize the vector on the heap to zeros
Vec larg_vecInitZerosA(size_t len)
{
    return larg_vecInitA(0.0, len);
}
// initialize the vector on the heap to ones
Vec larg_vecInitOnesA(size_t len)
{
    return larg_vecInitA(1.0, len);
}

// make a copy of a vector on heap
Vec larg_vecCopyA(Vec vector)
{
    Vec x = { (double*)calloc(vector.len, sizeof(double)), vector.len };
    memcpy(x.x, vector.x, vector.len);
    return x;
}

// construct a vector from a pointer(does not allocate)
Vec larg_vecConstruct(double* ptr, size_t len)
{
    Vec x = { ptr, len };
    return x;
}

// pretty print a vector
void larg_vecPrint(Vec a)
{
    printf("(");
    for(size_t i = 0; i < a.len - 1; i++)
    {
        printf("%f, ", a.x[i]);
    }
    printf("%f)", a.x[a.len - 1]);
}

// add 2 vectors and get result into another vector
int larg_vecAdd(Vec a, Vec b, Vec* result)
{
    if(a.len != b.len)
    {
        printf("Fatal Error: Attempt to add vectors with dimension %zu and %zu!\n", a.len, b.len);
        return LARG_ERROR;
    }
    else if(!result || !result->x)
    {
        printf("Fatal Error: Resultant vector is null!\n");
        return LARG_ERROR;
    }
    else if(!a.x || !b.x)
    {
        printf("Fatal Error: Input vector/s is/are null!\n");
        return LARG_ERROR;
    }
    
    for(size_t i = 0; i < a.len; i++)
    {
        result->x[i] = a.x[i] + b.x[i];
    }

    return LARG_OK;
}
// multiply scalar value to vectors and get result into another vector
int larg_vecScale(double a, Vec b, Vec* result)
{
    if(!result || !result->x)
    {
        printf("Fatal Error: Resultant vector is null!\n");
        return LARG_ERROR;
    }
    else if(!b.x)
    {
        printf("Fatal Error: Input vector/s is/are null!\n");
        return LARG_ERROR;
    }
    
    for(size_t i = 0; i < b.len; i++)
    {
        result->x[i] = a * b.x[i];
    }

    return LARG_OK;
}
// unit vector of the norm
int larg_vecNormalize(Vec a, Vec* result)
{
    return larg_vecScale(1 / larg_vecMagnitude(a), a, result);
}
// get the dot product between 2 variables
double larg_vecDot(Vec a, Vec b)
{
    if(a.len != b.len)
    {
        printf("Fatal Error: Attempt to take dot product of vectors with dimension %zu and %zu!\n", a.len, b.len);
        return NAN;
    }
    else if(!a.x || !b.x)
    {
        printf("Fatal Error: Input vector/s is/are null!\n");
        return NAN;
    }

    double result = 0.0;

    for(size_t i = 0; i < a.len; i++)
    {
        result += a.x[i] * b.x[i];
    }

    return result;
}
// get the L2 norm of vector
double larg_vecMagnitude(Vec a)
{
    return sqrt(larg_vecDot(a, a));
}
// get the L_p norm of vector, prints warning if p < 1
// for p = inf use larg_vecMax 
double larg_vecNorm(Vec a, double p)
{
    if(!a.x)
    {
        printf("Fatal Error: Input vector is null!\n");
        return NAN;
    }
    else if(p < 1)
    {
        printf("Fatal Error: L_p is not a valid norm for p = %f\n", p);
        return NAN;
    }

    double result = 0.0;

    for(size_t i = 0; i < a.len; i++)
    {
        result +=  pow(fabs(a.x[i]), p);
    }

    return pow(result, 1/p);
}
// maximum value in the vector
double larg_vecMax(Vec a)
{
    if(!a.x)
    {
        printf("Fatal Error: Input vector is null!\n");
        return NAN;
    }
    else if(a.len == 0)
    {
        printf("Warning: max of a zero dimention vector");
        return INFINITY;
    }

    double result = -INFINITY;

    for(size_t i = 0; i < a.len; i++)
    {
        result = result > a.x[i] ? result : a.x[i];
    }

    return result;
}
// minimum value in the vector
double larg_vecMin(Vec a)
{
    if(!a.x)
    {
        printf("Fatal Error: Input vector is null!\n");
        return NAN;
    }
    else if(a.len == 0)
    {
        printf("Warning: min of a zero dimention vector");
        return -INFINITY;
    }

    double result = INFINITY;

    for(size_t i = 0; i < a.len; i++)
    {
        result = result < a.x[i] ? result : a.x[i];
    }

    return result;
}

// free the vector on the heap
void larg_freeVecX(Vec* vec)
{
    // skip if null
    if(!vec->x) return;

    free(vec->x);
    vec->x = NULL;
    vec->len = 0;
}