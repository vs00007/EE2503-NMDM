#include "include/linarg.h"

#include <stdlib.h>
#include <memory.h>
#include <math.h>

// gets value at index from vector by reference(dereferenced)
// allows for syntax like: LA_VIDX(a, 2) = 5;
// DOES NOT CHECK FOR OUT OF BOUNDS
#define LA_VIDX(vector, index) *(vector.x + vector.offset * index)

// gets value at index from vector(ptr) by reference(dereferenced)
// allows for syntax like: LA_VIDX(a, 2) = 5;
// DOES NOT CHECK FOR OUT OF BOUNDS
#define LA_VIDX_PTR(vector, index) *(vector->x + vector->offset * index)

// initialzie the vector on the heap with some initial value
Vec linarg_vecInitA(double value, size_t len)
{
    Vec x = { (double*)calloc(len, sizeof(double)), len, 1 };
    LINARG_ASSERT_ERROR(!x.x, x, "unkown error occured when allocation memory!");
    for(size_t i = 0; i < x.len; i++) x.x[i] = value;
    return x;
}
// initialize the vector on the heap to zeros
Vec linarg_vecInitZerosA(size_t len)
{
    return linarg_vecInitA(0.0, len);
}
// initialize the vector on the heap to ones
Vec linarg_vecInitOnesA(size_t len)
{
    return linarg_vecInitA(1.0, len);
}

// make a copy of a vector on heap
Vec linarg_vecCopyA(Vec vector)
{
    Vec x = { (double*)calloc(vector.len, sizeof(double)), vector.len, 1 };
    LINARG_ASSERT_ERROR(!x.x, x, "unkown error occured when allocation memory!");
    for(size_t i = 0; i < x.len; i++) x.x[i] = LA_VIDX(vector, i);
    return x;
}

// construct a vector from a pointer(does not allocate)
Vec linarg_vecConstruct(double* ptr, size_t len)
{
    Vec x = { ptr, len, 1 };
    return x;
}

// pretty print a vector
void linarg_vecPrint(Vec a)
{
    printf("[");
    for(size_t i = 0; i < a.len - 1; i++)
    {
        printf("%f, ", LA_VIDX(a, i));
    }
    printf("%f]", LA_VIDX(a, a.len - 1));
}

// gets the nth value in a vector
// handles buffer offsets
// checks for out-of-bounds
double linarg_vecGet(Vec a, size_t n)
{
    LINARG_ASSERT_ERROR(n >= a.len, NAN, "out of bounds vector access!");
    return *(a.x + a.offset * n);
}

// gets the nth value in a vector
// handles buffer offsets
// checks for out-of-bounds
double* linarg_vecRef(Vec a, size_t n)
{
    LINARG_ASSERT_ERROR(n >= a.len, NULL, "out of bounds vector access!");
    return (a.x + a.offset * n);
}

// add 2 vectors and get result into another vector
int linarg_vecAdd(Vec a, Vec b, Vec* result)
{
    LINARG_ASSERT_ERROR(a.len != b.len, LINARG_ERROR, "attempt to add vectors with dimension %zu and %zu!", a.len, b.len);
    LINARG_ASSERT_ERROR(!result || !result->x, LINARG_ERROR, "result vector is null!");
    LINARG_ASSERT_ERROR(!a.x || !b.x, LINARG_ERROR, "input vector/s is/are null!");
    LINARG_ASSERT_ERROR(b.len < result->len, LINARG_ERROR, "output vector not big enough to store result!");
    LINARG_ASSERT_ERROR(b.len > result->len, LINARG_ERROR, "output dimension larger than input dimension!");

    for(size_t i = 0; i < a.len; i++)
    {
        LA_VIDX_PTR(result, i) = LA_VIDX(a, i) + LA_VIDX(b, i);
    }

    return LINARG_OK;
}
// multiply scalar value to vectors and get result into another vector
int linarg_vecScale(double a, Vec b, Vec* result)
{
    LINARG_ASSERT_ERROR(!result || !result->x, LINARG_ERROR, "resultant vector is null!");
    LINARG_ASSERT_ERROR(!b.x, LINARG_ERROR, "input vector/s is/are null!");
    LINARG_ASSERT_ERROR(b.len < result->len, LINARG_ERROR, "output vector not big enough to store result!");
    LINARG_ASSERT_ERROR(b.len > result->len, LINARG_ERROR, "output dimension larger than input dimension!");

    for(size_t i = 0; i < b.len; i++)
    {
        LA_VIDX_PTR(result, i) = a * LA_VIDX(b, i);
    }

    return LINARG_OK;
}
// unit vector of the norm
int linarg_vecNormalize(Vec a, Vec* result)
{
    return linarg_vecScale(1 / linarg_vecMagnitude(a), a, result);
}
// get the dot product between 2 variables
double linarg_vecDot(Vec a, Vec b)
{
    LINARG_ASSERT_ERROR(a.len != b.len, NAN, "attempt to take dot product of vectors with dimension %zu and %zu!", a.len, b.len);
    LINARG_ASSERT_ERROR(!a.x || !b.x, NAN, "input vector/s is/are null!");

    double result = 0.0;

    for(size_t i = 0; i < a.len; i++)
    {
        result += LA_VIDX(a, i) * LA_VIDX(b, i);
    }

    return result;
}
// get the L2 norm of vector
double linarg_vecMagnitude(Vec a)
{
    return sqrt(linarg_vecDot(a, a));
}
// get the L_p norm of vector, prints warning if p < 1
// for p = inf use linarg_vecMax 
double linarg_vecNorm(Vec a, double p)
{
    LINARG_ASSERT_ERROR(!a.x, NAN, "input vector is null!");
    LINARG_ASSERT_ERROR(p < 1, NAN, "L_p is not a valid norm for p = %f", p);

    double result = 0.0;

    for(size_t i = 0; i < a.len; i++)
    {
        result +=  pow(fabs(LA_VIDX(a, i)), p);
    }

    return pow(result, 1/p);
}
// maximum value in the vector
double linarg_vecMax(Vec a)
{
    LINARG_ASSERT_ERROR(!a.x, NAN, "input vector is null!");
    LINARG_ASSERT_WARN(a.len == 0, INFINITY, "max of a zero dimention vector");

    double result = -INFINITY;

    for(size_t i = 0; i < a.len; i++)
    {
        result = result > LA_VIDX(a, i) ? result : LA_VIDX(a, i);
    }

    return result;
}
// minimum value in the vector
double linarg_vecMin(Vec a)
{
    LINARG_ASSERT_ERROR(!a.x, NAN, "input vector is null!");
    LINARG_ASSERT_WARN(a.len == 0, -INFINITY, "min of a zero dimention vector");

    double result = INFINITY;

    for(size_t i = 0; i < a.len; i++)
    {
        result = result < LA_VIDX(a, i) ? result : LA_VIDX(a, i);
    }

    return result;
}

// free the vector on the heap
void linarg_freeVecX(Vec* vec)
{
    // skip if null
    if(!vec->x) return;

    free(vec->x);
    vec->x = NULL;
    vec->len = 0;
}