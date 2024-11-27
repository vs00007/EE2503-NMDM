#include<stdio.h>
#include<math.h>
#include<include/poisson.h>
#include<include/inputs.h>
#include<stdlib.h>
#include<string.h>
#include<include/linalg.h>
#include<stdint.h>


long double d_nm(size_t n , size_t m , InputData input_data) ;


Mat2d matrix_d_nm(InputData input_data);


Mat2d matrix_E_n(InputData input_data, Vec mesh);


long double r_nm(InputData input_data , Mat2d mat_E , Mat2d mat_d , size_t n , size_t m);


Mat2d matrix_r_nm(InputData input_data , Mat2d mat_E , Mat2d mat_d);


long double transmission_param(long double T_b , InputData input_data , long double V_electrode);


Mat2d R_en(InputData input_data, Vec mesh);
