#include <include/steadystate.h>
#define MIN_ERROR 1e-15

//returns elementwise product as a vector 
void vecMultiply(Vec a, Vec b, Vec* result){
    if(a.len != b.len){
        if(a.len > b.len){
            printf("Multiplication not possible as 1st vector has greater size than 2nd. Size of 1st %zu, 2nd %zu", a.len, b.len);
        }
        if(a.len < b.len){
            printf("Multiplication not possible as 2nd vector has greater size than 1st.Size of 1st %zu, 2nd %zu", a.len, b.len);
        }
    }
    else{
        for(size_t i = 0; i < a.len; i++){
            VEC_INDEX(*result, i) = VEC_INDEX(a, i)*VEC_INDEX(b, i);
        }
    }
}

//initializes the i+1, j+1 elements of jacobian matrix
void jacobian_matrix(Mat2d matrix /*Initialize this matrix to 0*/, Mat2d coeffmatrix/*state to state transmission coefficients*/ , Vec R1 /*electrode 1 coefficients matrix*/, Vec R2 /*electrode 2 coefficients matrix*/,Vec f , int i, int j){
    Vec one = vecInitOnesA(f.len);//remove this dumb
    Vec fbar = vecInitZerosA(f.len);//remove this dumb
    vecSub(one, f, &fbar);
    if(i == j){
        double abbar = vecDot(mat2DRow(coeffmatrix, i), fbar);
        double ab = vecDot(mat2DCol(coeffmatrix, i), f);
        *mat2DRef(matrix, i, i) = -(VEC_INDEX(R1, i) + VEC_INDEX(R2, i) + abbar + ab);
    }
    else{
        *mat2DRef(matrix, i, j) = *mat2DRef(coeffmatrix, i, j)*VEC_INDEX(f, i) + *mat2DRef(coeffmatrix, j, i)*VEC_INDEX(fbar, i);
    }
    freeVec(&one), freeVec(&fbar);//removed them dumb
}

//swaps a, b
void swap(double* a, double* b){
    double temp = *a;
    *a = *b;
    *b = temp;
}

//swaps row1 with row2 for gaussian elimination
void swap_rows(Mat2d A, Vec b, int row1, int row2){
    //matrix element swap
    for(size_t l = 0; l < A.rows; l++){
        swap(mat2DRef(A, row1, l), mat2DRef(A, row2, l));
    }
    //vector element swap
    swap(vecRef(b, row1), vecRef(b, row2));
}

int find_pivot_row(Mat2d A, int start_row, int column){
    int max_row = start_row;
    double max_val = fabs(*mat2DRef(A, start_row, column));
    for(size_t i = start_row + 1; i < A.rows; i++){
        if(fabs(*mat2DRef(A, i, column)) > max_val){
            max_val = fabs(*mat2DRef(A, i, column));
            max_row = i;
        }
    }
    return max_row;
}

//does gaussian elimination
void gaussian_elimination(Mat2d A/*jacobian matrix*/, Vec b/*coefficient vector*/){
    int pivot_row = 0;
    double pivot, factor;

    for(size_t i = 0; i < A.rows; i++){
        pivot_row = find_pivot_row(A, i, i);
        if(pivot_row != (int)i){
            swap_rows(A, b, i, pivot_row);
        }

        pivot = *mat2DRef(A, i, i);
        if(fabs(pivot) < MIN_ERROR){
            printf("The matrix is not good dumb fuck idiot. Be a OHIO SIGMA RIZZLER! don't be a NO GYAT BETA!");
            return;
        }
        VEC_INDEX(b, i)/=pivot;
        for(size_t j = i; j < A.rows; j++){
            *mat2DRef(A, i, j)/=pivot;
        }

        for(size_t k = i+1; k < A.cols; k++){
            factor = *mat2DRef(A, k, i);
            VEC_INDEX(b, k) -= factor*VEC_INDEX(b, i);
            for(size_t m = i; m < A.rows; m++){
                *mat2DRef(A, k, m) -= factor*(*mat2DRef(A, i, m));
            }
        }
    }
}

//does backsubstitutin for gaussian elimination
void back_substitutionA(Mat2d A, Vec b, Vec* x){
    for(int i = A.cols - 1; i >= 0; i--){
        VEC_INDEX(*x, i) = VEC_INDEX(b, i);
        for(size_t j = i+1; j < A.rows; j++){
            VEC_INDEX(*x, i) -= *mat2DRef(A, i, j) * VEC_INDEX(*x, j);
        }
    }
}

Vec masterequationcoffA(Vec f, Vec R1, Vec R2, Mat2d coeffmatrix){
    Vec one = vecInitOnesA(f.len);//remove this dumb
    Vec fbar = vecInitZerosA(f.len);//remove this dumb
    Vec F = vecInitZerosA(f.len);
    vecSub(one, f, &fbar);
    Vec f1 = vecInitZerosA(f.len), f2 = vecInitZerosA(f.len), f3 = vecInitZerosA(f.len), f4 = vecInitZerosA(f.len), f5 = vecInitZerosA(f.len), f6 = vecInitZerosA(f.len), f7 = vecInitZerosA(f.len), f8 = vecInitZerosA(f.len);//don't forget to free this dumb
    vecMultiply(R1, fbar, &f1);
    vecMultiply(R2, f, &f2);
    vecSub(f1, f2, &f8);
    Mat2d coeffmatrixTranspose = mat2DInitZerosA(coeffmatrix.cols, coeffmatrix.rows);
    mat2DTranspose(coeffmatrix, &coeffmatrixTranspose);
    mat2DTransform(coeffmatrix, fbar, &f3);
    mat2DTransform(coeffmatrixTranspose, f, &f4);
    vecMultiply(f3, f, &f5);
    vecMultiply(f4, fbar, &f6);
    vecSub(f6, f5, &f7);
    vecAdd(f7, f8, &F);
    freeVec(&f1), freeVec(&f2), freeVec(&f3), freeVec(&f4), freeVec(&f5), freeVec(&f6), freeVec(&f7), freeVec(&f8);
    freeMat2D(&coeffmatrixTranspose);
    return F;
}

//implements jacobian algorithm
Vec jacobian_implementationA(Mat2d coeffmatrix, Vec R1, Vec R2){
    srand(time(NULL));
    Mat2d matrix = mat2DInitZerosA(coeffmatrix.rows, coeffmatrix.cols);//don't forget to remove it dumb
    Vec f = vecInitA(0.0000005, matrix.cols);//need it dumb
    Vec F = vecInitA(0, matrix.cols);//need it dumb but can free it later
    Vec delta_f = vecInitA(0.0000001, matrix.cols);//free it dumb
    while(fabs(vecMax(delta_f)) > MIN_ERROR){
        for(size_t i = 0; i < matrix.rows; i++){
            for(size_t j = 0; j < matrix.cols; j++){
                jacobian_matrix(matrix, coeffmatrix, R1, R2, f, i, j);
            }
        }
        F = masterequationcoffA(f, R1, R2, coeffmatrix);
        gaussian_elimination(matrix, F);
        back_substitutionA(matrix, F, &delta_f);
        vecSub(f, delta_f, &f);
    }
    freeVec(&F), freeVec(&delta_f);
    freeMat2D(&matrix);
    return f;
}


