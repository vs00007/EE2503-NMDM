# Linear Algebra (linalg) Library Documentation

## Overview

This code defines a basic linear algebra library for working with vectors. It provides macros for error handling and functions for vector operations like addition, scaling, dot product, and norms. Vectors are represented by the `Vec` struct, and most vector operations are performed in-place or return a new vector.

### Constants
- `LINALG_OK`: Used to signify success in a function, with a value of `0`.
- `LINALG_ERROR`: Used to signify failure in a function, with a value of `1`.

### Error Handling(Internal Code Only)

Error and warning macros provide centralized error reporting and debugging:

- `LINALG_REPORT_ERROR(...)`: Reports an error message.
- `LINALG_REPORT_WARN(...)`: Reports a warning message.
- `LINALG_ASSERT_ERROR(condition, ret, ...)`: Checks if a condition is true, reports an error, and returns `ret`.
- `LINALG_ASSERT_WARN(condition, ret, ...)`: Checks if a condition is true, reports a warning, and returns `ret`.

The function `error_handler` is invoked by the macros to handle errors:
```c
void error_handler(const char* file, const char* function, size_t line_no);
```
This function logs the file, function, and line number where the error occurred.

### Vector Representation
The `Vec` struct represents a vector:
```c
typedef struct Vec {
    double* x;     // Pointer to the data
    size_t len;    // Length of the vector
    size_t offset; // Offset for accessing elements
} Vec;
```
Vectors are stored as contiguous arrays of `double` values, with an optional offset for indexing.

## Macros

### `VEC_INDEX(vector, index)`
Gets the value at `index` from a `Vec` instance by dereferencing.
Checks for out of bound access.
```c
#define VEC_INDEX(vector, index) *linalg_vecRef(vector, index)
```

## Functions
Here is the documentation for each function in the provided code:

---

### `Vec linalg_vecInitA(double value, size_t len)`
Initializes a vector on the heap and fills it with a specified value.

#### Parameters:
- `double value`: The value to initialize each element of the vector with.
- `size_t len`: The length of the vector.

#### Returns:
- A `Vec` structure representing the initialized vector.

#### Example:
```c
Vec a = linalg_vecInitA(1.0, 3);  // Initializes [1.0, 1.0, 1.0]
```

---

### `Vec linalg_vecInitZerosA(size_t len)`
Initializes a vector on the heap and fills it with zeros.

#### Parameters:
- `size_t len`: The length of the vector.

#### Returns:
- A `Vec` structure representing the initialized vector.

#### Example:
```c
Vec a = linalg_vecInitZerosA(3);  // Initializes [0.0, 0.0, 0.0]
```

---

### `Vec linalg_vecInitOnesA(size_t len)`
Initializes a vector on the heap and fills it with ones.

#### Parameters:
- `size_t len`: The length of the vector.

#### Returns:
- A `Vec` structure representing the initialized vector.

#### Example:
```c
Vec a = linalg_vecInitOnesA(3);  // Initializes [1.0, 1.0, 1.0]
```

---

### `Vec linalg_vecCopyA(Vec vector)`
Creates a copy of a given vector on the heap.

#### Parameters:
- `Vec vector`: The input vector to be copied.

#### Returns:
- A new `Vec` structure that is a copy of the input vector.

#### Example:
```c
Vec a = linalg_vecCopyA(b);  // Creates a copy of vector `b`.
```

---

### `Vec linalg_vecConstruct(double* ptr, size_t len)`
Constructs a vector from an existing pointer, without allocating memory.

#### Parameters:
- `double* ptr`: A pointer to the array of data.
- `size_t len`: The length of the vector.

#### Returns:
- A `Vec` structure that points to the provided data.

#### Example:
```c
double data[] = {1.0, 2.0, 3.0};
Vec a = linalg_vecConstruct(data, 3);
```

---

### `void linalg_vecPrint(Vec a)`
Prints the vector in a readable format.

#### Parameters:
- `Vec a`: The vector to be printed.

#### Example:
```c
linalg_vecPrint(a);  // Prints [1.0, 2.0, 3.0]
```

---

### `double linalg_vecGet(Vec a, size_t n)`
Gets the value at the nth index in a vector, checking for out-of-bounds access.

