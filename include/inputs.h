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
    double V_0;
    double V_L;
    double eps_r;
    double L;
    double nu_0;
    double m_eff;
    double gamma_0;
    double mobility;
    double temp ;
    size_t num_traps;
    size_t chunk_size ;
    double electron_affinity ;
} OxParams;

typedef struct InputData 
{
    OxParams params;
    Vec locs;
    Vec probs;
} InputData;

int parse_toml_file(const char* filename, OxParams* params, Vec* locations, Vec* occ_probs);

InputData getInput(char *filename);

void printParams(OxParams p);

void printOxParams(const OxParams* params);

void printInputData(const InputData* data);