#include "include/linalg.h"

#include <stdlib.h>
#include <memory.h>
#include <math.h>

// LINALG_UNPACK_MAT(matrix, r, c)[x][y] = value at xth col and yth row
#define LA_UNPACK(matrix) ((long double (*)[matrix.cols]) matrix.mat)

#define LA_UNPACK_PTR(matrix) ((long double (*)[matrix->cols]) matrix->mat)

#define LA_UNPACK_ROW(matrix, row) mat2DRow(matrix, row)
#define LA_UNPACK_COL(matrix, col) mat2DCol(matrix, col)

// initialzie the matrix on the heap with some initial value
Mat2d mat2DInitA(long double value, size_t rows, size_t cols)
{
    if(rows == 0 || cols == 0)
    {
        LINALG_REPORT_ERROR("invalid zero row or col matrix requested!");
        return (Mat2d){ NULL, 0, 0 };
    }
    Mat2d mat = { (long double*)calloc(rows*cols, sizeof(long double)), rows, cols };
    LINALG_ASSERT_ERROR(!mat.mat, mat, "unkown error occured when allocation memory!");
    for(size_t i = 0; i < mat.cols*mat.rows; i++) mat.mat[i] = value;

    return mat;
}
// initialize the matrix on the heap to zeros
Mat2d mat2DInitZerosA(size_t rows, size_t cols)
{
    return mat2DInitA(0.0, rows, cols);
}
// initialize the matrix on the heap to ones
Mat2d mat2DInitOnesA(size_t rows, size_t cols)
{
    return mat2DInitA(1.0, rows, cols);
}

// make a copy of a matrix on heap
Mat2d mat2DCopyA(Mat2d matrix)
{
    if(matrix.rows == 0 || matrix.cols == 0)
    {
        LINALG_REPORT_ERROR("invalid zero row or col matrix requested!");
        return (Mat2d){ NULL, 0, 0 };
    }
    else if(matrix.mat == NULL)
    {
        LINALG_REPORT_ERROR("invalid matrix pointer(null)!");
        return (Mat2d){ NULL, 0, 0 };
    }
    Mat2d mat = { (long double*)calloc(matrix.rows*matrix.cols, sizeof(long double)), matrix.rows, matrix.cols };
    LINALG_ASSERT_ERROR(!mat.mat, mat, "unkown error occured when allocation memory!");
    for(size_t i = 0; i < mat.cols*mat.rows; i++) mat.mat[i] = matrix.mat[i];
    return mat;
}

// construct a matrix from a pointer(does not allocate)
Mat2d mat2DConstruct(long double* ptr, size_t rows, size_t cols)
{
    Mat2d mat = { ptr, rows, cols };
    return mat;
}

// pretty print a matrix
void mat2DPrint(Mat2d a)
{
    printf("[\n");
    for(size_t i = 0; i < a.rows; i++)
    {
        printf("%2c", ' ');
        vecPrint(LA_UNPACK_ROW(a, i));
        if(i != a.rows - 1) printf(",\n");
        else printf("\n");
    }
    printf("]");
}

// get a row as vector
// Warning: this is a copy by reference
// DO NOT USE after matrix is freed
Vec mat2DRow(Mat2d matrix, size_t row)
{
    // rows are stored continuously in buffer
    Vec vrow = {&(LA_UNPACK(matrix)[row][0]), matrix.cols, 1};
    return vrow;
}
// get a column as vector
// Warning: this is a copy by reference
// DO NOT USE after matrix is freed
Vec mat2DCol(Mat2d matrix, size_t col)
{
    // cols are not stored continuously in buffer, each value is stored at a offset of matrix.cols
    Vec vcol = {&(LA_UNPACK(matrix)[0][col]), matrix.rows, matrix.cols};
    return vcol;
}

// gets the nth value in a matrix(by value)
// checks for out-of-bounds
long double mat2DGet(Mat2d a, size_t row, size_t col)
{
    LINALG_ASSERT_ERROR(row >= a.rows, NAN, "out of bounds matrix row access!");
    LINALG_ASSERT_ERROR(col >= a.cols, NAN, "out of bounds matrix col access!");

    return LA_UNPACK(a)[row][col];
}
// gets the nth value in a matrix(by ref)
// checks for out-of-bounds( returns nullptr is performed )
long double* mat2DRef(Mat2d a, size_t row, size_t col)
{
    LINALG_ASSERT_ERROR(row >= a.rows, NULL, "out of bounds matrix row access!");
    LINALG_ASSERT_ERROR(col >= a.cols, NULL, "out of bounds matrix col access!");

    return &(LA_UNPACK(a)[row][col]);
}

