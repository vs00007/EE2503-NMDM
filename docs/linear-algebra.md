# Linear Algebra Vector Library Documentation

This document describes the functions provided in the Larg Vector Library for vector manipulation. All vectors are represented using a `Vec` structure, which holds a pointer to the vector data (`x`) and its length (`len`).

## Constants

- `LARG_OK`: Indicates a successful operation.
- `LARG_ERROR`: Indicates an error during the operation.

## Function Descriptions

### `Vec larg_vecInitA(double value, size_t len)`
Initializes a vector of length `len` on the heap, with each element set to `value`.

- **Parameters:**
  - `value`: The initial value for each element in the vector.
  - `len`: The length of the vector.
  
- **Returns:** A `Vec` structure with allocated memory, initialized with the provided `value`.

### `Vec larg_vecInitZerosA(size_t len)`
Initializes a vector of length `len` on the heap, with all elements set to `0.0`.

- **Parameters:**
  - `len`: The length of the vector.
  
- **Returns:** A `Vec` structure with allocated memory, initialized to zeros.

### `Vec larg_vecInitOnesA(size_t len)`
Initializes a vector of length `len` on the heap, with all elements set to `1.0`.

- **Parameters:**
  - `len`: The length of the vector.
  
- **Returns:** A `Vec` structure with allocated memory, initialized to ones.

### `Vec larg_vecCopyA(Vec vector)`
Creates a deep copy of the input `vector`.

- **Parameters:**
  - `vector`: The vector to be copied.
  
- **Returns:** A `Vec` structure containing a copy of the input vector.

### `Vec larg_vecConstruct(double* ptr, size_t len)`
Constructs a `Vec` using a pointer to pre-existing data. This function does **not** allocate memory.

- **Parameters:**
  - `ptr`: Pointer to the pre-existing data.
  - `len`: The length of the vector.
  
- **Returns:** A `Vec` structure pointing to the given data.

### `void larg_vecPrint(Vec a)`
Prints the vector in a human-readable format.

- **Parameters:**
  - `a`: The vector to be printed.
  
- **Returns:** Nothing (prints to standard output).

### `int larg_vecAdd(Vec a, Vec b, Vec* result)`
Adds two vectors element-wise and stores the result in `result`.

- **Parameters:**
  - `a`: First input vector.
  - `b`: Second input vector.
  - `result`: Pointer to the vector where the result will be stored.
  
- **Returns:** 
  - `LARG_OK` if the addition is successful.
  - `LARG_ERROR` if the vectors have different lengths, or if any vector is `NULL`. The error is printed to stdout.

### `int larg_vecScale(double a, Vec b, Vec* result)`
Scales the vector `b` by the scalar `a`, storing the result in `result`.

- **Parameters:**
  - `a`: Scalar value.
  - `b`: Input vector.
  - `result`: Pointer to the vector where the result will be stored.
  
- **Returns:**
  - `LARG_OK` if the scaling is successful.
  - `LARG_ERROR` if any vector is `NULL`. The error is printed to stdout.

### `int larg_vecNormalize(Vec a, Vec* result)`
Normalizes the input vector `a` to unit length and stores the result in `result`.

- **Parameters:**
  - `a`: Input vector.
  - `result`: Pointer to the vector where the normalized result will be stored.
  
- **Returns:** 
  - `LARG_OK` if successful.
  - `LARG_ERROR` if an error occurs. The error is printed to stdout.

### `double larg_vecDot(Vec a, Vec b)`
Computes the dot product between two vectors `a` and `b`.

- **Parameters:**
  - `a`: First input vector.
  - `b`: Second input vector.
  
- **Returns:**
  - The dot product as a `double`.
  - `NAN` if the vectors have different lengths or if any vector is `NULL`. The error is printed to stdout.

### `double larg_vecMagnitude(Vec a)`
Calculates the L2 norm (magnitude) of the vector `a`.

- **Parameters:**
  - `a`: Input vector.
  
- **Returns:** The magnitude of the vector.

### `double larg_vecNorm(Vec a, double p)`
Computes the L_p norm of vector `a`. If `p < 1`, a warning is printed.

- **Parameters:**
  - `a`: Input vector.
  - `p`: The norm's order (L_p norm).
  
- **Returns:**
  - The L_p norm as a `double`.
  - `NAN` if `p < 1` or the input vector is `NULL`. The error is printed to stdout.

### `double larg_vecMax(Vec a)`
Finds the maximum value in the vector `a`.

- **Parameters:**
  - `a`: Input vector.
  
- **Returns:** 
  - The maximum value in the vector.
  - `NAN` if the vector is `NULL`.  The error is printed to stdout.
  - `INFINITY` if the vector length is zero. A warning is printed to stdout.

### `double larg_vecMin(Vec a)`
Finds the minimum value in the vector `a`.

- **Parameters:**
  - `a`: Input vector.
  
- **Returns:** 
  - The minimum value in the vector.
  - `NAN` if the vector is `NULL`. The error is printed to stdout.
  - `-INFINITY` if the vector length is zero.A warning is printed to stdout.

### `void larg_freeVecX(Vec* vec)`
Frees the memory allocated for the vector `vec`.

- **Parameters:**
  - `vec`: Pointer to the vector to be freed.
  
- **Returns:** Nothing (frees the allocated memory and resets the vector).

### Usage Notes
- All functions that return an integer (`int`) will return `LARG_OK` on success and `LARG_ERROR` on failure.
- When creating or copying vectors, ensure to call `larg_freeVecX()` to free allocated memory and avoid memory leaks.
