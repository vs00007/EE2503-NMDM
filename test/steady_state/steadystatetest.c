#include <test/steady_state/steadystatetest.h>


int testSteadystate(){
    Mat2d coeffmatrix = mat2DInitA(0, 2, 2);
    coeffmatrix.mat[0] = 0.1;
    coeffmatrix.mat[1] = 0.2;
    coeffmatrix.mat[2] = 0.3;
    coeffmatrix.mat[3] = 0.1;
    double value1 = 0.6;
    double value2 = 0.5;

    Vec R1 = vecInitA(value1, coeffmatrix.cols);
    Vec R2 = vecInitA(value2, coeffmatrix.cols);
    Vec f = jacobianImplementationA(coeffmatrix, R1, R2);    
    vecPrint(f);
    Vec F = masterEquationCoeffA(f, R1, R2, coeffmatrix);
    vecPrint(F);
    return 0;
}