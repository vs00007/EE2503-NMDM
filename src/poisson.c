#include <include/linalg.h>
#include <include/poisson.h>

double analyticalPoissonSol(const Vec f_n, const Vec d, double x)
{

    double sum = 0.0;
    for (size_t i = 0; i < f_n.len; i++)
    {
        double r_i = fabs(d.x[i] - x);

        if (r_i < TOL)
        {
            printf("\nSkipping charge at %.15lf.\n", r_i);
            continue;
        }

        sum += f_n.x[i] / r_i;
    }

    return K * Q * sum;
}

// Helper function to validate input
int validateInput(const Vec f_n, const Vec d, double x)
{
    if (f_n.len != d.len || f_n.len == 0) {
        return 0;  
    }
    if (isnan(x)) {
        return 0;  
    }
    for (size_t i = 0; i < f_n.len; i++) {
        if (isnan(f_n.x[i]) || isnan(d.x[i])) {
            return 0; 
        }
    }
    return 1;
}

// Wrapper function for error management
double poissonSolve(const Vec f_n, const Vec d, double x)
{
    if (!validateInput(f_n, d, x)) {
        errno = EINVAL;
        return NAN;
    }

    return analyticalPoissonSol(f_n, d, x);
}

