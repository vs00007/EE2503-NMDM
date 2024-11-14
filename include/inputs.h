#pragma once

#include <include/toml-parser/toml.h>
#include <include/linalg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct OxParams 
{
    double V_0;
    double V_L;
    double eps_r;
    double L;
    double nu_0;
    double m_eff;
    double gamma_0;
    double mobility;
    size_t num_traps;
} OxParams;

typedef struct InputData 
{
    OxParams params;
    Vec locs;
    Vec probs;
} InputData;

int parse_toml_file(const char* filename, OxParams* params, Vec* locations, Vec* occ_probs);

InputData getInput(char *filename);