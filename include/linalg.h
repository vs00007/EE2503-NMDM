#pragma once
#include <stdio.h>

#define LINALG_OK 0
#define LINALG_ERROR 1

// Error Handling

// call this macro to handle errors
// all linalg errors are routed through this macro,
// so by placing a breakpoint in error_handler function code(defined in src/linalg/common.c), 
// we can check the error directly
#define LINALG_REPORT_ERROR(...) \
        printf("Error: "); \
        error_handler(__FILE__, __func__, __LINE__); \
        printf(__VA_ARGS__); \
        printf("\n");

// call this macro to handle warnings
// all linalg errors are routed through this macro,
// so by placing a breakpoint in error_handler function code(defined in src/linalg/common.c), 
// we can check the error directly
#define LINALG_REPORT_WARN(...) \
        printf("Warning: "); \
        error_handler(__FILE__, __func__, __LINE__); \
        printf(__VA_ARGS__); \
        printf("\n");

// if condition is true, then report an error and return ret.
// leave ret blank for void functions
#define LINALG_ASSERT_ERROR(condition, ret, ...) \
        if(condition) \
        { \
            LINALG_REPORT_ERROR(__VA_ARGS__) \
            return ret; \
        }

// if condition is true, then report an warning and return ret.
// leave ret blank for void functions
#define LINALG_ASSERT_WARN(condition, ret, ...) \
        if(condition) \
        { \
            LINALG_REPORT_WARN(__VA_ARGS__) \
            return ret; \
        }

// if condition is true, then report an warning and *NOT* return.
#define LINALG_WARN_IF(condition, ...) \
        if(condition) \
        { \
            LINALG_REPORT_WARN(__VA_ARGS__) \
        }

// gets value at index from vector by reference(dereferenced)
// allows for syntax like: VEC_INDEX(a, 2) = 5;
#define VEC_INDEX(vector, index) *vecRef(vector, index)

// This function is called in the LINALG_REPORT_ERROR and LINALG_ASSERT_ERROR macros
void error_handler(const char* file, const char* function, size_t line_no);

// Vector implementation

// a column vector
typedef struct Vec {
    long double* x;
    size_t len;
    size_t offset;
} Vec;

// initialzie the vector on the heap with some initial value
Vec vecInitA(long double value, size_t len);
// initialize the vector on the heap to zeros
Vec vecInitZerosA(size_t len);
// initialize the vector on the heap to ones
Vec vecInitOnesA(size_t len);

// make a copy of a vector on heap
Vec vecCopyA(Vec vector);

// construct a vector from a pointer(does not allocate)
Vec vecConstruct(long double* ptr, size_t len);

// Concept allow allocation on stack, using alloca(increase stack size?)
// advantages: 
// -> automatic free
// -> faster execution

// pretty print a vector
void vecPrint(Vec a);

// blatantly copied vecPrint which can print units (Only a single char)
void printVecUnits(Vec f_n, char a);

// gets the nth value in a vector(by value)
// handles buffer offsets
// checks for out-of-bounds
long double vecGet(Vec a, size_t n);
// gets the nth value in a vector(by ref)
// handles buffer offsets
// checks for out-of-bounds( returns nullptr for out-of-bound access)
long double* vecRef(Vec a, size_t n);

// add 2 vectors and get result into another vector, prints error if input is invalid
int vecAdd(Vec a, Vec b, Vec* result);
// subtract 2 vectors(a - b) and get result into another vector, prints error if input is invalid
int vecSub(Vec a, Vec b, Vec* result);
// multiply scalar value to vectors and get result into another vector, prints error if input is invalid
int vecScale(long double a, Vec b, Vec* result);
// unit vector of the norm, prints error if input is invalid
int vecNormalize(Vec a, Vec* result);
// get the dot product between 2 variables, prints error if input is invalid
long double vecDot(Vec a, Vec b);
// get the L2 norm of vector, prints error if input is invalid
long double vecMagnitude(Vec a);
// get the L_p norm of vector, prints warning if p < 1, prints error if input is invalid
// for p = inf use vecMax 
long double vecNorm(Vec a, long double p);
// maximum value in the vector, prints error if input is invalid
long double vecMax(Vec a);
// maximum abs value in the vector, prints error if input is invalid
long double vecMaxAbs(Vec a);
// minimum value in the vector, prints error if input is invalid
long double vecMin(Vec a);
// sum all values in a vector
long double vecSum(Vec a);
// return the product of all values in a vector
long double vecProd(Vec a);
// get the range of vector, i.e max - min
// NOTE: if you need to check if vector is (relatively) constant, use vecRangeRelative instead
long double vecRange(Vec a);
// get the (relative) range of vector, i.e (max - min) / min( |max|, |min| )
long double vecRangeRelative(Vec a);
// get the standard deviation of the vector
// NOTE: if you need to check if vector is (relatively) constant, use vecRangeRelative instead
long double vecStandardDeviation(Vec a);

