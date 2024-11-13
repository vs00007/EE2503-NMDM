#pragma once
// #include<testmaster.h>
#include<include/master.h>

Vec f(double t, Vec y){
    Vec function = vecInitZerosA(y.len);
    // *(vecRef(function, 0)) = *vecRef(y, 2) + *vecRef(y, 1) + 2;
    // *(vecRef(function, 1)) = *vecRef(y, 2) + *vecRef(y, 0) + 3;
    // *(vecRef(function, 2)) = *vecRef(y, 0) + *vecRef(y, 1) + 7;
    
    // define the rhs function here

    return function;
}

rk45 rkf45_calculator(double h, double t_i, Vec y_i){
    size_t l = y_i.len;

    // double k_0 = h * f(t_i, y_i);

    Vec k_0 = f(t_i, y_i); 
    vecScale(h, k_0, &k_0);


    // double k_1 = h * f(t_i + h * 1 / 4, y_i + k_0 / 4);

    Vec vec11 = vecInitZerosA(l);
    vecScale(0.25, k_0, &vec11);
    vecAdd(y_i, vec11, &vec11);
    Vec k_1 = vecInitZerosA(l);
    vecScale(h, f(t_i + h * 1.0 / 4.0, vec11), &k_1);


    // double k_2 = h * f(t_i + h * 3 / 8, y_i + k_0 * 3 / 32 + k_1 * 9 / 32);

    Vec vec21 = vecInitZerosA(l);
    vecScale(3.0 / 32.0, k_0, &vec21); 
    Vec vec22 = vecInitZerosA(l);
    vecScale(9.0 / 32, k_1, &vec22);  
    vecAdd(y_i, vec21, &vec21);
    vecAdd(vec21, vec22, &vec21);
    Vec k_2 = vecInitZerosA(l);
    vecScale(h, f(t_i + h * 3.0 / 8.0, vec21), &k_2);


    // double k_3 = h * f(t_i + h * 12 / 13, y_i + k_0 * 1932 / 2197 + k_1 * (-7200) / 2197 + k_2 * 7296 / 2197);

    Vec vec31 = vecInitZerosA(l);
    vecScale(1932.0 / 2197.0, k_0, &vec31); 
    Vec vec32 = vecInitZerosA(l);
    vecScale((-7200.0) / 2197.0, k_1, &vec32); 
    Vec vec33 = vecInitZerosA(l);
    vecScale(7296.0 / 2197.0, k_2, &vec33); 
    vecAdd(y_i, vec31, &vec31); 
    vecAdd(vec31, vec32, &vec31); 
    vecAdd(vec31, vec33, &vec31); 
    Vec k_3 = vecInitZerosA(l);
    vecScale(h, f(t_i + h * 12.0 / 13.0, vec31), &k_3);


    // double k_4 = h * f(t_i + h, y_i + k_0 * 439 / 216 + k_1 * (-8) + k_2 * 3680 / 513 + k_3 * (-845) / 4104);

    Vec vec41 = vecInitZerosA(l);
    vecScale(439.0 / 216.0, k_0, &vec41);
    Vec vec42 = vecInitZerosA(l);
    vecScale(-8.0, k_1, &vec42);
    Vec vec43 = vecInitZerosA(l);
    vecScale(3680.0 / 513.0, k_2, &vec43);
    Vec vec44 = vecInitZerosA(l);
    vecScale((-845.0) / 4104.0, k_3, &vec44);
    vecAdd(y_i, vec41, &vec41);
    vecAdd(vec41, vec42, &vec41);
    vecAdd(vec41, vec43, &vec41);
    vecAdd(vec41, vec44, &vec41);
    Vec k_4 = vecInitZerosA(l);
    vecScale(h, f(t_i + h, vec41), &k_4);



    // double k_5 = h * f(t_i + h * 1 / 2, y_i + k_0 * (-8) / 27 + k_1 * 2 + k_2 * (-3544) / 2565 + k_3 * 1859 / 4104 + k_4 * (-11) / 40);

    Vec vec51 = vecInitZerosA(l);
    vecScale(-8.0 / 27.0, k_0, &vec51);
    Vec vec52 = vecInitZerosA(l);
    vecScale(2, k_1, &vec52);
    Vec vec53 = vecInitZerosA(l);
    vecScale(-3544.0 / 2565, k_2, &vec53);
    Vec vec54 = vecInitZerosA(l);
    vecScale(1859.0 / 4104.0, k_3, &vec54);
    Vec vec55 = vecInitZerosA(l);
    vecScale(-11.0 / 40.0, k_4, &vec55);
    vecAdd(vec51, y_i, &vec51);
    vecAdd(vec51, vec52, &vec51);
    vecAdd(vec51, vec53, &vec51);
    vecAdd(vec51, vec54, &vec51);
    vecAdd(vec51, vec55, &vec51);
    Vec k_5 = vecInitZerosA(l);
    vecScale(h, f(t_i + h * 0.5, vec51), &k_5);


    // double y_order4 = y_i + k_0 * 25 / 216 + k_2 * 1408 / 2565 + k_3 * 2197 / 4104 + k_4 * (-1) / 5;

    Vec y_4_1 = vecInitZerosA(l);
    vecScale(25.0 / 216.0, k_0, &y_4_1);
    Vec y_4_2 = vecInitZerosA(l);
    vecScale(1408.0 / 2565.0, k_2, &y_4_2);
    Vec y_4_3 = vecInitZerosA(l);
    vecScale(2197.0 / 4104.0, k_3, &y_4_3);
    Vec y_4_4 = vecInitZerosA(l);
    vecScale(-0.2, k_4, &y_4_4);
    vecAdd(y_4_1, y_i, &y_4_1);
    vecAdd(y_4_1, y_4_2, &y_4_1);
    vecAdd(y_4_1, y_4_3, &y_4_1);
    vecAdd(y_4_1, y_4_4, &y_4_1);


    // double y_order5 = y_i + k_0 * 16 / 135 + k_2 * 6656 / 12825 + k_3 * 28561 / 56430 + k_4 * (-9) / 50 + k_5 * 2 / 55;


    Vec y_5_1 = vecInitZerosA(l);
    vecScale(16.0 / 135.0, k_0, &y_5_1);
    Vec y_5_2 = vecInitZerosA(l);
    vecScale(6656.0 / 12825.0, k_2, &y_5_2);
    Vec y_5_3 = vecInitZerosA(l);
    vecScale(28561.0 / 56430.0, k_3, &y_5_3);
    Vec y_5_4 = vecInitZerosA(l);
    vecScale(-0.18, k_4, &y_5_4);
    Vec y_5_5 = vecInitZerosA(l);
    vecScale(2.0 / 55.0, k_5, &y_5_5);
    vecAdd(y_5_1, y_i, &y_5_1);
    vecAdd(y_5_1, y_5_2, &y_5_1);
    vecAdd(y_5_1, y_5_3, &y_5_1);
    vecAdd(y_5_1, y_5_4, &y_5_1);
    vecAdd(y_5_1, y_5_5, &y_5_1);

    Vec error = vecInitZerosA(l);
    vecSub(y_5_1, y_4_1, &error);
    for(size_t i = 0; i < l; i++){
        VEC_INDEX(error, i) = fabs(VEC_INDEX(error, i));
    }

    freeVec(&vec11);
    freeVec(&vec21);
    freeVec(&vec22);
    freeVec(&vec31);
    freeVec(&vec32);
    freeVec(&vec33);
    freeVec(&vec41);
    freeVec(&vec42);
    freeVec(&vec43);
    freeVec(&vec44);
    freeVec(&vec51);
    freeVec(&vec52);
    freeVec(&vec53);
    freeVec(&vec54);
    freeVec(&vec55);
    freeVec(&y_4_2);
    freeVec(&y_4_3);
    freeVec(&y_4_4);
    freeVec(&y_5_2);
    freeVec(&y_5_3);
    freeVec(&y_5_4);
    freeVec(&y_5_5);
    rk45 res;
    res.y_5 = y_5_1;
    res.err = error;
    return res;
}


