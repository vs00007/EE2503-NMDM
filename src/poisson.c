#include <include/linalg.h>
#include <include/poisson.h>

long double analyticalPoissonSol(const Vec f_n, const Vec d, long double x)
{
    long double sum = 0.0;
    for (size_t i = 0; i < f_n.len; i++)
    {
        long double r_i = fabsl(d.x[i] - x);

        if (r_i < 1e-15)
        {
            printf("\nSkipping charge at %Le.\n", r_i);
            continue;
        }

        sum += f_n.x[i] / r_i;
    }

    return K * Q * sum;
}

int validateInput(const Vec f_n, const Vec d, long double x)
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

long double poissonSolveAnalytical(const Vec f_n, const Vec d, long double x)
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
    mesh_vec->x = (long double *)mesh->data;
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
    size_t chunk_size = oxparams.chunk_size ; 
    if (!validateVec(d, oxparams))
    {
        printf("Input Invalid.\n");
        return (Vec){NULL, 0, 0};
    }
    long double eps_dist = 1e-12;
    DynStack mesh = dynStackInit(sizeof(long double));
    long double mesh_point = 0;
    
    // Piecewise mesh creation. Adds all points to the mesh

    long double d_0 = d.x[0];
    // from 0 to d[0]
    for(size_t i = 0; i < chunk_size; i++)
    {
        mesh_point = (long double)(i * d_0) / chunk_size; 
        // printf("%Lg\n", mesh_point);
        dynStackPush(&mesh, &mesh_point);
    }

    // Everywhere else
    for (size_t i = 0; i < d.len - 1; i++)
    {
        long double d_i = fabsl(d.x[i] - d.x[i + 1]);
        for (size_t j = 0; j < chunk_size; j++)
        {
            mesh_point = d.x[i] + (long double)(j * d_i) / chunk_size;
            // printf("%Lg\n", mesh_point);
            dynStackPush(&mesh, &mesh_point);
        }

    }

    // from d[n] to L
    long double d_n = fabsl(d.x[d.len - 1] - oxparams.L);

    // mesh_point = d_n - eps_dist;
    // dynStackPush(&mesh, &mesh_point);

    for(size_t i = 0; i < chunk_size; i++)
    {
        mesh_point = d.x[d.len - 1] + (long double)(i * d_n) / chunk_size; 
        // printf("%Lg\n", mesh_point);
        dynStackPush(&mesh, &mesh_point);
    }


    // Last point
    mesh_point = oxparams.L;
    dynStackPush(&mesh, &mesh_point);

    // for (size_t i = 0; i < mesh.len; i ++)
    // {
    //     printf("%Lg\n", *(long double *)dynStackGet(mesh, i));
    // }
    Vec mesh_vec = vecInitZerosA(mesh.len);
    stackToVec(&mesh, &mesh_vec);
    // freeDynStack(&mesh);

    return mesh_vec;
}

Vec generateStepSize(Vec mesh_vec)
{
    Vec h = vecInitZerosA(mesh_vec.len - 1);
    // vecPrint(mesh_vec);
    for(size_t i = 1; i < mesh_vec.len; i ++)
    {
        *vecRef(h, i - 1) = vecGet(mesh_vec, i) - vecGet(mesh_vec, i - 1);
    }

    return h;
}

MatTD generateJacobian(Vec mesh)
{
    MatTD jcob = matTDinitA(mesh.len);
    
    Vec h = generateStepSize(mesh);
  
    *vecRef(jcob.main, 0) = 1;
    *vecRef(jcob.main, mesh.len - 1) = 1;

    for (size_t i = 1; i < mesh.len - 1; i ++)
    {
        long double avg_step = 0;
        avg_step = 0.5 * (vecGet(h, i) + vecGet(h, i - 1));

        long double term_i_minus_1 = 1 / (vecGet(h, i - 1) * avg_step);
        long double term_i_plus_1 = 1 / (vecGet(h, i) * avg_step);
        long double term_i = -2 / (vecGet(h, i) * vecGet(h, i - 1));

        *vecRef(jcob.main, i) = term_i;
        *vecRef(jcob.sub, i) = term_i_minus_1;
        *vecRef(jcob.sup, i) = term_i_plus_1;
    }

    // vecPrint(jcob.sup);
    // printf("\n");
    // vecPrint(jcob.main);
    // printf("\n");
    // vecPrint(jcob.sub);
    // printf("\n");

    return jcob;
}

