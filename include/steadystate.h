#pragma once

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<include/linalg.h>

void vecMultiply(Vec a, Vec b, Vec* result);

void jacobian_matrix(Mat2d matrix /*Initialize this matrix to 0*/, Mat2d coeffmatrix/*state to state transmission coefficients*/ , Vec R1 /*electrode 1 coefficients matrix*/, Vec R2 /*electrode 2 coefficients matrix*/,Vec f , int i, int j);

void swap(double* a, double*b);

void swap_rows(Mat2d A, Vec b, int row1, int row2);

int find_pivot_row(Mat2d A, int start_row, int column);

void gaussian_elimination(Mat2d A/*jacobian matrix*/, Vec b/*coefficient vector*/);

void back_substitutionA(Mat2d A, Vec b, Vec* x);

Vec masterequationcoffA(Vec f, Vec R1, Vec R2, Mat2d coeffmatrix);

Vec jacobian_implementationA(Mat2d coeffmatrix, Vec R1, Vec R2);



