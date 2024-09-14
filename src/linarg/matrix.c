#include "include/linarg.h"

#include <stdlib.h>
#include <memory.h>
#include <math.h>

// LINARG_UNPACK_MAT(matrix, r, c)[x][y] = value at xth col and yth row
#define LA_UNPACK(matrix) ((float (*)[matrix.rows]) matrix.mat)

#define LA_UNPACK_ROW(matrix, row) linarg_mat2DRow(matrix, row)
#define LA_UNPACK_COL(matrix, col) linarg_mat2DRow(matrix, row)

// initialzie the matrix on the heap with some initial value
Mat2d linarg_mat2DInitA(double value, size_t rows, size_t cols)
{
    Mat2d mat = { (double*)calloc(rows*cols, sizeof(double)), rows, cols };
    LINARG_ASSERT_ERROR(!mat.mat, mat, "unkown error occured when allocation memory!");
    for(size_t i = 0; i < mat.cols*mat.rows; i++) mat.mat[i] = value;

    return mat;
}
// initialize the matrix on the heap to zeros
Mat2d linarg_mat2DInitZerosA(size_t rows, size_t cols)
{
    return linarg_mat2DInitA(0.0, rows, cols);
}
// initialize the matrix on the heap to ones
Mat2d linarg_mat2DInitOnesA(size_t rows, size_t cols)
{
    return linarg_mat2DInitA(1.0, rows, cols);
}

// make a copy of a matrix on heap
Mat2d linarg_mat2DCopyA(Mat2d matrix)
{
    Mat2d mat = { (double*)calloc(matrix.rows*matrix.cols, sizeof(double)), matrix.rows, matrix.cols };
    LINARG_ASSERT_ERROR(!mat.mat, mat, "unkown error occured when allocation memory!");
    for(size_t i = 0; i < mat.cols*mat.rows; i++) mat.mat[i] = matrix.mat[i];
    return mat;
}

// construct a matrix from a pointer(does not allocate)
Mat2d linarg_mat2DConstruct(double* ptr, size_t rows, size_t cols)
{
    Mat2d mat = { ptr, rows, cols };
    return mat;
}

// pretty print a matrix
void linarg_mat2DPrint(Mat2d a)
{
    printf("[");
    for(size_t i = 0; i < a.rows; i++)
    {
        printf("\t");
        linarg_vecPrint(LA_UNPACK_ROW(a, i));
        if(i != a.rows - 1) printf(",\n");
        else printf("\n");
    }
    printf("]");
}

// get a row as vector
// Warning: this is a copy by reference
// DO NOT USE after matrix is freed
Vec linarg_mat2DRow(Mat2d matrix, size_t row)
{
    // rows are stored continuously in buffer
    Vec vrow = {&(LA_UNPACK(matrix)[0][row]), matrix.cols, 1};
    return vrow;
}
// get a column as vector
// Warning: this is a copy by reference
// DO NOT USE after matrix is freed
Vec linarg_mat2DCol(Mat2d matrix, size_t col)
{
    // cols are not stored continuously in buffer, each value is stored at a offset of matrix.cols
    Vec vcol = {&(LA_UNPACK(matrix)[col][0]), matrix.rows, matrix.cols};
    return vcol;
}

// gets the nth value in a matrix(by value)
// checks for out-of-bounds
double linarg_mat2DGet(Mat2d a, size_t row, size_t col)
{
    LINARG_ASSERT_ERROR(row >= a.rows, NAN, "out of bounds matrix row access!");
    LINARG_ASSERT_ERROR(col >= a.cols, NAN, "out of bounds matrix col access!");

    return LA_UNPACK(a)[col][row];
}
// gets the nth value in a matrix(by ref)
// checks for out-of-bounds( returns nullptr is performed )
double* linarg_mat2DRef(Mat2d a, size_t row, size_t col)
{
    LINARG_ASSERT_ERROR(row >= a.rows, NULL, "out of bounds matrix row access!");
    LINARG_ASSERT_ERROR(col >= a.cols, NULL, "out of bounds matrix col access!");

    return &(LA_UNPACK(a)[col][row]);
}

// add 2 matrixs and get result into another matrix, prints error if input is invalid
int linarg_mat2DAdd(Mat2d a, Mat2d b, Mat2d* result)
{
    LINARG_ASSERT_ERROR(!result, LINARG_ERROR, "result matrix is null!");
    LINARG_ASSERT_ERROR(!result->mat, LINARG_ERROR, "result matrix is null!");
    LINARG_ASSERT_ERROR(a.rows != b.rows || b.cols != a.cols, LINARG_ERROR, "attempt to add mat(%xx%x) and mat(%xx%x)", a.cols, a.rows, b.cols, b.rows);
    LINARG_ASSERT_ERROR(a.rows != result->rows || b.cols != result->cols, LINARG_ERROR, "result matrix is mat(%xx%x) but inputs are mat(%xx%x)", result->cols, result->rows, b.cols, b.rows);

    for(size_t i = 0; i < a.rows*a.cols; i++) result->mat[i] = a.mat[i] + b.mat[i];

    return LINARG_OK;
}
// multiply scalar value to matrixs and get result into another matrix, prints error if input is invalid
int linarg_mat2DScale(double a, Mat2d b, Mat2d* result)
{
    LINARG_ASSERT_ERROR(!result, LINARG_ERROR, "result matrix is null!");
    LINARG_ASSERT_ERROR(!result->mat, LINARG_ERROR, "result matrix is null!");
    LINARG_ASSERT_ERROR(b.rows != result->rows || b.cols != result->cols, LINARG_ERROR, "result matrix is mat(%xx%x) but inputs are mat(%xx%x)", result->cols, result->rows, b.cols, b.rows);

    for(size_t i = 0; i < b.rows*b.cols; i++) result->mat[i] = a * b.mat[i];

    return LINARG_OK;
}

// maximum value in the matrix, prints error if input is invalid
double linarg_mat2DMax(Mat2d a)
{
    LINARG_ASSERT_ERROR(!a.mat, NAN, "input matrix is null!");
    LINARG_ASSERT_WARN(a.rows*a.cols == 0, -INFINITY, "input matrix is null!");

    double max_value = -INFINITY;

    for(size_t i = 0; i < a.rows*a.cols; i++) max_value = max_value > a.mat[i] ? max_value : a.mat[i];

    return max_value;
}
// minimum value in the matrix, prints error if input is invalid
double linarg_mat2DMin(Mat2d a)
{
    LINARG_ASSERT_ERROR(!a.mat, NAN, "input matrix is null!");
    LINARG_ASSERT_WARN(a.rows*a.cols == 0, -INFINITY, "input matrix is null!");

    double min_value = -INFINITY;

    for(size_t i = 0; i < a.rows*a.cols; i++) min_value = min_value < a.mat[i] ? min_value : a.mat[i];

    return min_value;
}

// free the matrix on the heap
void linarg_freeMat2D(Mat2d* mat)
{
    if(!mat->mat) return;

    free(mat->mat);
    mat->rows = 0;
    mat->cols = 0;
}