// add 2 matrixs and get result into another matrix, prints error if input is invalid
int mat2DAdd(Mat2d a, Mat2d b, Mat2d* result)
{
    LINALG_ASSERT_ERROR(!result, LINALG_ERROR, "result matrix is null!");
    LINALG_ASSERT_ERROR(!result->mat, LINALG_ERROR, "result matrix is null!");
    LINALG_ASSERT_ERROR(a.rows != b.rows || b.cols != a.cols, LINALG_ERROR, "attempt to add mat(%zux%zu) and mat(%zux%zu)", a.cols, a.rows, b.cols, b.rows);
    LINALG_ASSERT_ERROR(a.rows != result->rows || b.cols != result->cols, LINALG_ERROR, "result matrix is mat(%zux%zu) but inputs are mat(%zux%zu)", result->cols, result->rows, b.cols, b.rows);
    LINALG_WARN_IF(mat2DContainsNan(a), "input matrix contains INF or NAN!");
    LINALG_WARN_IF(mat2DContainsNan(b), "input matrix contains INF or NAN!");

    for(size_t i = 0; i < a.rows*a.cols; i++) result->mat[i] = a.mat[i] + b.mat[i];

    return LINALG_OK;
}
// subtract 2 matrixes(a-b) and get result into another matrix, prints error if input is invalid
int mat2DSub(Mat2d a, Mat2d b, Mat2d* result)
{
    LINALG_ASSERT_ERROR(!result, LINALG_ERROR, "result matrix is null!");
    LINALG_ASSERT_ERROR(!result->mat, LINALG_ERROR, "result matrix is null!");
    LINALG_ASSERT_ERROR(a.rows != b.rows || b.cols != a.cols, LINALG_ERROR, "attempt to add mat(%zux%zu) and mat(%zux%zu)", a.cols, a.rows, b.cols, b.rows);
    LINALG_ASSERT_ERROR(a.rows != result->rows || b.cols != result->cols, LINALG_ERROR, "result matrix is mat(%zux%zu) but inputs are mat(%zux%zu)", result->cols, result->rows, b.cols, b.rows);
    LINALG_WARN_IF(mat2DContainsNan(a), "input matrix contains INF or NAN!");
    LINALG_WARN_IF(mat2DContainsNan(b), "input matrix contains INF or NAN!");

    for(size_t i = 0; i < a.rows*a.cols; i++) result->mat[i] = a.mat[i] - b.mat[i];

    return LINALG_OK;
}
// multiply scalar value to matrixs and get result into another matrix, prints error if input is invalid
int mat2DScale(long double a, Mat2d b, Mat2d* result)
{
    LINALG_ASSERT_ERROR(!result, LINALG_ERROR, "result matrix is null!");
    LINALG_ASSERT_ERROR(!result->mat, LINALG_ERROR, "result matrix is null!");
    LINALG_ASSERT_ERROR(b.rows != result->rows || b.cols != result->cols, LINALG_ERROR, "result matrix is mat(%zux%zu) but inputs are mat(%zux%zu)", result->cols, result->rows, b.cols, b.rows);
    LINALG_WARN_IF(mat2DContainsNan(b), "input matrix contains INF or NAN!");

    for(size_t i = 0; i < b.rows*b.cols; i++) result->mat[i] = a * b.mat[i];

    LINALG_WARN_IF(mat2DContainsNan(*result), "input matrix contains INF or NAN!");

    return LINALG_OK;
}