Vec constructB(Vec f_n, Vec d, Vec mesh, size_t chunk, OxParams params)
{
    Vec b = vecInitZerosA(mesh.len);
    size_t idx = 0;

    // vecPrint(mesh);
    // printNL();
    // vecPrint(d);
    // printNL();
    // vecPrint(f_n);
    // printNL();

    *vecRef(b, 0) = params.V_0;
    for (size_t i = 0; i < mesh.len; i ++)
    {
        idx = i / chunk - 1;
        if (idx > d.len - 1) continue;

        long double diff = (vecGet(mesh, i + 1) - vecGet(mesh, i - 1)) / 2;

        long double entry = vecGet(f_n, idx) * Q / ((params.eps_r * EPS0) * powl(diff, 3));

        if ((vecGet(d, idx) - vecGet(mesh, i)) == 0) *vecRef(b, i) = entry;
    }
    *vecRef(b, b.len - 1) = params.V_L; 
    // printNL();
    // printNL();
    // vecPrint(b);
    // printNL();
    return b;
}

Vec numSolveV(MatTD mat, Vec b)
{

    // printf("\nLen of superDiag : %zu\nLen of main diag : %zu\nLen of subDiag : %zu", mat.sup.len, mat.main.len, mat.sub.len);

    Vec mod_d = vecCopyA(b);
    Vec mod_sup = vecCopyA(mat.sup);
    Vec sol = vecInitZerosA(b.len);

    *vecRef(mod_sup, 0) = vecGet(mat.sup, 0) / vecGet(mat.main, 0);
    *vecRef(mod_d, 0) = vecGet(b, 0) / vecGet(mat.main, 0);

    for (size_t i = 1; i < b.len; i ++)
    {
        long double num_sup = vecGet(mat.sup, i);
        long double den = vecGet(mat.main, i) - vecGet(mat.sub, i) * vecGet(mod_sup, i - 1);
        *vecRef(mod_sup, i) = num_sup / den;

        long double num_sol = vecGet(b, i) - vecGet(mat.sub, i) * vecGet(mod_d, i - 1);
        *vecRef(mod_d, i) = num_sol / den;
    }

    *vecRef(sol, sol.len - 1) = vecGet(mod_d, b.len - 1);

    for (int j = sol.len - 2; j >=0; j --)
    {
        *vecRef(sol, j) = vecGet(mod_d, j) - vecGet(mod_sup, j) * vecGet(sol, j + 1);
    }

    freeVec(&mod_d);
    freeVec(&mod_sup);

    return sol;
}

Vec poissonWrapper(InputData data, Vec mesh)
{
    MatTD jcob = generateJacobian(mesh);

    Vec b = constructB(data.probs, data.locs, mesh, data.params.chunk_size, data.params);

    Vec sol = numSolveV(jcob, b);

    freeVec(&jcob.main);
    freeVec(&jcob.sub);
    freeVec(&jcob.sup);
    freeVec(&b);

    return sol;
}

Vec getGridNumV(InputData data, Vec mesh)
{
    Vec numSol = poissonWrapper(data, mesh);
    Vec gridV = vecInitA(0, data.locs.len);
    if (!gridV.x) printf("Allocation Failure!\n");
    size_t idx = 0;
    for (size_t i = 0; i < mesh.len; i ++)
    {
        idx = i / data.params.chunk_size - 1;
        if (idx > data.locs.len - 1) continue;
        if ((vecGet(data.locs, idx) - vecGet(mesh, i)) == 0) *vecRef(gridV, idx) = vecGet(numSol, i);
    }
    freeVec(&numSol);
    return gridV;
}

Vec getGridNumE(InputData data, Vec mesh)
{
    Vec gridV = getGridNumV(data, mesh);
    Vec Et = vecInitA(0, gridV.len);
    
    for(size_t i = 0; i < gridV.len; i++)
    {
        // Et = -qV - χ - Ed
        long double ET_val = -Q * (vecGet(gridV, i)) - ((data.params.electron_affinity) - vecGet(data.energies, i));
        *vecRef(Et, i) = ET_val;
    }
    // printVecUnits(EC, 'eV');

    return Et;
}
void printNL()
{
    printf("\n");
}
