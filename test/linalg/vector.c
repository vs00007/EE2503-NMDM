#include <assert.h>
#include "include/linalg.h"
#include <math.h>

void test_vecInitA() {
    Vec v = vecInitA(5.0, 3);
    assert(v.len == 3);
    for (size_t i = 0; i < v.len; i++) {
        assert(VEC_INDEX(v, i) == 5.0);
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
}

void test_vecInitOnesA() {
    Vec v = vecInitOnesA(4);
    assert(v.len == 4);
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
}

void test_vecDot() {
    Vec a = vecInitA(2.0, 3);
    Vec b = vecInitA(3.0, 3);
    double dot = vecDot(a, b);
    assert(dot == 18.0); // 2*3 + 2*3 + 2*3 = 18
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
}

void test_vecNormalize() {
    Vec a = vecInitA(3.0, 3);
    Vec result = vecInitZerosA(3);
    int status = vecNormalize(a, &result);
    double magnitude = vecMagnitude(a);
    assert(status == LINALG_OK);
    for (size_t i = 0; i < 3; i++) {
        assert(fabs(VEC_INDEX(result, i) - (3.0 / magnitude)) < 1e-6);
    }
    freeVec(&a);
    freeVec(&result);
}

void test_vecMagnitude() {
    Vec a = vecInitA(3.0, 3);
    double magnitude = vecMagnitude(a);
    assert(fabs(magnitude - sqrt(27.0)) < 1e-6); // sqrt(3^2 + 3^2 + 3^2) = sqrt(27)
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
