#include<stdio.h>
#include<math.h>
#include<include/poisson.h>
#include<include/inputs.h>
#include<stdlib.h>
#include<string.h>
#include<include/linalg.h>
#include<stdint.h>


double d_nm(size_t n , size_t m , InputData input_data) 
{
    return fabs(input_data.locs.x[n] - input_data.locs.x[n]) ;
}

Mat2d matrix_d_nm(InputData input_data)
{
    size_t len = input_data.params.num_traps ;
    size_t i,j ;
    Mat2d Mat_d_nm = mat2DInitZerosA(len,len); 

    for(i=0;i<len;i++)
    {
        for(j=0;j<len;j++)
        {
            Mat_d_nm.mat[i*len + j] = d_nm(i , j , input_data) ;
        }
    }

    return Mat_d_nm ;
}


Mat2d matrix_E_n(InputData input_data)
{
    size_t len = input_data.params.num_traps;

    Mat2d Mat_E_n = mat2DInitZerosA(len,len);

    Vec fn = input_data.probs ;
    Vec d1 = input_data.locs ;
    OxParams params_1 = input_data.params ;

    Vec E = getGridE(fn , d1 , params_1);

    size_t i,j ;
    for(i=0;i<len ; i++){
        for(j=0; j<len ; j++){
            Mat_E_n.mat[i*len + j] = E.x[i] - E.x[j] ;
        }
    }
    return Mat_E_n ;
}

double r_nm(InputData input_data , Mat2d mat_E , Mat2d mat_d , size_t n , size_t m)
{
    size_t len = input_data.params.num_traps ;

    double nu = input_data.params.nu_0 ;
    double gamma = input_data.params.gamma_0 ;
    double kb_T = 1.38*1e-23*input_data.params.temp ;

    double E_nm = mat_E.mat[len*n + m] ;
    double d_nm = mat_d.mat[len*n + m] ;

    return (nu*(exp((-d_nm/gamma) - (E_nm/kb_T))));
}

Mat2d matrix_r_nm(InputData input_data , Mat2d mat_E , Mat2d mat_d)
{
    /*Generate the d_nm matrix and Enm matrix after accepting the inputs and
    then pass them to functions instead of using the functions repeatedly in each function*/

    size_t len = input_data.params.num_traps ;
    size_t i , j ; 
    Mat2d mat_r = mat2DInitZerosA(len, len);
    for(i=0;i<len;i++){
        for(j=0;j<len;j++){
            mat_r.mat[len*i + j] = r_nm(input_data, mat_E , mat_d , i,j);
        }
    }
    return mat_r ;
}

Vec transmission_param(double E , double V)
{
    double inf_E = 1e6 ;
    
}

Mat2d R_en()
{
    
}
