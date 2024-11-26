// header files for the ode solver

#include <include/linalg.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define N 1e8

typedef struct rk45
{
    Vec y_5;
    Vec err;
} rk45;


Vec f(long double t, Vec y);

rk45 rkf45_calculator(long double h, long double t_i, Vec y_i);

void solver(long double h, long double t_initial, long double t_final, Vec y_initial, long double TOL, Vec t_res, Mat2d res);