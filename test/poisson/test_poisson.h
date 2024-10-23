#include <include/linalg.h>
#include <include/poisson.h>

void test_poisson();

void test_poisson_full();

void printVecUnits(Vec f_n, char a);

void randF(Vec param, double min, double max, int sort);

int compareDoubles(double a, double b, double epsilon);

int qsort_compare(const void *a, const void *b);

Vec getV(size_t len);

void test_getGridV();