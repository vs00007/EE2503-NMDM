#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>
#include <errno.h> 

#include <linalg.h>
#include <stack.h>
#include <include/inputs.h>

#define M_PI 3.14159265358979323846
#define Q 1.602176634e-19
#define K 1 / (4 * M_PI * 8.85418782e-12)
#define EPS0 8.85418782e-12
#define TOL 1e-10



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
int validateInput(const Vec f_n, const Vec d, double x);

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
int validateTwoVecs(const Vec f_n, const Vec d);

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
double analyticalPoissonSol(const Vec f_n, const Vec d, double x);

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
double poissonSolveAnalytical(const Vec f_n, const Vec d, double x);

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
Vec getGridV(Vec f_n, Vec d);

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
Vec getGridE(Vec f_n, Vec d, OxParams params);

/**
 * @brief Converts a dynamic stack to a vector
 *
 * @param dstack Pointer to dynamic stack
 * @param vec Pointer to vector to store result
 */
void stackToVec(DynStack* dstack, Vec* vec);

/**
 * @brief Validates vector for meshing
 *
 * @param d The vector
 * @param params Oxide parameters
 * @return 1 if vectors are valid for meshing, 0 otherwise
 * 
 * Checks:
 * - Vector has non-zero length
 * - No NaN values in vectors
 * - Length of the oxide is non zero
 * - Vector elements are within oxide bounds
 */
int validateVec(const Vec d, const OxParams params);

/**
 * @brief Generates mesh points for numerical solution
 *
 * @param d Vector of charge positions
 * @param oxparams Oxide parameters
 * @return Vector containing mesh points
 * 
 * Creates uniform mesh with fixed number of points between:
 * - Start (0) and first charge
 * - Each pair of consecutive charges
 * - Last charge and end of oxide (L)
 */
Vec generateMesh(Vec d, OxParams oxparams, size_t chunk_size);


/**
 * @brief Generates the step size vector `h`
 * 
 * @param mesh_vec The mesh
 * @param params Oxide parameters
 * @return Vector h containing step size x_i - x_{i-1}
 */
Vec generateStepSize(Vec mesh_vec);

Vec constructB(Vec f_n, Vec d, Vec mesh, size_t chunk, OxParams params);

MatTD generateJacobian(Vec mesh);

Vec numSolveV(MatTD mat, Vec b);

Vec poissonWrapper(InputData data, size_t chunk_size);

void printNL();
