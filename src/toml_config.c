#include "include/inputs.h"

int parse_toml_file(const char* filename, OxParams* params, Vec* locations, Vec* occ_probs) {
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

    if (!raw_length || !raw_rel_perm || !raw_v_top || !raw_v_bottom) {
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
    params->V_0 = strtod(raw_v_bottom, &endptr);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid value for v_bottom\n");
        toml_free(conf);
        return -1;
    }
    params->V_L = strtod(raw_v_top, &endptr);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid value for v_top\n");
        toml_free(conf);
        return -1;
    }

    toml_table_t* traps = toml_table_in(oxide, "traps");
    if (!traps) {
        fprintf(stderr, "Missing [Oxide.traps] section\n");
        toml_free(conf);
        return -1;
    }

    // Parse trap arrays
    toml_array_t* loc_array = toml_array_in(traps, "locations");
    toml_array_t* prob_array = toml_array_in(traps, "occ_probs");

    if (!loc_array || !prob_array) {
        fprintf(stderr, "Missing trap arrays\n");
        toml_free(conf);
        return -1;
    }

    // Get array sizes
    size_t array_size = toml_array_nelem(loc_array);
    if (array_size != (size_t)toml_array_nelem(prob_array)) {
        fprintf(stderr, "Location and probability arrays must have same size\n");
        toml_free(conf);
        return -1;
    }

    // Initialize Vecs
    *locations = vecInitOnesA(array_size);
    *occ_probs = vecInitOnesA(array_size);

    // Parse array elements
    for (size_t i = 0; i < array_size; i++) {
        const char* raw_loc = toml_raw_at(loc_array, i);
        const char* raw_prob = toml_raw_at(prob_array, i);
        
        if (!raw_loc || !raw_prob) {
            fprintf(stderr, "Error parsing array elements\n");
            free(locations->x);
            free(occ_probs->x);
            toml_free(conf);
            return -1;
        }

        locations->x[i] = strtod(raw_loc, &endptr);
        if (*endptr != '\0') {
            fprintf(stderr, "Invalid value in locations array\n");
            free(locations->x);
            free(occ_probs->x);
            toml_free(conf);
            return -1;
        }
        occ_probs->x[i] = strtod(raw_prob, &endptr);
        if (*endptr != '\0') {
            fprintf(stderr, "Invalid value in occ_probs array\n");
            free(locations->x);
            free(occ_probs->x);
            toml_free(conf);
            return -1;
        }
    }
    vecScale(params->L, *locations, locations);

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

    params->m_eff = strtod(raw_m_eff, &endptr);
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

    toml_free(conf);
    return 0;
}

InputData getInput(char *filename) {
    OxParams params;
    Vec locations;
    Vec occ_probs;

    if (parse_toml_file(filename, &params, &locations, &occ_probs) != 0) {
        fprintf(stderr, "Failed to parse TOML file\n");
        return (InputData){0};
    }

    InputData data;
    data.params = params;
    data.locs = locations;
    data.probs = occ_probs;

    return data;
}
