#pragma once

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<include/linalg.h>

void vecMultiply(Vec a, Vec b, Vec* result);

void jacobianMatrix(Mat2d matrix /*Initialize this matrix to 0*/, Mat2d coeffmatrix/*state to state transmission coefficients*/ , Vec R1 /*electrode 1 coefficients matrix*/, Vec R2 /*electrode 2 coefficients matrix*/,Vec f , int i, int j);

void swap(long double* a, long double*b);

void swapRows(Mat2d A, Vec b, int row1, int row2);

int find_pivot_row(Mat2d A, int start_row, int column);

void gaussianElimination(Mat2d A/*jacobian matrix*/, Vec b/*coefficient vector*/);

void backSubsA(Mat2d A, Vec b, Vec* x);

Vec masterEquationCoeffA(Vec f, Vec R1, Vec R2, Mat2d coeffmatrix);

Vec jacobianImplementationA(Mat2d coeffmatrix, Vec R1, Vec R2);