// compute result = Ax. prints error if the input is invalid
int mat2DTransform(Mat2d A, Vec x, Vec* result)
{
    LINALG_ASSERT_ERROR(!result, LINALG_ERROR, "result matrix is null!");
    LINALG_ASSERT_ERROR(!result->x, LINALG_ERROR, "result matrix is null!");
    LINALG_ASSERT_ERROR(A.cols != x.len, LINALG_ERROR, "invalid vector: mat(%zux%zu) applied over vec(%zu)", A.rows, A.cols, x.len);
    LINALG_ASSERT_ERROR(A.rows != result->len, LINALG_ERROR, "invalid vector: mat(%zux%zu) applied over vec(%zu) is put in vec(%zu)", A.rows, A.cols, x.len, result->len);
    LINALG_WARN_IF(mat2DContainsNan(A), "input matrix contains INF or NAN!");
    LINALG_WARN_IF(vecContainsNan(x), "input vector contains INF or NAN!");

    for(size_t i = 0; i < A.rows; i++)
    {
        long double val = 0;
        for(size_t j = 0; j < A.cols; j++) val += LA_UNPACK(A)[i][j] * VEC_INDEX(x, j);
        VEC_INDEX(*result, i) = val;
    }

    return LINALG_OK;
}
// compute result = Ax. prints error if the input is invalid(allocates memory)
Vec mat2DTransformA(Mat2d A, Vec x)
{
    Vec badVec = {NULL, 0, 0};
    LINALG_ASSERT_ERROR(A.cols != x.len, badVec, "invalid vector: mat(%zux%zu) applied over vec(%zu)", A.rows, A.cols, x.len);
    LINALG_WARN_IF(mat2DContainsNan(A), "input matrix contains INF or NAN!");
    LINALG_WARN_IF(vecContainsNan(x), "input vector contains INF or NAN!");

    Vec result = vecInitZerosA(A.rows);

    for(size_t i = 0; i < A.rows; i++)
    {
        long double val = 0;
        for(size_t j = 0; j < A.cols; j++) val += LA_UNPACK(A)[i][j] * VEC_INDEX(x, j);
        VEC_INDEX(result, i) = val;
    }

    return result;
}

// compute result = A*B. prints error if the input is invalid
int mat2DMul(Mat2d A, Mat2d B, Mat2d* result)
{
    LINALG_ASSERT_ERROR(!result, LINALG_ERROR, "result matrix is null!");
    LINALG_ASSERT_ERROR(!result->mat, LINALG_ERROR, "result matrix is null!");
    LINALG_ASSERT_ERROR(A.cols != B.rows, LINALG_ERROR, "invalid operation: multiplication between mat(%zux%zu) and mat(%zux%zu)", A.rows, A.cols, B.rows, B.cols);
    LINALG_ASSERT_ERROR(A.rows != result->rows || B.cols != result->cols, LINALG_ERROR, 
                        "invalid operation: multiplication between mat(%zux%zu) and mat(%zux%zu) stored in mat(%zux%zu)", A.rows, A.cols, B.rows, B.cols, result->rows, result->cols);

    LINALG_WARN_IF(mat2DContainsNan(A), "input matrix contains INF or NAN!");
    LINALG_WARN_IF(mat2DContainsNan(B), "input matrix contains INF or NAN!");

    for(size_t i = 0; i < A.rows; i++)
    {
        for(size_t j = 0; j < B.cols; j++)
        {
            long double res = 0;
            for(size_t k = 0; k < B.rows; k++) res += LA_UNPACK(A)[i][k] * LA_UNPACK(B)[k][j];
            LA_UNPACK_PTR(result)[i][j] = res;
        }
    }

    return LINALG_OK;
}
// compute result = A*B(allocates memory). prints error if the input is invalid
Mat2d mat2DMulA(Mat2d A, Mat2d B)
{
    Mat2d bad_mat = {NULL, 0, 0};
    LINALG_ASSERT_ERROR(A.cols != B.rows, bad_mat, "invalid operation: multiplication between mat(%zux%zu) and mat(%zux%zu)", A.rows, A.cols, B.rows, B.cols);
    LINALG_WARN_IF(mat2DContainsNan(A), "input matrix contains INF or NAN!");
    LINALG_WARN_IF(mat2DContainsNan(B), "input matrix contains INF or NAN!");

    Mat2d result = mat2DInitZerosA(A.rows, B.cols);

    for(size_t i = 0; i < A.rows; i++)
    {
        for(size_t j = 0; j < B.cols; j++)
        {
            long double res = 0;
            for(size_t k = 0; k < B.rows; k++) res += LA_UNPACK(A)[i][k] * LA_UNPACK(B)[k][j];
            LA_UNPACK(result)[i][j] = res;
        }
    }

    return result;
}

