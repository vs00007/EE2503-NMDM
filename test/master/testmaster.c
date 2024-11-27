#pragma once
#include "testmaster.h"
#include "include/linalg.h"
#include <include/master.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


void testmaster(){
    FILE * f1 = fopen("test/master/coupled1.txt", "w");
    FILE * f2 = fopen("test/master/coupled2.txt", "w");
    FILE * f3 = fopen("test/master/coupled3.txt", "w");
    size_t n = 3;

    long double h = 0.01;
    long double t_initial = 0;
    long double t_final = 2;
    // printf("%Lf\n%Lf\n", t_final, t_initial);
    Vec y_initial = vecInitOnesA(n);
    Vec t_res = vecInitZerosA(N);
    // long double h2 = 0.01;
    Mat2d res = mat2DInitZerosA(n, N);
    // solver(h, t_initial, t_final, y_initial, 1e-8, t_res, res);
    
    // printf("%zu\n", 1);    

    size_t end1 = N-1;
    // printf("%zu\n", end1);
    while(vecGet(t_res, end1) == 0.0){
        end1 = end1 - 1;
    }
    end1++;
    for(size_t i = 0; i < end1 - 1; i++){
        fprintf(f1, "%Le ", VEC_INDEX(t_res, i));
    }
    fprintf(f1, "%Le\n", VEC_INDEX(t_res, end1 - 1));
    Vec y1 = mat2DRow(res, 0);
    Vec y2 = mat2DRow(res, 1);
    Vec y3 = mat2DRow(res, 2);
    for(size_t i = 0; i < end1 - 1; i++){
        fprintf(f1, "%Le ", VEC_INDEX(y1, i));
    }
    fprintf(f1, "%Le\n", VEC_INDEX(y1, end1 - 1));

    for(size_t i = 0; i < end1 - 1; i++){
        fprintf(f2, "%Le ", VEC_INDEX(t_res, i));
    }
    fprintf(f2, "%Le\n", VEC_INDEX(t_res, end1 - 1));

    for(size_t i = 0; i < end1 - 1; i++){
        fprintf(f2, "%Le ", VEC_INDEX(y2, i));
    }
    
    fprintf(f2, "%Le", VEC_INDEX(y2, end1 - 1));

    for(size_t i = 0; i < end1 - 1; i++){
        fprintf(f3, "%Le ", VEC_INDEX(t_res, i));
    }
    fprintf(f3, "%Le\n", VEC_INDEX(t_res, end1 - 1));
    for(size_t i = 0; i < end1 - 1; i++){
        fprintf(f3, "%Le ", VEC_INDEX(y3, i));
    }
    fprintf(f3, "%Le", VEC_INDEX(y3, end1 - 1));

    fclose(f1);
    fclose(f2);
    // fclose(f3);
}
