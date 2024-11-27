# EE2503-NMDM
This repo contains codes and Final Project Submission of the Group <b><em>Ghanta Labs</em></b> for the Course <em>EE2503- Numerical Methods for Device Modeling</em>.

All the members of this project are requested to write documentation as they write or test the code, so as to ensure a nice time for everyone involved.

Mihir : @Soham, please take care of the makefile.

Soham : Sure, done.

## Guidelines
1. Please write comments on top of a function describing the function's input, output and quirks.
2. Try not to use heap allocation, if you do then any heap allocation must be freed at the end of the function.
3. If the heap memory should be useable beyond the function then suffix the function with a 'A' indicating it allocates memory.
4. Every allocating function must have a complementary free function.
5. The free function MUST zero out the given pointer.
6. Write straightforward code, we can always optimize later.
7. Avoid repeating similar code more than 2 times, instead make it it's own function(if it makes sense logically).
8. Important Variables and Function names must be descriptive.
9. all #defines must be capital case.
10. Prefix functions with name of their group of functions.
11. Every header file should start with a `#pragma once`.

Example:
```C
// larg stands for linear algebra

typedef struct VecX {
    long double* x;
    size_t dim;
} VecX;

// initialize vector on HEAP, initialized to zero.
VecX larg_initVecXA(size_t dim) {
    VecX vecx;
    
    vecx.x = (long double*)malloc(dim * sizeof(long double));
    vecx.dim = dim;

    for(size_t i = 0; i < dim; i++) vecx.x[i] = 0.0;
    
    return vecx;
}

// free vector on HEAP.
VecX larg_freeVecX(VecX* vecx) {
    free(vecx->x);
    vecx->x = NULL;
    vecx->dim = 0;
}

// ...
```

## Compiling
```bash
make
```
This will compile the project.

```bash
make release
```
This will compile the project, with maximum optimization(slow compiling).

```bash
make test
```
This will compile the test cases.

## Running
Give the executables perimission to run:
```bash
chmod +x ./run
chmod +x ./check
```
Then
```bash
./run
```
will run the executable and,
```bash
./check
```
will run the test cases.


## Pro Tips 

1. If you are facing any floating point errors while running, and you suspect it could be from linalg, put a breakpoint near the printf statement in [this file](src/linarg/common.c), and run the debugger. Call Stack in the debug window will show the Traceback of the error.


## Tasks to do for this project

| Task |   | Status |
|:---------|:---------------:|:-----------|
| Interpolation, File Management | Prajwal | TODO |
| Matrix Stuff, Algorithm Research | Soham | TODO |
| *Poisson's Eqn* | *Mihir* | (Done) | 
| Steady State Solver| Manoj | TODO |
| Initial Value Problem (Master's Equation)| Siddhartha | TODO |
| Coefficient Generator | Kaustubh | TODO |


