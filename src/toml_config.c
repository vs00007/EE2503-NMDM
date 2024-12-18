#include "include/inputs.h"

int parse_toml_file(const char* filename, OxParams* params, Vec* locations, Vec* trap_energies) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Cannot open file\n");
        return -1;
    }

    char errbuf[200];
    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);

    if (!conf) {
        fprintf(stderr, "Cannot parse file: %s\n", errbuf);
        return -1;
    }

    toml_table_t* oxide = toml_table_in(conf, "Oxide");
    if (!oxide) {
        fprintf(stderr, "Missing [Oxide] section\n");
        toml_free(conf);
        return -1;
    }

    const char* raw_length = toml_raw_in(oxide, "length");
    const char* raw_rel_perm = toml_raw_in(oxide, "relative_permitivity");
    const char* raw_v_top = toml_raw_in(oxide, "v_top");
    const char* raw_v_bottom = toml_raw_in(oxide, "v_bottom");
    const char* raw_temp = toml_raw_in(oxide, "temperature");
    const char* raw_affinity = toml_raw_in(oxide, "electron_affinity");

    if (!raw_length || !raw_rel_perm || !raw_v_top || !raw_v_bottom || !raw_temp || !raw_affinity) {
        fprintf(stderr, "Missing required oxide parameters\n");
        toml_free(conf);
        return -1;
    }

    char* endptr;
    
    params->L = strtod(raw_length, &endptr);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid value for length\n");
        toml_free(conf);
        return -1;
    }
    params->eps_r = strtod(raw_rel_perm, &endptr);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid value for relative_permitivity\n");
        toml_free(conf);
        return -1;
    }
    params->V_L = strtod(raw_v_bottom, &endptr);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid value for v_bottom\n");
        toml_free(conf);
        return -1;
    }
    params->V_0 = strtod(raw_v_top, &endptr);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid value for v_top\n");
        toml_free(conf);
        return -1;
    }
    params->temp = strtod(raw_temp, &endptr);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid value for temperature\n");
        toml_free(conf);
        return -1;
    }
    params->electron_affinity = strtod(raw_affinity, &endptr);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid value for electron_affinity\n");
        toml_free(conf);
        return -1;
    }

    toml_table_t* traps = toml_table_in(oxide, "traps");
    if (!traps) {
        fprintf(stderr, "Missing [Oxide.traps] section\n");
        toml_free(conf);
        return -1;
    }

    toml_array_t* loc_array = toml_array_in(traps, "locations");
       toml_array_t* energy_array = toml_array_in(traps, "trap_energy");

    if (!loc_array || !energy_array) {
        fprintf(stderr, "Missing Trap array or Energy array\n");
        toml_free(conf);
        return -1;
    }

    size_t array_size = toml_array_nelem(loc_array);

    if (array_size != (size_t)toml_array_nelem(energy_array)) {
        fprintf(stderr, "Location and Energy arrays must have same size\n");
        toml_free(conf);
        return -1;
    }

    *locations = vecInitOnesA(array_size);
    *trap_energies = vecInitZerosA(array_size);

    for (size_t i = 0; i < array_size; i++) {
        const char* raw_loc = toml_raw_at(loc_array, i);
        *vecRef(*locations, i) = strtod(raw_loc, &endptr);
        if (*endptr != '\0') {
            fprintf(stderr, "Invalid value in locations or energies array\n");
            free(locations->x);
            free(trap_energies->x);
            toml_free(conf);
            return -1;
        }
    }
    for (size_t i = 0; i < array_size; i++) {
        const char* raw_e = toml_raw_at(energy_array, i);
        *vecRef(*trap_energies, i) = strtod(raw_e, &endptr);
        if (*endptr != '\0') {
            fprintf(stderr, "Invalid value in locations or energies array\n");
            free(locations->x);
            free(trap_energies->x);
            toml_free(conf);
            return -1;
        }
    }
            
    vecScale(params->L, *locations, locations);
    vecScale(Q, *trap_energies, trap_energies);

    toml_table_t* transport = toml_table_in(oxide, "transport");
    if (!transport) {
        fprintf(stderr, "Missing [Oxide.transport] section\n");
        toml_free(conf);
        return -1;
    }
    const char* raw_nu_0 = toml_raw_in(transport, "nu_0");
    const char* raw_mobility = toml_raw_in(transport, "mobility");
    const char* raw_m_eff = toml_raw_in(transport, "m_eff");
    const char* raw_d_relx = toml_raw_in(transport, "relaxation_distance");

    params->nu_0 = strtod(raw_nu_0, &endptr);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid value for nu_0\n");
        toml_free(conf);
        return -1;
    }

    params->m_eff = Me * strtod(raw_m_eff, &endptr);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid value for effective Mass: m_eff\n");
        toml_free(conf);
        return -1;
    }

    params->mobility = strtod(raw_mobility, &endptr);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid value for mobility\n");
        toml_free(conf);
        return -1;
    }

    params->gamma_0 = strtod(raw_d_relx, &endptr);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid value for relaxation distance\n");
        toml_free(conf);
        return -1;
    }

    params->num_traps = locations->len;

    toml_table_t* simParams = toml_table_in(oxide, "SimParams");
   
    const char * raw_chunk_size = toml_raw_in(simParams, "chunk_size");
   
    params->chunk_size = strtoul(raw_chunk_size, &endptr, 10);

    if (*endptr != '\0') {
        fprintf(stderr, "Invalid value for chunk size\n");
        toml_free(conf);
        return -1;
    }  

    toml_free(conf);
    return 0;
}