// returns 1 if vec contains a nan
int vecContainsNan(Vec a);

// free the vector on the heap
void freeVec(Vec* vec);

/*=========================*/
// Matrix implementation
/*=========================*/


// Matrix is stored in 1d array
// 0*col ... 1*cols-1: 1st row
// 1*col ... 2*cols-1: 2nd row
// 2*col ... 3*cols-1: 3nd row
// so on
// (row-1)*col ... row*cols-1: last row
typedef struct Mat2d
{
    long double* mat;
    size_t rows;
    size_t cols;
} Mat2d;

// initialzie the matrix on the heap with some initial value
Mat2d mat2DInitA(long double value, size_t rows, size_t cols);
// initialize the matrix on the heap to zeros
Mat2d mat2DInitZerosA(size_t rows, size_t cols);
// initialize the matrix on the heap to ones
Mat2d mat2DInitOnesA(size_t rows, size_t cols);

// make a copy of a matrix on heap
Mat2d mat2DCopyA(Mat2d matrix);

// construct a matrix from a pointer(does not allocate)
Mat2d mat2DConstruct(long double* ptr, size_t rows, size_t cols);

// pretty print a matrix
void mat2DPrint(Mat2d a);

// gets the value at row and col in a matrix(by value)
// checks for out-of-bounds
long double mat2DGet(Mat2d a, size_t row, size_t col);
// gets the value at row and col in a matrix(by ref)
// checks for out-of-bounds( returns nullptr is performed )
long double* mat2DRef(Mat2d a, size_t row, size_t col);

// get a row as vector(by ref)
// Warning: this is a copy by reference
// DO NOT USE after matrix is freed
Vec mat2DRow(Mat2d matrix, size_t row);
// get a column as vector(by ref)
// Warning: this is a copy by reference
// DO NOT USE after matrix is freed
Vec mat2DCol(Mat2d matrix, size_t col);

// add 2 matrixes and get result into another matrix, prints error if input is invalid
int mat2DAdd(Mat2d a, Mat2d b, Mat2d* result);
// subtract 2 matrixes(a-b) and get result into another matrix, prints error if input is invalid
int mat2DSub(Mat2d a, Mat2d b, Mat2d* result);
// multiply scalar value to matrixs and get result into another matrix, prints error if input is invalid
int mat2DScale(long double a, Mat2d b, Mat2d* result);

// compute result = Ax. prints error if the input is invalid
int mat2DTransform(Mat2d A, Vec x, Vec* result);
// compute result = Ax(allocates result Vec). prints error if the input is invalid
Vec mat2DTransformA(Mat2d A, Vec x);

// compute result = A*B. prints error if the input is invalid
int mat2DMul(Mat2d A, Mat2d B, Mat2d* result);
// compute result = A*B(allocates memory). prints error if the input is invalid
Mat2d mat2DMulA(Mat2d A, Mat2d B);

// calculate result = A^k for k > 0; 
// UNIMPLEMENTED
int mat2DSqPowU(Mat2d A, unsigned int k, Mat2d* result);

// compute result = A^T. prints error if the input is invalid
int mat2DTranspose(Mat2d A, Mat2d* result);

// maximum value in the matrix, prints error if input is invalid
long double mat2DMax(Mat2d a);
// maximum abs value in the matrix, prints error if input is invalid
long double mat2DMaxAbs(Mat2d a);
// minimum value in the matrix, prints error if input is invalid
long double mat2DMin(Mat2d a);

// returns 1 if matrix contains nan
int mat2DContainsNan(Mat2d a);

// free the matrix on the heap
void freeMat2D(Mat2d* mat);


// Tridiagonal Matrix 
typedef struct MatTD
{
    Vec sub;
    Vec main;
    Vec sup;
}MatTD;

MatTD matTDinitA(size_t len);