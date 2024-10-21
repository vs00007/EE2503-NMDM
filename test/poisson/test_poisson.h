#include <include/linalg.h>
#include <include/poisson.h>

void test_poisson();

void printVecUnits(Vec f_n, char a);

void getV();

void randF(Vec param, double min, double max, int sort);

int compareDoubles(double a, double b, double epsilon);

int qsort_compare(const void *a, const void *b);