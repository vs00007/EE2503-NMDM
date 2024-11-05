#include <include/toml-parser/toml.h>
#include <include/poisson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct InputData 
{
    OxParams params;
    Vec locs;
    Vec probs;
} InputData;

int parse_toml_file(const char* filename, OxParams* params, Vec* locations, Vec* occ_probs);

InputData getInput(char *filename);