#include <include/linalg.h>
#include <include/poisson.h>

void test_poisson();

void test_poisson_full();

void printVecUnits(Vec f_n, char a);

void randF(Vec param, long double min, long double max, int sort);

int comparelongdoubles(long double a, long double b, long double epsilon);

int qsort_compare(const void *a, const void *b);

Vec getV(size_t len);

void test_getGridV();

void testMeshGen();

void testSolver();