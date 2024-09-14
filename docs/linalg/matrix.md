# Linear Algebra (linalg) 2D Matrix Library Documentation

## Overview

This code provides a linear algebra library for working with 2D matrices. It includes functions for initializing matrices, copying them, performing arithmetic operations, retrieving values, and more. Matrices are represented by the `Mat2d` struct, which holds a pointer to the matrix data and its dimensions (rows and columns).

### Error Handling (Internal Code Only)

Error-handling macros are used to simplify error reporting:

- `LINALG_ASSERT_ERROR(condition, ret, ...)`: Asserts that a condition is met. If not, it logs an error and returns `ret`.

## Matrix Representation

The `Mat2d` struct represents a 2D matrix:
```c
typedef struct Mat2d {
    double* mat;  // Pointer to matrix data (stored in row-major order)
    size_t rows;  // Number of rows
    size_t cols;  // Number of columns
} Mat2d;
```

## Macros (Internal Code Only)

### `LA_UNPACK(matrix)`
Unpacks the matrix data and allows direct access to its elements using array notation. It converts the `mat` field into a 2D array based on the number of rows.
```c
#define LA_UNPACK(matrix) ((float (*)[matrix.rows]) matrix.mat)
```

### `LA_UNPACK_ROW(matrix, row)`
Unpacks a specific row of the matrix and returns a vector representing that row.
```c
#define LA_UNPACK_ROW(matrix, row) linalg_mat2DRow(matrix, row)
```

### `LA_UNPACK_COL(matrix, row)`
Unpacks a specific col of the matrix and returns a vector representing that col.
```c
#define LA_UNPACK_COL(matrix, row) linalg_mat2DCol(matrix, row)
```

## Functions

Here is the documentation for each function in the provided code:

---

### `Mat2d linalg_mat2DInitA(double value, size_t rows, size_t cols)`
Initializes a matrix on the heap, filling each element with the specified value.

#### Parameters:
- `double value`: The value to initialize each matrix element with.
- `size_t rows`: The number of rows in the matrix.
- `size_t cols`: The number of columns in the matrix.

#### Returns:
- A `Mat2d` structure representing the initialized matrix.

#### Example:
```c
Mat2d mat = linalg_mat2DInitA(2.0, 3, 3);  // Initializes a 3x3 matrix with all elements as 2.0
```

---

### `Mat2d linalg_mat2DInitZerosA(size_t rows, size_t cols)`
Initializes a matrix on the heap with all elements set to zero.

#### Parameters:
- `size_t rows`: The number of rows in the matrix.
- `size_t cols`: The number of columns in the matrix.

#### Returns:
- A `Mat2d` structure representing the initialized matrix with zeros.

#### Example:
```c
Mat2d mat = linalg_mat2DInitZerosA(3, 3);  // Initializes a 3x3 matrix of zeros
```

---

### `Mat2d linalg_mat2DInitOnesA(size_t rows, size_t cols)`
Initializes a matrix on the heap with all elements set to one.

#### Parameters:
- `size_t rows`: The number of rows in the matrix.
- `size_t cols`: The number of columns in the matrix.

#### Returns:
- A `Mat2d` structure representing the initialized matrix with ones.

#### Example:
```c
Mat2d mat = linalg_mat2DInitOnesA(3, 3);  // Initializes a 3x3 matrix of ones
```

---

### `Mat2d linalg_mat2DCopyA(Mat2d matrix)`
Creates a copy of a given matrix on the heap.

#### Parameters:
- `Mat2d matrix`: The matrix to be copied.

#### Returns:
- A new `Mat2d` structure that is a copy of the input matrix.

#### Example:
```c
Mat2d copy = linalg_mat2DCopyA(mat);  // Creates a copy of matrix `mat`
```

---

### `Mat2d linalg_mat2DConstruct(double* ptr, size_t rows, size_t cols)`
Constructs a matrix from an existing pointer, without allocating memory.

#### Parameters:
- `double* ptr`: Pointer to the matrix data.
- `size_t rows`: The number of rows.
- `size_t cols`: The number of columns.

#### Returns:
- A `Mat2d` structure that points to the provided data.

#### Example:
```c
double data[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
Mat2d mat = linalg_mat2DConstruct(data, 2, 3);  // Constructs a 2x3 matrix from `data`
```

---

### `void linalg_mat2DPrint(Mat2d a)`
Prints the matrix in a readable format.

#### Parameters:
- `Mat2d a`: The matrix to be printed.

