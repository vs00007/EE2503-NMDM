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
            printf("\nSkipping charge at %e.\n", r_i);
            continue;
        }

        sum += f_n.x[i] / r_i;
    }

    return K * Q * sum;
}

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

double poissonSolveAnalytical(const Vec f_n, const Vec d, double x)
{
    if (!validateInput(f_n, d, x)) {
        errno = EINVAL;
        return NAN;
    }

    return analyticalPoissonSol(f_n, d, x);
}

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
        result.x[i] = poissonSolveAnalytical(f_n, d, d.x[i]);
    }

    printf("Resultant Voltages: ");
    printVecUnits(result, 'V');

    return result;
}

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
        result.x[i] = poissonSolveAnalytical(f_n, d, d.x[i]);
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

void stackToVec(DynStack* mesh, Vec *mesh_vec)
{
    *mesh_vec = *(Vec *)dynStackGet(*mesh, 0);
    // mesh_vec->x = mesh->data;
}

int validateVec(const Vec d, const OxParams params)
{
    if (d.len == 0 || params.L <= 0) {
        return 0;  
    }
    if (d.x[0] < 0 || d.x[d.len - 1] > params.L)
        return 0;
    for (size_t i = 0; i < d.len; i++) {
        if (isnan(d.x[i])) {
            return 0; 
        }
    }
    return 1;
}

Vec generateMesh(Vec d, OxParams oxparams)
{
    // Check Input 
    if (!validateVec(d, oxparams))
    {
        printf("Input Invalid.\n");
        return (Vec){NULL, 0, 0};
    }

    DynStack mesh = dynStackInit(sizeof(double));
    size_t chunk_size = 10;
    double mesh_point = 0;
    
    // Piecewise mesh creation. Adds all points to the mesh

    // from 0 to d[n]
    double d_0 = d.x[0];
    for(size_t i = 0; i < chunk_size; i++)
    {
        mesh_point = (double)(i * d_0) / chunk_size; 
        dynStackPush(&mesh, &mesh_point);
    }

    // Everywhere else
    for (size_t i = 0; i < d.len - 1; i++)
    {
        double d_i = fabs(d.x[i] - d.x[i + 1]);
        for (size_t j = 0; j < chunk_size; j++)
        {
            mesh_point = d.x[i] + (double)(j * d_i) / chunk_size;
            dynStackPush(&mesh, &mesh_point);
        }
    }

    // from d[n] to L
    double d_n = fabs(d.x[d.len - 1] - oxparams.L);
    for(size_t i = 0; i < chunk_size; i++)
    {
        mesh_point = d.x[d.len - 1] + (double)(i * d_n) / chunk_size; 
        dynStackPush(&mesh, &mesh_point);
    }

    // Last point
    mesh_point = oxparams.L;
    dynStackPush(&mesh, &mesh_point);

    // for (size_t i = 0; i < mesh.len; i ++)
    // {
    //     printf("%g\n", *(double *)dynStackGet(mesh, i));
    // }

    Vec mesh_vec = vecInitZerosA(mesh.len);
    stackToVec(&mesh, &mesh_vec);

    // freeDynStack(&mesh);

    return mesh_vec;
}

Vec generateStepSize(Vec mesh_vec)
{
    Vec h = vecInitZerosA(mesh_vec.len - 1);
    vecPrint(mesh_vec);
    for(size_t i = 1; i < mesh_vec.len; i ++)
    {
        *vecRef(h, i - 1) = vecGet(mesh_vec, i) - vecGet(mesh_vec, i - 1);
    }

    return h;
}

Mat2d generateJacobian(Vec mesh, OxParams params)
{
    Mat2d jcob = mat2DInitZerosA(mesh.len, mesh.len);
    
    Vec h = generateStepSize(mesh);
  
    for (size_t i = 1; i < mesh.len - 1; i ++)
    {
        double avg_step = 0;
        avg_step = 0.5 * (vecGet(h, i) + vecGet(h, i - 1));

        double term_i_minus_1 = 1 / (vecGet(h, i - 1) * avg_step);
        double term_i_plus_1 = 1 / (vecGet(h, i) * avg_step);
        double term_i = 1 / (vecGet(h, i) * vecGet(h, i - 1));
        printf("%lf, %lf, %lf\n", term_i, term_i_minus_1, term_i_plus_1);
        *mat2DRef(jcob, i, i - 1) = term_i_minus_1;
        *mat2DRef(jcob, i, i) = term_i;
        *mat2DRef(jcob, i, i + 1) = term_i_plus_1;
    }
    mat2DPrint(jcob);
}