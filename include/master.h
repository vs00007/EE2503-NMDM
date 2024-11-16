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


Vec f(double t, Vec y);

rk45 rkf45_calculator(double h, double t_i, Vec y_i);

void solver(double h, double t_initial, double t_final, Vec y_initial, double TOL, Vec t_res, Mat2d res);