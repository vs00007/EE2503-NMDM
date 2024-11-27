#include <test/steady_state/steadystatetest.h>



int testSteadystate(){
    Mat2d coeffmatrix = mat2DInitA(0, 2, 2);
    coeffmatrix.mat[0] = 0.1;
    coeffmatrix.mat[1] = 0.2;
    coeffmatrix.mat[2] = 0.3;
    coeffmatrix.mat[3] = 0.1;
    long double value1 = 0.6;
    long double value2 = 0.5;

    Vec R1 = vecInitA(value1, coeffmatrix.cols);
    Vec R2 = vecInitA(value2, coeffmatrix.cols);
    Vec f = jacobianImplementationA(coeffmatrix, R1, R2);    
    vecPrint(f);
    Vec F = masterEquationCoeffA(f, R1, R2, coeffmatrix);
    vecPrint(F);
    return 0;
}


void test_gaussianElimination()
{
    Mat2d test = mat2DConstruct(
        (long double[]){
  -3355751630.7723111, 3354626279.0251185, 1125351.7471925912,
  3354626279.0251185, -6709252558.0502371, 3354626279.0251185,
  1125351.7471925912, 3354626279.0251185, -3355751630.7723111
}, 3, 3);
    
    Vec test_b = vecConstruct(
        (long double[]){
            3.5527165483724359e-15, 2.8695719349316226e-21, 3.5527165483724359e-15
        }, 3);

    mat2DPrint(test);
    printf("\n");
    vecPrint(test_b);
    printf("\n");

    gaussianElimination(test, test_b);

    vecPrint(test_b);
    printf("\n");
}