#### Example:
```c
linalg_mat2DPrint(mat);  // Prints the matrix
```

---

### `Vec linalg_mat2DRow(Mat2d matrix, size_t row)`
Returns a vector representing a specific row of the matrix. The vector is a reference, not a copy.

So, this **must** only be used till matrix is not freed.

#### Parameters:
- `Mat2d matrix`: The matrix.
- `size_t row`: The index of the row.

#### Returns:
- A `Vec` structure representing the row.

#### Example:
```c
Vec row_vec = linalg_mat2DRow(mat, 1);  // Gets the second row as a vector
```

---

### `Vec linalg_mat2DCol(Mat2d matrix, size_t col)`
Returns a vector representing a specific column of the matrix. The vector is a reference, not a copy.

So, this **must** only be used till matrix is not freed.

#### Parameters:
- `Mat2d matrix`: The matrix.
- `size_t col`: The index of the column.

#### Returns:
- A `Vec` structure representing the column.

#### Example:
```c
Vec col_vec = linalg_mat2DCol(mat, 1);  // Gets the second column as a vector
```

---

### `double linalg_mat2DGet(Mat2d a, size_t row, size_t col)`
Gets the value at the specified row and column, checking for out-of-bounds access.

#### Parameters:
- `Mat2d a`: The matrix.
- `size_t row`: The row index.
- `size_t col`: The column index.

#### Returns:
- The value at the specified location, or `NAN` if out-of-bounds.

#### Example:
```c
double val = linalg_mat2DGet(mat, 1, 1);  // Gets the value at (1, 1)
```

---

### `double* linalg_mat2DRef(Mat2d a, size_t row, size_t col)`
Gets a reference to the value at the specified row and column, checking for out-of-bounds access.

#### Parameters:
- `Mat2d a`: The matrix.
- `size_t row`: The row index.
- `size_t col`: The column index.

#### Returns:
- A pointer to the value at the specified location, or `NULL` if out-of-bounds.

#### Example:
```c
double* ref = linalg_mat2DRef(mat, 1, 1);  // Gets a reference to the value at (1, 1)
```

---

### `int linalg_mat2DAdd(Mat2d a, Mat2d b, Mat2d* result)`
Adds two matrices element-wise and stores the result in a third matrix.

#### Parameters:
- `Mat2d a`: The first matrix.
- `Mat2d b`: The second matrix.
- `Mat2d* result`: Pointer to the result matrix.

#### Returns:
- `LINALG_OK` on success, `LINALG_ERROR` if an error occurs (e.g., dimension mismatch).

#### Example:
```c
linalg_mat2DAdd(a, b, &result);  // Adds matrices `a` and `b`, stores result in `result`
```

---

### `int linalg_mat2DScale(double a, Mat2d b, Mat2d* result)`
Multiplies a matrix by a scalar and stores the result in another matrix.

#### Parameters:
- `double a`: The scalar value.
- `Mat2d b`: The input matrix.
- `Mat2d* result`: Pointer to the result matrix.

#### Returns:
- `LINALG_OK` on success, `LINALG_ERROR` if an error occurs.

#### Example:
```c
linalg_mat2DScale(2.0, mat, &result);  // Scales matrix `mat` by 2.0
```

---

### `double linalg_mat2DMax(Mat2d a)`
Finds the maximum value in a matrix.

#### Parameters:
- `Mat2d a`: The input matrix.

#### Returns:
- The maximum value, or `-INFINITY` if the matrix is empty.

#### Example:
```c
double max_value = linalg_mat2DMax(mat);  // Finds the maximum value in the matrix
```

---

### `double linalg_mat2DMin(Mat2d a)`
Finds the minimum value in a matrix.

#### Parameters:
- `Mat2d a`: The input matrix.

#### Returns:
- The minimum value, or `-INFINITY` if the matrix is empty.

#### Example:
```c
double min_value = linalg_mat2DMin(mat);  // Finds the minimum value in the matrix
```

---

### `void linalg_freeMat2D(Mat2d* mat)`
Frees the memory allocated

 for a matrix.

#### Parameters:
- `Mat2d* mat`: Pointer to the matrix to be freed.

#### Example:
```c
linalg_freeMat2D(&mat);  // Frees the memory allocated for matrix `mat`
```

## Usage Examples

```c
Mat2d mat = linalg_mat2DInitOnesA(3, 3);
linalg_mat2DPrint(mat);
linalg_freeMat2D(&mat);
```
