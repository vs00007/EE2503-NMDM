#include <include/inputs.h>

void printVecExp(Vec f_n)
{
    // Blatantly copied printVec functiion, adapted to print the values with units
    printf("[");
    for(size_t i = 0; i < f_n.len - 1; i++)
    {
        printf("%e, ", vecGet(f_n, i));
    }
    printf("%e]", vecGet(f_n, f_n.len - 1));
}

void test_toml()
{
    char *filepath = "data/input-params.toml";
    
    InputData data;

    data = getInput(filepath);

    printf("Locations : ");
    printVecExp(data.locs);
    printf("\n");

    printf("Occupation probabilities : ");
    vecPrint(data.probs);
    printf("\n");

    printf("Relative Permitivity : %lf\n", data.params.eps_r);
    printf("Potential at L = 0 : %lf\n", data.params.V_0);
    printf("Potential at L = L : %lf\n", data.params.V_L);
    printf("Length of the oxide : %e\n", data.params.L);
}