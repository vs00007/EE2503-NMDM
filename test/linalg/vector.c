#include "test/linalg/vector.h"

#define TO_STR(token) #token

#define LINALG_CONS_ARR(arr) vecConstruct((double[])arr, sizeof((double[])arr) / sizeof(double))
#define LINALG_VEC_ARR(arr) (double[])arr, sizeof((double[])arr) / sizeof(double)

#define P(...) __VA_ARGS__
#define P99_PROTECT(...) __VA_ARGS__
#define VECTOR(...) __VA_ARGS__

// assert if the given expressions are not the exact same vector
#define LINALG_ASSERT_EX(a, b) \
        if(!check_vec_exact(a, b)) \
        { \
            printf("assertion failed: %s == %s [exact]\n", TO_STR(a), TO_STR(b)) \
            fail_count++; \
        }

// assert if the given expressions are not the exact same vector(but b is a array)
#define LINALG_ASSERT_EX_ARR(a, ...) \
        if(!check_vec_exact(a, LINALG_CONS_ARR(P99_PROTECT(__VA_ARGS__)))) \
        { \
            printf("assertion failed: %s == %s [exact]\n", TO_STR(a), TO_STR(VECTOR(__VA_ARGS__))); \
            fail_count++; \
        }

#define LINALG_ASSERT_VV_V(func, in1, in2, out) \
        check_vec_VV_V(func, LINALG_VEC_ARR(P(in1)), LINALG_VEC_ARR(P(in2)), LINALG_VEC_ARR(P(out)), __FILE__, #func, __LINE__);

#define LINALG_ASSERT_SV_V(func, in1, in2, out) \
        check_vec_SV_V(func, in1, LINALG_VEC_ARR(P(in2)), LINALG_VEC_ARR(P(out)), __FILE__, #func, __LINE__);

#define LINALG_ASSERT_VV_S(func, in1, in2, out) \
        check_vec_VV_S(func, LINALG_VEC_ARR(P(in1)), LINALG_VEC_ARR(P(in2)), out, __FILE__, #func, __LINE__);

#define LINALG_ASSERT_V_S(func, in1, out) \
        check_vec_V_S(func, LINALG_VEC_ARR(P(in1)), out, __FILE__, #func, __LINE__);


int check_vec_exact(Vec a, Vec b)
{
    if(a.len != b.len) return 0;
    for (size_t i = 0; i < a.len; i++)
    {
        if(VEC_INDEX(a, i) != VEC_INDEX(b, i)) return 0;
    }
    return 1;
}

int check_vec_VV_V(int (*func)(Vec, Vec, Vec*), double* a, size_t alen, double* b, size_t blen, double* out, size_t outlen, const char* file, const char* funcname, size_t line)
{
    Vec result = vecInitZerosA(outlen);
    func(vecConstruct(a, alen), vecConstruct(b, blen), &result);
    if(!check_vec_exact(result, vecConstruct(out, outlen)))
    {
        printf("in file %s, at line %zu\n\t", file, line);
        printf("assertion failed: %s(", funcname);
        vecPrint(vecConstruct(a, alen)); printf(", "); vecPrint(vecConstruct(b, blen));
        printf(") == "); vecPrint(vecConstruct(out, outlen)); printf(" [exact]\n");
        return 1;
    }
    freeVec(&result);
    return 0;
}

int check_vec_SV_V(int (*func)(double, Vec, Vec*), double a, double* b, size_t blen, double* out, size_t outlen, const char* file, const char* funcname, size_t line)
{
    Vec result = vecInitZerosA(outlen);
    func(a, vecConstruct(b, blen), &result);
    if(!check_vec_exact(result, vecConstruct(out, outlen)))
    {
        printf("in file %s, at line %zu\n\t", file, line);
        printf("assertion failed: %s(%f, ", funcname, a);
        vecPrint(vecConstruct(b, blen)); printf(") == "); vecPrint(vecConstruct(out, outlen));
        printf(" [exact]\n");
        return 1;
    }
    freeVec(&result);
    return 0;
}

int check_vec_VV_S(double (*func)(Vec, Vec), double* a, size_t alen, double* b, size_t blen, double out, const char* file, const char* funcname, size_t line)
{
    double result = func(vecConstruct(a, alen), vecConstruct(b, blen));
    if(result != out)
    {
        printf("in file %s, at line %zu\n\t", file, line);
        printf("assertion failed: %s(", funcname);
        vecPrint(vecConstruct(a, alen)); printf(", "); vecPrint(vecConstruct(b, blen));
        printf(") == %f [exact]\n", out);
        return 1;
    }
    return 0;
}

int check_vec_V_S(double (*func)(Vec), double* a, size_t alen, double out, const char* file, const char* funcname, size_t line)
{
    double result = func(vecConstruct(a, alen));
    if(result != out)
    {
        printf("in file %s, at line %zu\n\t", file, line);
        printf("assertion failed: %s(", funcname);
        vecPrint(vecConstruct(a, alen));
        printf(") == %f [exact]\n", out);
        return 1;
    }
    return 0;
}

int linalg_vec_test()
{
    int fail_count = 0;

    Vec a = vecInitZerosA(3);
    LINALG_ASSERT_EX_ARR(a, {0.0, 0.0, 0.0});

    Vec b = vecInitOnesA(3);
    LINALG_ASSERT_EX_ARR(a, {1.0, 1.0, 1.0});

    Vec c = vecConstruct((double[]){0.3, 0.1, 2}, 3);
    LINALG_ASSERT_EX_ARR(a, {0.3, 0.1, 2.0});

    double val = 0.0;

    fail_count += LINALG_ASSERT_VV_V(vecAdd, P({1.0, 2.0}), P({2.0}), P({3.0}));
    fail_count += LINALG_ASSERT_VV_V(vecAdd, P({1.0, 2.0}), P({2.0}), P({3.0}));
    fail_count += LINALG_ASSERT_VV_V(vecAdd, P({1.0, 2.0}), P({2.0}), P({3.0}));

    fail_count += LINALG_ASSERT_SV_V(vecScale, 2.0, P({1.0}), P({2.0}));

    fail_count += LINALG_ASSERT_VV_S(vecDot, P({1.0}), P({1.0}), 2.0);
    
    fail_count += LINALG_ASSERT_V_S(vecMagnitude, P({1.0}), 2.0);
    fail_count += LINALG_ASSERT_V_S(vecMax, P({1.0}), 2.0);
    fail_count += LINALG_ASSERT_V_S(vecMin, P({1.0}), 2.0);

    // free all memory
    freeVec(&a);
    freeVec(&b);

    return fail_count;
}