void solver(double h, double t_initial, double t_final, Vec y_initial, double TOL, Vec t_res, Mat2d res){
    size_t l = y_initial.len;
    VEC_INDEX(t_res, 0) = t_initial;
    Vec col1 = mat2DCol(res, 0);


    for(size_t i = 0; i < l; i++){
        VEC_INDEX(col1, i) = VEC_INDEX(y_initial, i);
    }


    size_t n = 0;
    while (VEC_INDEX(t_res, n) < t_final && n < N - 2){
        h = fmin(h, t_final - VEC_INDEX(t_res, n));

        Vec y = mat2DCol(res, n);

        rk45 tuple = rkf45_calculator(h, VEC_INDEX(t_res, n), y);
        Vec y5 = tuple.y_5;
        Vec error = tuple.err;

        Vec col = mat2DCol(res, n+1);
        double errmax = vecMax(error);
        if(errmax < TOL){

            for(size_t i = 0; i < l; i++){
                VEC_INDEX(col, i) = VEC_INDEX(y5, i);
            }
            VEC_INDEX(t_res, n + 1) = VEC_INDEX(t_res, n) + h;
            n++;
        }

        h *= 0.9 * fmax(0.5, fmin(2, 0.9 * pow((TOL / errmax), 1.0 / 5)));
    }
}
