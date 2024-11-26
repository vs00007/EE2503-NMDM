#include <assert.h>
#include <math.h>
#include <float.h>
#include "include/linalg.h"

void test_vecInitA() {
    Vec v = vecInitA(5.0, 3);
    assert(v.len == 3);
    for (size_t i = 0; i < v.len; i++) {
        assert(VEC_INDEX(v, i) == 5.0);
    }
    freeVec(&v);

    // Test with zero length
    v = vecInitA(1.0, 0);
    assert(v.len == 0);
    assert(v.x == NULL);
    freeVec(&v);

    // Test with large length
    v = vecInitA(2.0, 1000000);
    assert(v.len == 1000000);
    for (size_t i = 0; i < v.len; i++) {
        assert(VEC_INDEX(v, i) == 2.0);
    }
    freeVec(&v);
}

void test_vecInitZerosA() {
    Vec v = vecInitZerosA(4);
    assert(v.len == 4);
    for (size_t i = 0; i < v.len; i++) {
        assert(VEC_INDEX(v, i) == 0.0);
    }
    freeVec(&v);

    // Test with zero length
    v = vecInitZerosA(0);
    assert(v.len == 0);
    assert(v.x == NULL);
    freeVec(&v);
}

void test_vecInitOnesA() {
    Vec v = vecInitOnesA(4);
    assert(v.len == 4);
    for (size_t i = 0; i < v.len; i++) {
        assert(VEC_INDEX(v, i) == 1.0);
    }
    freeVec(&v);

    // Test with large length
    v = vecInitOnesA(1000000);
    assert(v.len == 1000000);
    for (size_t i = 0; i < v.len; i++) {
        assert(VEC_INDEX(v, i) == 1.0);
    }
    freeVec(&v);
}

void test_vecCopyA() {
    Vec v = vecInitA(2.0, 3);
    Vec v_copy = vecCopyA(v);
    assert(v_copy.len == v.len);
    for (size_t i = 0; i < v.len; i++) {
        assert(VEC_INDEX(v_copy, i) == VEC_INDEX(v, i));
    }
    freeVec(&v);
    freeVec(&v_copy);

    // Test copying empty vector
    v = vecInitZerosA(0);
    v_copy = vecCopyA(v);
    assert(v_copy.len == 0);
    assert(v_copy.x == NULL);
    freeVec(&v);
    freeVec(&v_copy);
}

void test_vecAdd() {
    Vec a = vecInitA(2.0, 3);
    Vec b = vecInitA(3.0, 3);
    Vec result = vecInitZerosA(3);
    int status = vecAdd(a, b, &result);
    assert(status == LINALG_OK);
    for (size_t i = 0; i < 3; i++) {
        assert(VEC_INDEX(result, i) == 5.0);
    }
    freeVec(&a);
    freeVec(&b);
    freeVec(&result);

    // Test adding vectors of different lengths
    a = vecInitA(1.0, 2);
    b = vecInitA(2.0, 3);
    result = vecInitZerosA(3);
    status = vecAdd(a, b, &result);
    assert(status == LINALG_ERROR);
    freeVec(&a);
    freeVec(&b);
    freeVec(&result);
}

void test_vecSub() {
    Vec a = vecInitA(5.0, 3);
    Vec b = vecInitA(2.0, 3);
    Vec result = vecInitZerosA(3);
    int status = vecSub(a, b, &result);
    assert(status == LINALG_OK);
    for (size_t i = 0; i < 3; i++) {
        assert(VEC_INDEX(result, i) == 3.0);
    }
    freeVec(&a);
    freeVec(&b);
    freeVec(&result);

    // Test subtracting vectors of different lengths
    a = vecInitA(5.0, 2);
    b = vecInitA(2.0, 3);
    result = vecInitZerosA(3);
    status = vecSub(a, b, &result);
    assert(status == LINALG_ERROR);
    freeVec(&a);
    freeVec(&b);
    freeVec(&result);
}

