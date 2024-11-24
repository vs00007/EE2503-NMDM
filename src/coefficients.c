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
    return fabs(input_data.locs.x[n] - input_data.locs.x[m]) ;
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
            *mat2DRef(Mat_d_nm, i, j) = d_nm(i , j , input_data) ;
        }
    }

    return Mat_d_nm ;
}


Mat2d matrix_E_n(InputData input_data, Vec mesh)
{
    size_t len = input_data.params.num_traps;

    Mat2d Mat_E_n = mat2DInitZerosA(len,len);

    Vec E = poissonWrapper(input_data, mesh);
    vecScale(Q, E, &E);

    size_t i,j ;
    for(i = 0; i < len ; i++){
        for(j = 0; j < len ; j++){
            *mat2DRef(Mat_E_n, i, j) = E.x[i] - E.x[j] ;
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

    for(i = 0; i < len; i++){
        for(j = 0; j < len; j++){
            *mat2DRef(mat_r, i, j) = r_nm(input_data, mat_E , mat_d , i,j);
        }
    }
    return mat_r ;
}

double transmission_param(double T_b , InputData input_data , double V_electrode)
{
    double h_bar = 1.054571817e-34 ;
    double q = -1.6e-19 ;
    double m = 9.1e-32;
    double E_A = input_data.params.electron_affinity ;

    Vec mesh = generateMesh( input_data.locs ,input_data.params);
    Vec V_x = poissonWrapper(input_data, mesh) ;
    
    size_t i = 0;
    double T_exp= 0 ;
    size_t j = 0 ; 
    Vec d_now = input_data.locs ;
    size_t band = input_data.params.chunk_size ;
    double delta = (d_now.x[1] - d_now.x[0])/band ;

    while(mesh.x[i] <= T_b){
        double E = -q*V_x.x[i] + E_A ;
        if(mesh.x[i] > d_now.x[ j + 1 ] && j< (input_data.params.num_traps - 1)){
            j++ ;
            delta = (d_now.x[ j + 1 ] - d_now.x[j])/band ;
        }     
        // E = -qVa everywhere
        T_exp += sqrt(m*(E + q*V_electrode))*delta ;
        i++ ;
    }
    return exp((-2 * T_exp) / h_bar) ;
}

Mat2d R_en(InputData input_data, Vec mesh)
{
    double kb_T = 1.38*1e-23*input_data.params.temp ;
    size_t len = input_data.params.num_traps ;
    Mat2d mat_R = mat2DInitZerosA(len, 2) ;
    Vec d = input_data.locs ;
    double k = 1 ;
    double q = -1.6e-19 ;
    
    size_t i = 0 ;
    double t = 0;

    Vec E = getGridNumE(input_data, mesh);

    //Top electrode
    double V_0 = input_data.params.V_0 ;

    for(i = 0 ; i < len ; i++){
        t = transmission_param(d.x[i] , input_data , V_0) ;
        *mat2DRef(mat_R, i, 0) = k * t * kb_T * log(1 + exp(E.x[i] + q*V_0 )) ;
    }

    // Bottom electrode
    double V_L = input_data.params.V_L ;

    for(i = 0 ; i < len ; i++){
        t = transmission_param(d.x[i] , input_data , V_L) ;
        *mat2DRef(mat_R, i, 1) = k * t * kb_T * log(1 + exp(E.x[i] + q * V_L )) ;
    }
    return mat_R ;
}
