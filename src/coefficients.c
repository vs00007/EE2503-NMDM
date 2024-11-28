#include<stdio.h>
#include<math.h>
#include<include/poisson.h>
#include<include/inputs.h>
#include<stdlib.h>
#include<string.h>
#include<include/linalg.h>
#include<stdint.h>


long double d_nm(size_t n , size_t m , InputData input_data) 
{
    return fabsl(input_data.locs.x[n] - input_data.locs.x[m]) ;
}

Mat2d matrix_d_nm(InputData input_data)
{
    size_t len = input_data.params.num_traps;
    Mat2d Mat_d_nm = mat2DInitZerosA(len,len); 

    for(size_t i=0;i<len;i++)
    {
        for(size_t j=0;j<len;j++)
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
    
    Vec E = getGridNumE(input_data, mesh);
    
    for(size_t i = 0; i < len    ; i++){
        // if (isnan(E.x[i])) printf("Found Bad E. i = %zu\n", i);
        for(size_t j = 0; j < len ; j++){
            *mat2DRef(Mat_E_n, i, j) = E.x[i] - E.x[j] ;
        }
    }

    return Mat_E_n ;
}

long double r_nm(InputData input_data , Mat2d mat_E , Mat2d mat_d , size_t n , size_t m)
{
    size_t len = input_data.params.num_traps ;

    long double nu = input_data.params.nu_0 ;
    long double gamma = input_data.params.gamma_0 ;
    long double kb_T = 1.38 * 1e-23 * input_data.params.temp ;

    long double E_nm = mat2DGet(mat_E, n, m);
    long double d_nm = mat2DGet(mat_d, n, m);

    if(E_nm > 0.0) return nu * exp(-d_nm/gamma);

    return nu * exp((-d_nm/gamma) + (E_nm/kb_T));
}

Mat2d matrix_r_nm(InputData input_data , Mat2d mat_E , Mat2d mat_d)
{
    /*Generate the d_nm matrix and Enm matrix after accepting the inputs and
    then pass them to functions instead of using the functions repeatedly in each function*/

    size_t len = input_data.params.num_traps ;

    Mat2d mat_r = mat2DInitZerosA(len, len);

    for(size_t i = 0; i < len; i++){
        for(size_t j = 0; j < len; j++){
            *mat2DRef(mat_r, i, j) = r_nm(input_data, mat_E , mat_d , i,j);
        }
    }
    return mat_r ;
}

long double transmission_param(long double T_b , InputData input_data , long double V_electrode)
{
    long double h_bar = 1.054571817 * 1e-34 ;

    long double E_A = input_data.params.electron_affinity ;

    Vec mesh = generateMesh( input_data.locs ,input_data.params);
    Vec V_x = poissonWrapper(input_data, mesh) ;
    
    size_t i = 0;
    long double T_exp= 0 ;
    size_t j = 0 ; 
    Vec d_now = input_data.locs ;
    size_t band = input_data.params.chunk_size ;
    long double delta = (d_now.x[1] - d_now.x[0])/band ;

    while(mesh.x[i] <= T_b){
        long double E = -Q*V_x.x[i] + E_A ;
        if(mesh.x[i] > d_now.x[ j + 1 ] && j< (input_data.params.num_traps - 1)){
            j++ ;
            delta = (d_now.x[ j + 1 ] - d_now.x[j])/band ;
        }     
        // E = -qVa everywhere
        T_exp += sqrt(Me*(E + Q * V_electrode)) * delta ;
        i++ ;
    }
    return exp((-2 * T_exp) / h_bar) ;
}

Mat2d R_en(InputData input_data, Vec mesh)
{
    long double kb_T = 1.38*1e-23*input_data.params.temp ;
    size_t len = input_data.params.num_traps ;
    Mat2d mat_R = mat2DInitZerosA(len, 2) ;
    Vec d = input_data.locs ;
    long double k = 1e13;
    long double phi_M = Q * 2.85L;
    
    Vec fn = input_data.probs ;
    Vec d1 = input_data.locs ;
    OxParams params_1 = input_data.params ;
    Vec E = getGridNumE(input_data, mesh);

    //Top electrode
    long double V_0 = input_data.params.V_0;
    // Bottom electrode
    long double V_L = input_data.params.V_L;

    for(size_t i = 0 ; i < len ; i++){
        long double t = 1.0; //transmission_param(d.x[i] , input_data , V_0) ;

        long double ln_expo1 = -(E.x[i] + Q * V_0 + phi_M) / kb_T;
        if (ln_expo1 < 1000) ln_expo1 = logl(1 + expl(ln_expo1));
        //if(kb_T*ln_expo1 > 1.0) ln_expo1 = 1/kb_T;
        
        long double ln_expo2 = -(E.x[i] + Q * V_L + phi_M) / kb_T;
        if (ln_expo2 < 1000) ln_expo2 = logl(1 + expl(ln_expo2));
        //if(kb_T*ln_expo2 > 1.0) ln_expo2 = 1/kb_T;
        
        long double e_top_t = k * t * kb_T * ln_expo1;
        long double e_bottom_t = k * t * kb_T * ln_expo2;
        *mat2DRef(mat_R, i, 0) = e_top_t + e_bottom_t;
    }

    for(size_t i = 0 ; i < len ; i++){
        long double t = 1.0; // transmission_param(d.x[i] , input_data , V_L) ;
        
        long double ln_expo1 = (E.x[i] + Q * V_0 + phi_M) / kb_T;
        if (ln_expo1 < 1000) ln_expo1 = logl(1 + expl(ln_expo1));
        //if(kb_T*ln_expo1 > 1.0) ln_expo1 = 1/kb_T;

        long double ln_expo2 = (E.x[i] + Q * V_L + phi_M) / kb_T;
        if (ln_expo2 < 1000) ln_expo2 = logl(1 + expl(ln_expo2));
        //if(kb_T*ln_expo2 > 1.0) ln_expo2 = 1/kb_T;
        
        long double e_top_t = k * t * kb_T * ln_expo1;
        long double e_bottom_t = k * t * kb_T * ln_expo2;
        *mat2DRef(mat_R, i, 1) = e_top_t + e_bottom_t;
    }
    return mat_R ;
}
