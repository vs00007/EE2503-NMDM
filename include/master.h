// header files for the ode solver

#include <include/linalg.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<include/inputs.h>

#define N 1e8

typedef struct rk45
{
    Vec y_5;
    Vec err;
} rk45;

typedef struct RK45Config
{
    long double h;
    long double t_initial;
    long double t_final;
    Vec y_initial;
    long double tol;
    Vec mesh;
    InputData data;
} RK45Config;

Vec f(long double t, Vec y, InputData data, Vec mesh);

rk45 rkf45_calculator(long double h, long double t_i, Vec y_i, InputData data, Vec mesh);

void solver(RK45Config config, Vec t_res, Mat2d res);