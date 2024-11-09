#include <include/linalg.h>
#include <include/poisson.h>

/**
 * @brief Calculates electrostatic potential in vacuum due to point charges
 *
 * This function calculates the potential using vacuum permittivity (ε₀).
 * For actual material potential, divide result by relative permittivity (εᵣ).
 *
 * @param f_n Vector of charges (in units of elementary charge)
 * @param d Vector of positions (in meters)
 * @param x Position at which to calculate potential (in meters)
 * @return Potential in volts (assuming vacuum permittivity)
 */

double analyticalPoissonSol(const Vec f_n, const Vec d, double x)
{
    double sum = 0.0;
    for (size_t i = 0; i < f_n.len; i++)
    {
        double r_i = fabs(d.x[i] - x);

        if (r_i < TOL)
        {
            printf("\nSkipping charge at %e.\n", r_i);
            continue;
        }

        sum += f_n.x[i] / r_i;
    }

    return K * Q * sum;
}

/**
 * @brief Validates input vectors and position for Poisson solver
 *
 * @param f_n Vector of charges
 * @param d Vector of positions
 * @param x Position to evaluate
 * @return 1 if inputs are valid, 0 otherwise
 * 
 * Checks:
 * - Vectors have same non-zero length
 * - Position x is not NaN
 * - No NaN values in vectors
 */

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

/**
 * @brief Validates two vectors for compatibility
 *
 * @param f_n First vector
 * @param d Second vector
 * @return 1 if vectors are compatible, 0 otherwise
 * 
 * Checks:
 * - Vectors have same non-zero length
 * - No NaN values in vectors
 */

int validateTwoVecs(const Vec f_n, const Vec d)
{
    if (f_n.len != d.len || f_n.len == 0) {
        return 0;  
    }
    for (size_t i = 0; i < f_n.len; i++) {
        if (isnan(f_n.x[i]) || isnan(d.x[i])) {
            return 0; 
        }
    }
    return 1;
}

/**
 * @brief Safe wrapper for analyticalPoissonSol with input validation
 *
 * @param f_n Vector of charges
 * @param d Vector of positions
 * @param x Position to evaluate
 * @return Potential in volts, or NAN if inputs invalid
 * 
 * Sets errno to EINVAL if inputs are invalid
 */

double poissonSolve(const Vec f_n, const Vec d, double x)
{
    if (!validateInput(f_n, d, x)) {
        errno = EINVAL;
        return NAN;
    }

    return analyticalPoissonSol(f_n, d, x);
}

/**
 * @brief Calculates potentials at each charge location
 *
 * @param f_n Vector of charges
 * @param d Vector of positions
 * @return Vector of potentials at each charge location
 *        Returns NULL vector if inputs invalid
 * 
 * Notes:
 * - Uses vacuum permittivity (ε₀)
 */

Vec getGridV(Vec f_n, Vec d)
{
    if (!validateTwoVecs(f_n, d)) 
    {
        errno = EINVAL;
        printf("Invalid Input. Returning NULL vector\n");
        return (Vec){ NULL, 0, 0 };
    }
    
    size_t len = d.len;
    Vec result = vecInitZerosA(len);

    for (size_t i = 0; i < len; i++)
    {
        result.x[i] = poissonSolve(f_n, d, d.x[i]);
    }

    printf("Resultant Voltages: ");
    printVecUnits(result, 'V');

    return result;
}

/**
 * @brief Calculates energy levels at each charge location
 *
 * @param f_n Vector of charges
 * @param d Vector of positions
 * @param params Oxide parameters (thickness, permittivity, potentials)
 * @return Vector of energies at each charge location
 *        Returns NULL vector if inputs invalid
 * 
 * Calculation steps:
 * 1. Calculate vacuum potential
 * 2. Convert to material potential using εᵣ
 * 3. Add linear potential from applied bias
 * 
 * Notes:
 * - Energies include both electrostatic and applied potential
 */

Vec getGridE(Vec f_n, Vec d, OxParams params)
{
    // Validate input parameters
    if (!validateTwoVecs(f_n, d)) 
    {
        errno = EINVAL;
        printf("Invalid Inputs: f_n and d.Returning NULL vector\n");
        return (Vec){ NULL, 0, 0 };
    }
    
    // Validate oxide parameters
    if (params.eps_r <= 0 || params.L <= 0) 
    {
        errno = EINVAL;
        printf("Invalid Input: Oxide parameters. Returning NULL vector\n");
        return (Vec){ NULL, 0, 0 };
    }
    
    size_t len = d.len;
    Vec result = vecInitZerosA(len);

    for (size_t i = 0; i < len; i++)
    {
        // Get vacuum potential
        result.x[i] = poissonSolve(f_n, d, d.x[i]);
        // Convert to device potential
        result.x[i] /= params.eps_r;
        // Add Linear potential drop due to Applied Bias
        result.x[i] += params.V_0 + (params.V_L - params.V_0)* d.x[i] / params.L;
    }

    printf("Resultant Energies: ");
    printVecUnits(result, 'E');
    
    return result;
}

/*================================================================================
                            Numerical Poisson Solver
================================================================================*/

Vec generateMesh(Vec d, SimParams simp)
{
    size_t mesh_size = 0;
    for(size_t i = 0; i < d.len - 1; i ++)
    {
        if (fabs(d.x[i] - d.x[i + 1]) >= 2*simp.trap_tol)
        {
            size_t num_bulk = (size_t)ceil((fabs(d.x[i] - d.x[i + 1]) - 2*simp.trap_tol)/simp.body_step);
            size_t num_trap = 2*ceil(simp.trap_tol/simp.trap_step);
            mesh_size += num_bulk + num_trap;
        }
        else if (fabs(d.x[i] - d.x[i + 1]) < 2*simp.trap_tol)
        {
            mesh_size += (size_t)ceil(fabs(d.x[i] - d.x[i + 1]))/simp.trap_step;
        }
    }
    printf("Mesh size = %zu", mesh_size);
}