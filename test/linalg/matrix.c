#include <assert.h>
#include <math.h>
#include "linalg.h"

void test_mat2DInitA() {
    Mat2d mat = mat2DInitA(5.0, 3, 4);
    assert(mat.rows == 3);
    assert(mat.cols == 4);
    for (size_t i = 0; i < mat.rows * mat.cols; i++) {
        assert(mat.mat[i] == 5.0);
    }
    freeMat2D(&mat);

    // Test with invalid dimensions
    Mat2d invalid_mat = mat2DInitA(1.0, 0, 5);
    assert(invalid_mat.mat == NULL);
    assert(invalid_mat.rows == 0);
    assert(invalid_mat.cols == 0);
}

void test_mat2DInitZerosA() {
    Mat2d mat = mat2DInitZerosA(2, 3);
    assert(mat.rows == 2);
    assert(mat.cols == 3);
    for (size_t i = 0; i < mat.rows * mat.cols; i++) {
        assert(mat.mat[i] == 0.0);
    }
    freeMat2D(&mat);
}

void test_mat2DInitOnesA() {
    Mat2d mat = mat2DInitOnesA(3, 2);
    assert(mat.rows == 3);
    assert(mat.cols == 2);
    for (size_t i = 0; i < mat.rows * mat.cols; i++) {
        assert(mat.mat[i] == 1.0);
    }
    freeMat2D(&mat);
}

void test_mat2DCopyA() {
    Mat2d original = mat2DInitA(2.5, 2, 2);
    Mat2d copy = mat2DCopyA(original);
    assert(copy.rows == original.rows);
    assert(copy.cols == original.cols);
    for (size_t i = 0; i < original.rows * original.cols; i++) {
        assert(copy.mat[i] == original.mat[i]);
    }
    freeMat2D(&original);
    freeMat2D(&copy);

    // Test copying invalid matrix
    Mat2d invalid_mat = {NULL, 0, 0};
    Mat2d invalid_copy = mat2DCopyA(invalid_mat);
    assert(invalid_copy.mat == NULL);
    assert(invalid_copy.rows == 0);
    assert(invalid_copy.cols == 0);
}

void test_mat2DConstruct() {
    long double data[] = {1.0, 2.0, 3.0, 4.0};
    Mat2d mat = mat2DConstruct(data, 2, 2);
    assert(mat.rows == 2);
    assert(mat.cols == 2);
    assert(mat.mat == data);
}

void test_mat2DGet() {
    Mat2d mat = mat2DInitA(0.0, 3, 3);
    *mat2DRef(mat, 1, 2) = 5.5;
    assert(mat2DGet(mat, 1, 2) == 5.5);
    assert(isnan(mat2DGet(mat, 3, 3)));  // Out of bounds access
    freeMat2D(&mat);
}

void test_mat2DRef() {
    Mat2d mat = mat2DInitA(0.0, 3, 3);
    *mat2DRef(mat, 1, 2) = 5.5;
    assert(*mat2DRef(mat, 1, 2) == 5.5);
    assert(mat2DRef(mat, 3, 3) == NULL);  // Out of bounds access
    freeMat2D(&mat);
}

void test_mat2DAdd() {
    Mat2d a = mat2DInitA(1.0, 2, 2);
    Mat2d b = mat2DInitA(2.0, 2, 2);
    Mat2d result = mat2DInitZerosA(2, 2);
    assert(mat2DAdd(a, b, &result) == LINALG_OK);
    for (size_t i = 0; i < a.rows * a.cols; i++) {
        assert(result.mat[i] == 3.0);
    }
    freeMat2D(&a);
    freeMat2D(&b);
    freeMat2D(&result);

    // Test addition with incompatible matrices
    Mat2d c = mat2DInitA(1.0, 2, 3);
    assert(mat2DAdd(a, c, &result) == LINALG_ERROR);
    freeMat2D(&c);
}

void test_mat2DSub() {
    Mat2d a = mat2DInitA(3.0, 2, 2);
    Mat2d b = mat2DInitA(1.0, 2, 2);
    Mat2d result = mat2DInitZerosA(2, 2);
    assert(mat2DSub(a, b, &result) == LINALG_OK);
    for (size_t i = 0; i < a.rows * a.cols; i++) {
        assert(result.mat[i] == 2.0);
    }
    freeMat2D(&a);
    freeMat2D(&b);
    freeMat2D(&result);
}

void test_mat2DScale() {
    Mat2d a = mat2DInitA(2.0, 2, 2);
    Mat2d result = mat2DInitZerosA(2, 2);
    assert(mat2DScale(3.0, a, &result) == LINALG_OK);
    for (size_t i = 0; i < a.rows * a.cols; i++) {
        assert(result.mat[i] == 6.0);
    }
    freeMat2D(&a);
    freeMat2D(&result);
}

void test_mat2DMax() {
    Mat2d mat = mat2DInitA(1.0, 3, 3);
    *mat2DRef(mat, 1, 1) = 5.5;
    *mat2DRef(mat, 2, 2) = -2.0;
    assert(mat2DMax(mat) == 5.5);
    freeMat2D(&mat);

    // Test with empty matrix
    Mat2d empty_mat = {NULL, 0, 0};
    assert(isnan(mat2DMax(empty_mat)));
}

void test_mat2DMin() {
    Mat2d mat = mat2DInitA(1.0, 3, 3);
    *mat2DRef(mat, 1, 1) = 5.5;
    *mat2DRef(mat, 2, 2) = -2.0;
    assert(mat2DMin(mat) == -2.0);
    freeMat2D(&mat);

    // Test with empty matrix
    Mat2d empty_mat = {NULL, 0, 0};
    assert(isnan(mat2DMin(empty_mat)));
}

int linalg_matrix_test() {
    test_mat2DInitA();
    test_mat2DInitZerosA();
    test_mat2DInitOnesA();
    test_mat2DCopyA();
    test_mat2DConstruct();
    test_mat2DGet();
    test_mat2DRef();
    test_mat2DAdd();
    test_mat2DSub();
    test_mat2DScale();
    test_mat2DMax();
    test_mat2DMin();

    printf("mat2D* functions passed all tests\n");
    return 0;
}