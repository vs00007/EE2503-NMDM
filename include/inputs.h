#pragma once

#include <include/toml-parser/toml.h>
#include <include/linalg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Q 1.602176634e-19
#define Me 9.1093837139e-31

typedef struct OxParams 
{
    long double V_0;
    long double V_L;
    long double eps_r;
    long double L;
    long double nu_0;
    long double m_eff;
    long double gamma_0;
    long double mobility;
    long double temp ;
    size_t num_traps;
    size_t chunk_size ;
    long double electron_affinity ;
} OxParams;

typedef struct InputData 
{
    OxParams params;
    Vec locs;
    Vec probs;
    Vec energies;
} InputData;

int parse_toml_file(const char* filename, OxParams* params, Vec* locations, Vec* occ_probs);

InputData getInput(char *filename);

void printOxParams(const OxParams* params);

void printInputData(const InputData* data);