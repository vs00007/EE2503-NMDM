#pragma once
#include <stdio.h>

#define LARG_OK 0
#define LARG_ERROR 1

typedef struct Vec {
    double* x;
    size_t len;
} Vec;

// initialzie the vector on the heap with some initial value
Vec larg_vecInitA(double value, size_t len);
// initialize the vector on the heap to zeros
Vec larg_vecInitZerosA(size_t len);
// initialize the vector on the heap to ones
Vec larg_vecInitOnesA(size_t len);

// make a copy of a vector on heap
Vec larg_vecCopyA(Vec vector);

// construct a vector from a pointer(does not allocate)
Vec larg_vecConstruct(double* ptr, size_t len);

// pretty print a vector
void larg_vecPrint(Vec a);

// add 2 vectors and get result into another vector, prints error if input is invalid
int larg_vecAdd(Vec a, Vec b, Vec* result);
// multiply scalar value to vectors and get result into another vector, prints error if input is invalid
int larg_vecScale(double a, Vec b, Vec* result);
// unit vector of the norm, prints error if input is invalid
int larg_vecNormalize(Vec a, Vec* result);
// get the dot product between 2 variables, prints error if input is invalid
double larg_vecDot(Vec a, Vec b);
// get the L2 norm of vector, prints error if input is invalid
double larg_vecMagnitude(Vec a);
// get the L_p norm of vector, prints warning if p < 1, prints error if input is invalid
// for p = inf use larg_vecMax 
double larg_vecNorm(Vec a, double p);
// maximum value in the vector, prints error if input is invalid
double larg_vecMax(Vec a);
// minimum value in the vector, prints error if input is invalid
double larg_vecMin(Vec a);

// free the vector on the heap
void larg_freeVecX(Vec* vec);