void test_vecDot() {
    Vec a = vecInitA(2.0, 3);
    Vec b = vecInitA(3.0, 3);
    long double dot = vecDot(a, b);
    assert(dot == 18.0); // 2*3 + 2*3 + 2*3 = 18
    freeVec(&a);
    freeVec(&b);

    // Test dot product of orthogonal vectors
    a = vecInitA(1.0, 3);
    b = vecInitZerosA(3);
    VEC_INDEX(b, 1) = 1.0;
    dot = vecDot(a, b);
    assert(dot == 1.0);
    freeVec(&a);
    freeVec(&b);

    // Test dot product of vectors with different lengths
    a = vecInitA(1.0, 2);
    b = vecInitA(2.0, 3);
    dot = vecDot(a, b);
    assert(isnan(dot));
    freeVec(&a);
    freeVec(&b);
}

void test_vecScale() {
    Vec a = vecInitA(2.0, 3);
    Vec result = vecInitZerosA(3);
    int status = vecScale(2.0, a, &result);
    assert(status == LINALG_OK);
    for (size_t i = 0; i < 3; i++) {
        assert(VEC_INDEX(result, i) == 4.0);
    }
    freeVec(&a);
    freeVec(&result);

    // Test scaling by zero
    a = vecInitA(2.0, 3);
    result = vecInitZerosA(3);
    status = vecScale(0.0, a, &result);
    assert(status == LINALG_OK);
    for (size_t i = 0; i < 3; i++) {
        assert(VEC_INDEX(result, i) == 0.0);
    }
    freeVec(&a);
    freeVec(&result);

    // Test scaling empty vector
    a = vecInitZerosA(0);
    result = vecInitZerosA(0);
    status = vecScale(2.0, a, &result);
    assert(status == LINALG_ERROR);
    assert(result.len == 0);
    assert(result.x == NULL);
    freeVec(&a);
    freeVec(&result);
}

void test_vecNormalize() {
    Vec a = vecInitA(3.0, 3);
    Vec result = vecInitZerosA(3);
    int status = vecNormalize(a, &result);
    long double magnitude = vecMagnitude(a);
    assert(status == LINALG_OK);
    for (size_t i = 0; i < 3; i++) {
        assert(fabsl(VEC_INDEX(result, i) - (3.0 / magnitude)) < 1e-6);
    }
    freeVec(&a);
    freeVec(&result);

    // Test normalizing zero vector
    a = vecInitZerosA(3);
    result = vecInitZerosA(3);
    status = vecNormalize(a, &result);
    assert(status == LINALG_OK);
    freeVec(&a);
    freeVec(&result);

    // Test normalizing unit vector
    a = vecInitZerosA(3);
    VEC_INDEX(a, 0) = 1.0;
    result = vecInitZerosA(3);
    status = vecNormalize(a, &result);
    assert(status == LINALG_OK);
    assert(fabsl(VEC_INDEX(result, 0) - 1.0) < 1e-6);
    assert(fabsl(VEC_INDEX(result, 1)) < 1e-6);
    assert(fabsl(VEC_INDEX(result, 2)) < 1e-6);
    freeVec(&a);
    freeVec(&result);
}

void test_vecMagnitude() {
    Vec a = vecInitA(3.0, 3);
    long double magnitude = vecMagnitude(a);
    assert(fabsl(magnitude - sqrt(27.0)) < 1e-6); // sqrt(3^2 + 3^2 + 3^2) = sqrt(27)
    freeVec(&a);

    // Test magnitude of zero vector
    a = vecInitZerosA(3);
    magnitude = vecMagnitude(a);
    assert(magnitude == 0.0);
    freeVec(&a);

    // Test magnitude of unit vector
    a = vecInitZerosA(3);
    VEC_INDEX(a, 1) = 1.0;
    magnitude = vecMagnitude(a);
    assert(fabsl(magnitude - 1.0) < 1e-6);
    freeVec(&a);
}

int linalg_vec_test() {
    test_vecInitA();
    test_vecInitZerosA();
    test_vecInitOnesA();
    test_vecCopyA();
    test_vecAdd();
    test_vecSub();
    test_vecDot();
    test_vecScale();
    test_vecNormalize();
    test_vecMagnitude();
    printf("vec* functions passed all tests\n");
    return 0;
}