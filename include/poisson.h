#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <linalg.h>
#include <stdbool.h>
#include <float.h>
#include <errno.h>

#define Q 1.602176634e-19
#define K 1 / (4 * M_PI * 8.85418782e-12)
#define EPS0 8.85418782e-12
#define TOL 1e-10

double analyticalPoissonSol(const Vec f_n, const Vec d, double x);

int validateInput(const Vec f_n, const Vec d, double x);

double poissonSolve(const Vec f_n, const Vec d, double x);



