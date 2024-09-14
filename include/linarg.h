#pragma once
#include <stdio.h>

#include "include/utils.h"

#define LINARG_OK 0
#define LINARG_ERROR 1

// Error Handling

// call this macro to handle errors
// all linarg errors are routed through this macro,
// so by placing a breakpoint in error_handler function code(defined in src/linarg/common.c), 
// we can check the error directly
#define LINARG_REPORT_ERROR(...) \
        printf("Error: "); \
        error_handler(__FILE__, __func__, __LINE__); \
        printf(__VA_ARGS__); 

// call this macro to handle warnings
// all linarg errors are routed through this macro,
// so by placing a breakpoint in error_handler function code(defined in src/linarg/common.c), 
// we can check the error directly
#define LINARG_REPORT_WARN(...) \
        printf("Warning: "); \
        error_handler(__FILE__, __func__, __LINE__); \
        printf(__VA_ARGS__);

// if condition is true, then report an error and return ret.
// leave ret blank for void functions
#define LINARG_ASSERT_ERROR(condition, ret, ...) \
        if(condition) \
        { \
            LINARG_REPORT_ERROR(__VA_ARGS__) \
            return ret; \
        }

// if condition is true, then report an warning and return ret.
// leave ret blank for void functions
#define LINARG_ASSERT_WARN(condition, ret, ...) \
        if(condition) \
        { \
            LINARG_REPORT_WARN(__VA_ARGS__) \
            return ret; \
        }

// gets value at index from vector by reference(dereferenced)
// allows for syntax like: VEC_INDEX(a, 2) = 5;
#define VEC_INDEX(vector, index) *linarg_vecRef(vector, index)

// This function is called in the LINARG_ERROR_TRAP macro
void error_handler(const char* file, const char* function, size_t line_no);

// Vector implimentation

typedef struct Vec {
    double* x;
    size_t len;
    size_t offset;
} Vec;

// initialzie the vector on the heap with some initial value
Vec linarg_vecInitA(double value, size_t len);
// initialize the vector on the heap to zeros
Vec linarg_vecInitZerosA(size_t len);
// initialize the vector on the heap to ones
Vec linarg_vecInitOnesA(size_t len);

// make a copy of a vector on heap
Vec linarg_vecCopyA(Vec vector);

// construct a vector from a pointer(does not allocate)
Vec linarg_vecConstruct(double* ptr, size_t len);

// pretty print a vector
void linarg_vecPrint(Vec a);

// gets the nth value in a vector(by value)
// handles buffer offsets
// checks for out-of-bounds
double linarg_vecGet(Vec a, size_t n);
// gets the nth value in a vector(by ref)
// handles buffer offsets
// checks for out-of-bounds( returns nullptr is performed )
double* linarg_vecRef(Vec a, size_t n);

// add 2 vectors and get result into another vector, prints error if input is invalid
int linarg_vecAdd(Vec a, Vec b, Vec* result);
// multiply scalar value to vectors and get result into another vector, prints error if input is invalid
int linarg_vecScale(double a, Vec b, Vec* result);
// unit vector of the norm, prints error if input is invalid
int linarg_vecNormalize(Vec a, Vec* result);
// get the dot product between 2 variables, prints error if input is invalid
double linarg_vecDot(Vec a, Vec b);
// get the L2 norm of vector, prints error if input is invalid
double linarg_vecMagnitude(Vec a);
// get the L_p norm of vector, prints warning if p < 1, prints error if input is invalid
// for p = inf use linarg_vecMax 
double linarg_vecNorm(Vec a, double p);
// maximum value in the vector, prints error if input is invalid
double linarg_vecMax(Vec a);
// minimum value in the vector, prints error if input is invalid
double linarg_vecMin(Vec a);

// free the vector on the heap
void linarg_freeVec(Vec* vec);

// Matrix implimentation

// Matrix is stored in 1d array
// 0*col ... 1*cols-1: 1st row
// 1*col ... 2*cols-1: 2nd row
// 2*col ... 3*cols-1: 3nd row
// so on
// (row-1)*col ... row*cols-1: last row
typedef struct Mat2d
{
    double* mat;
    size_t rows;
    size_t cols;
} Mat2d;

// initialzie the matrix on the heap with some initial value
Mat2d linarg_mat2DInitA(double value, size_t rows, size_t cols);
// initialize the matrix on the heap to zeros
Mat2d linarg_mat2DInitZerosA(size_t rows, size_t cols);
// initialize the matrix on the heap to ones
Mat2d linarg_mat2DInitOnesA(size_t rows, size_t cols);

// make a copy of a matrix on heap
Mat2d linarg_mat2DCopyA(Mat2d matrix);

// construct a matrix from a pointer(does not allocate)
Mat2d linarg_mat2DConstruct(double* ptr, size_t rows, size_t cols);

// pretty print a matrix
void linarg_mat2DPrint(Mat2d a);

// gets the value at row and col in a matrix(by value)
// checks for out-of-bounds
double linarg_mat2DGet(Mat2d a, size_t row, size_t col);
// gets the value at row and col in a matrix(by ref)
// checks for out-of-bounds( returns nullptr is performed )
double* linarg_mat2DRef(Mat2d a, size_t row, size_t col);

// get a row as vector(by ref)
// Warning: this is a copy by reference
// DO NOT USE after matrix is freed
Vec linarg_mat2DRow(Mat2d matrix, size_t row);
// get a column as vector(by ref)
// Warning: this is a copy by reference
// DO NOT USE after matrix is freed
Vec linarg_mat2DCol(Mat2d matrix, size_t col);

// add 2 matrixes and get result into another matrix, prints error if input is invalid
int linarg_mat2DAdd(Mat2d a, Mat2d b, Mat2d* result);
// multiply scalar value to matrixs and get result into another matrix, prints error if input is invalid
int linarg_mat2DScale(double a, Mat2d b, Mat2d* result);

// maximum value in the matrix, prints error if input is invalid
double linarg_mat2DMax(Mat2d a);
// minimum value in the matrix, prints error if input is invalid
double linarg_mat2DMin(Mat2d a);

// free the matrix on the heap
void linarg_freeMat2D(Mat2d* mat);