// compute result = A^T. prints error if the input is invalid
int mat2DTranspose(Mat2d A, Mat2d* result)
{
    LINALG_ASSERT_ERROR(!result, LINALG_ERROR, "result matrix is null!");
    LINALG_ASSERT_ERROR(!result->mat, LINALG_ERROR, "result matrix is null!");
    LINALG_ASSERT_ERROR(A.cols != result->rows || A.rows != result->cols, LINALG_ERROR, "invalid operation: multiplication between mat(%zux%zu) and mat(%zux%zu)", A.rows, A.cols, result->rows, result->cols);
    LINALG_WARN_IF(mat2DContainsNan(A), "input matrix contains INF or NAN!");

    for(size_t i = 0; i < A.rows; i++)
    {
        for(size_t j = 0; j < A.cols; j++)
        {
            LA_UNPACK_PTR(result)[j][i] = LA_UNPACK(A)[i][j];
        }
    }

    return LINALG_OK;
}

// maximum value in the matrix, prints error if input is invalid
long double mat2DMax(Mat2d a)
{
    LINALG_ASSERT_ERROR(!a.mat, NAN, "input matrix is null!");
    LINALG_ASSERT_WARN(a.rows*a.cols == 0, -INFINITY, "input matrix is null!");
    LINALG_WARN_IF(mat2DContainsNan(a), "input matrix contains INF or NAN!");

    long double max_value = -INFINITY;

    for(size_t i = 0; i < a.rows*a.cols; i++) max_value = max_value > a.mat[i] ? max_value : a.mat[i];

    return max_value;
}
// maximum abs value in the matrix, prints error if input is invalid
long double mat2DMaxAbs(Mat2d a)
{
    LINALG_ASSERT_ERROR(!a.mat, NAN, "input matrix is null!");
    LINALG_ASSERT_WARN(a.rows*a.cols == 0, -INFINITY, "input matrix is null!");
    LINALG_WARN_IF(mat2DContainsNan(a), "input matrix contains INF or NAN!");

    long double max_value = -INFINITY;

    for(size_t i = 0; i < a.rows*a.cols; i++) max_value = max_value > fabsl(a.mat[i]) ? max_value : fabsl(a.mat[i]);

    return max_value;
}
// minimum value in the matrix, prints error if input is invalid
long double mat2DMin(Mat2d a)
{
    LINALG_ASSERT_ERROR(!a.mat, NAN, "input matrix is null!");
    LINALG_ASSERT_WARN(a.rows*a.cols == 0, INFINITY, "input matrix is null!");
    LINALG_WARN_IF(mat2DContainsNan(a), "input matrix contains INF or NAN!");

    long double min_value = INFINITY;

    for(size_t i = 0; i < a.rows*a.cols; i++) min_value = min_value < a.mat[i] ? min_value : a.mat[i];

    return min_value;
}

// returns 1 if matrix contains nan
int mat2DContainsNan(Mat2d a)
{
    LINALG_ASSERT_ERROR(!a.mat, 0, "input matrix is null!");
    LINALG_ASSERT_WARN(a.rows*a.cols == 0, 0, "input matrix is null!");

    for(size_t i = 0; i < a.rows*a.cols; i++)
    {
        if(isnan(a.mat[i]) || !isfinite(a.mat[i])) return 1;
    }

    return 0;
}

// free the matrix on the heap
void freeMat2D(Mat2d* mat)
{
    if(!mat->mat) return;

    free(mat->mat);
    mat->rows = 0;
    mat->cols = 0;
}



MatTD matTDinitA(size_t len)
{
    if(len == 0)
    {
        LINALG_REPORT_ERROR("invalid zero length vector requested!");
        return (MatTD){{NULL, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}};
    }
    Vec a = { (long double*)calloc(len, sizeof(long double)), len, 1 };
    Vec b = { (long double*)calloc(len, sizeof(long double)), len, 1 };
    Vec c = { (long double*)calloc(len, sizeof(long double)), len, 1 };

    MatTD mat;
    mat.main = b;
    mat.sub = a;
    mat.sup = c;

    //MatTD bad_mat = (MatTD){{NULL, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}};
    // LINALG_ASSERT_ERROR((!b.x || !a.x || !c.x), (MatTD){NULL, NULL, NULL}, "unkown error occured when allocation memory!"); 

    for(size_t i = 0; i < a.len; i++) a.x[i] = 0;
    for(size_t i = 0; i < b.len; i++) b.x[i] = 0;
    for(size_t i = 0; i < c.len; i++) c.x[i] = 0;

    return mat;
}