void printOxParams(const OxParams* params) 
{
    printf("\n=== Oxide Parameters ===\n");
    printf("Device Properties:\n");
    printf("  Length: %.3Le m\n", params->L);
    printf("  Relative Permittivity: %Lf\n", params->eps_r);
    printf("  Electron Affinity: %Lf eV\n", params->electron_affinity);
    
    printf("\nVoltage Conditions:\n");
    printf("  Bottom Voltage (V₀): %Lf V\n", params->V_0);
    printf("  Top Voltage (Vₗ): %Lf V\n", params->V_L);
    
    printf("\nTransport Parameters:\n");
    printf("  Attempt Frequency (ν₀): %.3Le Hz\n", params->nu_0);
    printf("  Effective Mass (m*): %.3Le kg\n", params->m_eff);
    printf("  Mobility (μ): %.3Le m²/V·s\n", params->mobility);
    printf("  Relaxation Distance (γ₀): %.3Le m\n", params->gamma_0);
    
    printf("\nSimulation Parameters:\n");
    printf("  Temperature: %Lf K\n", params->temp);
    printf("  Number of Traps: %zu\n", params->num_traps);
    printf("  Chunk Size: %zu\n", params->chunk_size);
    printf("\n=====================================\n");
}

void printInputData(const InputData* data) 
{
    printf("\n========= Input Data Paramters =========\n");
    
    printOxParams(&data->params);
    
    printf("\n========================== Trap Data =============================\n");
    printf("Index    Location (m)    Occupation Prob.    Trap Energies (Ed)\n");
    printf("------------------------------------------------------------------\n");
    for(size_t i = 0; i < data->locs.len; i++) {
        printf("%-8zu %-14.3Le %-14.3Lf %9Lg\n", 
                i, 
                vecGet(data->locs, i), 
                vecGet(data->probs, i),
                vecGet(data->energies, i) / Q
    );
    }
    printf("\n==================================================================\n");
}

InputData getInput(char *filename) {
    OxParams params;
    Vec locations;
    Vec trap_energies;

    if (parse_toml_file(filename, &params, &locations, &trap_energies) != 0) {
        fprintf(stderr, "Failed to parse TOML file\n");
        return (InputData){0};
    }
    InputData data;
    data.params = params;
    data.locs = locations;
    data.probs = vecInitZerosA(params.num_traps);
    data.energies = trap_energies;
    return data;
}