#### Parameters:
- `Vec a`: The vector.
- `size_t n`: The index of the value to be retrieved.

#### Returns:
- The value at index `n`, or `NAN` if out-of-bounds.

#### Example:
```c
double val = linalg_vecGet(a, 2);
```

---

### `double* linalg_vecRef(Vec a, size_t n)`
Gets a reference to the value at the nth index in a vector, checking for out-of-bounds access.

#### Parameters:
- `Vec a`: The vector.
- `size_t n`: The index of the value to be retrieved.

#### Returns:
- A pointer to the value at index `n`, or `NULL` if out-of-bounds.

#### Example:
```c
double* val_ptr = linalg_vecRef(a, 2);
```

---

### `int linalg_vecAdd(Vec a, Vec b, Vec* result)`
Adds two vectors element-wise and stores the result in a third vector.

#### Parameters:
- `Vec a`: The first vector.
- `Vec b`: The second vector.
- `Vec* result`: Pointer to the result vector.

#### Returns:
- `LINALG_OK` on success, `LINALG_ERROR` if an error occurs (e.g., dimension mismatch).

#### Example:
```c
linalg_vecAdd(a, b, &result);
```

---

### `int linalg_vecScale(double a, Vec b, Vec* result)`
Multiplies a vector by a scalar and stores the result in another vector.

#### Parameters:
- `double a`: The scalar value.
- `Vec b`: The input vector.
- `Vec* result`: Pointer to the result vector.

#### Returns:
- `LINALG_OK` on success, `LINALG_ERROR` if an error occurs.

#### Example:
```c
linalg_vecScale(2.0, b, &result);  // Scales vector `b` by 2.0
```

---

### `int linalg_vecNormalize(Vec a, Vec* result)`
Normalizes a vector (makes it a unit vector) and stores the result in another vector.

#### Parameters:
- `Vec a`: The input vector.
- `Vec* result`: Pointer to the result vector.

#### Returns:
- `LINALG_OK` on success, `LINALG_ERROR` if an error occurs.

#### Example:
```c
linalg_vecNormalize(a, &result);
```

---

### `double linalg_vecDot(Vec a, Vec b)`
Calculates the dot product of two vectors.

#### Parameters:
- `Vec a`: The first vector.
- `Vec b`: The second vector.

#### Returns:
- The dot product of the two vectors, or `NAN` if an error occurs.

#### Example:
```c
double dot_product = linalg_vecDot(a, b);
```

---

### `double linalg_vecMagnitude(Vec a)`
Calculates the L2 norm (magnitude) of a vector.

#### Parameters:
- `Vec a`: The input vector.

#### Returns:
- The magnitude of the vector.

#### Example:
```c
double magnitude = linalg_vecMagnitude(a);
```

---

### `double linalg_vecNorm(Vec a, double p)`
Calculates the L_p norm of a vector. Prints a warning if `p < 1`.

#### Parameters:
- `Vec a`: The input vector.
- `double p`: The order of the norm.

#### Returns:
- The L_p norm, or `NAN` if an error occurs.

#### Example:
```c
double norm = linalg_vecNorm(a, 2.0);  // Calculates L2 norm
```

---

### `double linalg_vecMax(Vec a)`
Finds the maximum value in a vector.

#### Parameters:
- `Vec a`: The input vector.

#### Returns:
- The maximum value, or `INFINITY` if the vector is empty.

#### Example:
```c
double max_value = linalg_vecMax(a);
```

---

### `double linalg_vecMin(Vec a)`
Finds the minimum value in a vector.

#### Parameters:
- `Vec a`: The input vector.

#### Returns:
- The minimum value, or `-INFINITY` if the vector is empty.

#### Example:
```c
double min_value = linalg_vecMin(a);
```

---

### `void linalg_freeVecX(Vec* vec)`
Frees the memory allocated for a vector.

#### Parameters:
- `Vec* vec`: Pointer to the vector to be freed.

#### Example:
```c
linalg_freeVecX(&a);  // Frees the memory allocated for vector `a`
```

## Usage Examples

```c
Vec a = linalg_vecInitOnesA(5);
Vec b = linalg_vecInitZerosA(5);
linalg_vecAdd(a, b, &result);
linalg_vecPrint(result);
linalg_freeVecX(&a);
linalg_freeVecX(&b);
```
