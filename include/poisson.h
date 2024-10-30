#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>
#include <errno.h> 

#include <linalg.h>


#define Q 1.602176634e-19
#define M_PI 3.14159265358979323846
#define K 1 / (4 * M_PI * 8.85418782e-12) //Please fix this Mihir
#define EPS0 8.85418782e-12
#define TOL 1e-10

typedef struct OxParams 
{
    double V_0;
    double V_L;
    double eps_r;
    double L;
} OxParams;

int validateInput(const Vec f_n, const Vec d, double x);

int validateTwoVecs(const Vec f_n, const Vec d);

double analyticalPoissonSol(const Vec f_n, const Vec d, double x);

double poissonSolve(const Vec f_n, const Vec d, double x);

Vec getGridV(Vec f_n, Vec d);

Vec getGridE(Vec f_n, Vec d, OxParams params);
