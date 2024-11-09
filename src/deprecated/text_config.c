#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "include/deprecated/config.h"

// Helper function to check if a number is valid (not NaN or infinite)
static bool ValidNumber(double num)
{
    return !isnan(num) && !isinf(num);
}

// Helper function to trim whitespace
static char *Trim(char *str)
{
    char *end;
    while (isspace((unsigned char)*str))
        str++;
    if (*str == 0)
        return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    end[1] = '\0';
    return str;
}

static bool ValidateOxide(const OxideParams *params, char *error_msg)
{
    if (!ValidNumber(params->thickness) || params->thickness <= 0)
    {
        snprintf(error_msg, MAX_ERROR_LEN, "Invalid oxide thickness");
        return false;
    }
    if (!ValidNumber(params->relPermittivity) || params->relPermittivity <= 0)
    {
        snprintf(error_msg, MAX_ERROR_LEN, "Invalid relative permittivity");
        return false;
    }
    if (!ValidNumber(params->vBottom) || !ValidNumber(params->vTop))
    {
        snprintf(error_msg, MAX_ERROR_LEN, "Invalid voltage values");
        return false;
    }
    return true;
}

bool Validate(const TrapConfig *config, char *error_msg)
{
    // Validate oxide parameters first
    if (!ValidateOxide(&config->oxideParams, error_msg))
    {
        return false;
    }

    // Validate number of traps
    if (config->numTraps <= 0 || config->numTraps > MAX_TRAPS)
    {
        snprintf(error_msg, MAX_ERROR_LEN, "Invalid number of traps");
        return false;
    }

    // Validate trap positions
    for (int i = 0; i < config->numTraps; i++)
    {
        if (!ValidNumber(config->trapPositions[i]))
        {
            snprintf(error_msg, MAX_ERROR_LEN, "Invalid trap position at index %d", i);
            return false;
        }
        if (config->trapPositions[i] < 0 ||
            config->trapPositions[i] > config->oxideParams.thickness)
        {
            snprintf(error_msg, MAX_ERROR_LEN,
                     "Trap position at index %d is outside oxide thickness", i);
            return false;
        }
    }

    // Validate occupation probabilities
    for (int i = 0; i < config->numTraps; i++)
    {
        if (!ValidNumber(config->occupationProbs[i]))
        {
            snprintf(error_msg, MAX_ERROR_LEN,
                     "Invalid occupation probability at index %d", i);
            return false;
        }
        if (config->occupationProbs[i] < 0 || config->occupationProbs[i] > 1)
        {
            snprintf(error_msg, MAX_ERROR_LEN,
                     "Occupation probability at index %d must be between 0 and 1", i);
            return false;
        }
    }

    return true;
}

// Helper function to parse array from string
static bool ParseArray(const char *str, double *arr, int expected_size, char *error_msg)
{
    size_t len = strlen(str) + 1;
    char *copy = malloc(len);
    if (!copy)
    {
        snprintf(error_msg, MAX_ERROR_LEN, "Memory allocation failed");
        return false;
    }
    memcpy(copy, str, len);

    char *ptr = copy;

    // Remove braces
    if (ptr[0] != '{' || ptr[strlen(ptr) - 1] != '}')
    {
        free(copy);
        snprintf(error_msg, MAX_ERROR_LEN, "Array must be enclosed in braces {}");
        return false;
    }
    ptr[strlen(ptr) - 1] = '\0';
    ptr++;

    // Parse values
    int count = 0;
    char *token = strtok(ptr, ",");
    while (token && count < expected_size)
    {
        token = Trim(token);
        char *endptr;
        arr[count] = strtod(token, &endptr);
        if (*endptr != '\0')
        {
            free(copy);
            snprintf(error_msg, MAX_ERROR_LEN, "Invalid number format in array");
            return false;
        }
        count++;
        token = strtok(NULL, ",");
    }

    free(copy);

    if (count != expected_size)
    {
        snprintf(error_msg, MAX_ERROR_LEN,
                 "Array size mismatch. Expected %d, got %d", expected_size, count);
        return false;
    }

    return true;
}

bool Read(const char *filename, TrapConfig *config, char *error_msg)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        snprintf(error_msg, MAX_ERROR_LEN, "Could not open file: %s", filename);
        return false;
    }

    char line[MAX_LINE_LEN];

    // Read oxide parameters
    if (!fgets(line, MAX_LINE_LEN, file))
    {
        fclose(file);
        snprintf(error_msg, MAX_ERROR_LEN, "File is empty");
        return false;
    }
    if (sscanf(line, "%lf %lf %lf %lf",
               &config->oxideParams.thickness,
               &config->oxideParams.relPermittivity,
               &config->oxideParams.vBottom,
               &config->oxideParams.vTop) != 4)
    {
        fclose(file);
        snprintf(error_msg, MAX_ERROR_LEN, "Invalid oxide parameters format");
        return false;
    }

    // Read number of traps
    if (!fgets(line, MAX_LINE_LEN, file))
    {
        fclose(file);
        snprintf(error_msg, MAX_ERROR_LEN, "Missing number of traps");
        return false;
    }
    if (sscanf(line, "%d", &config->numTraps) != 1 ||
        config->numTraps <= 0 ||
        config->numTraps > MAX_TRAPS)
    {
        fclose(file);
        snprintf(error_msg, MAX_ERROR_LEN, "Invalid number of traps");
        return false;
    }

    // Allocate memory for arrays
    config->trapPositions = malloc(config->numTraps * sizeof(double));
    config->occupationProbs = malloc(config->numTraps * sizeof(double));
    if (!config->trapPositions || !config->occupationProbs)
    {
        free(config->trapPositions);
        free(config->occupationProbs);
        fclose(file);
        snprintf(error_msg, MAX_ERROR_LEN, "Memory allocation failed");
        return false;
    }

    // Read trap positions
    if (!fgets(line, MAX_LINE_LEN, file))
    {
        free(config->trapPositions);
        free(config->occupationProbs);
        fclose(file);
        snprintf(error_msg, MAX_ERROR_LEN, "Missing trap positions");
        return false;
    }
    if (!ParseArray(Trim(line), config->trapPositions, config->numTraps, error_msg))
    {
        free(config->trapPositions);
        free(config->occupationProbs);
        fclose(file);
        return false;
    }

    // Read occupation probabilities
    if (!fgets(line, MAX_LINE_LEN, file))
    {
        free(config->trapPositions);
        free(config->occupationProbs);
        fclose(file);
        snprintf(error_msg, MAX_ERROR_LEN, "Missing occupation probabilities");
        return false;
    }
    if (!ParseArray(Trim(line), config->occupationProbs, config->numTraps, error_msg))
    {
        free(config->trapPositions);
        free(config->occupationProbs);
        fclose(file);
        return false;
    }

    fclose(file);

    // Validate the complete configuration
    if (!Validate(config, error_msg))
    {
        free(config->trapPositions);
        free(config->occupationProbs);
        return false;
    }

    return true;
}

bool Write(const char *filename, const TrapConfig *config, char *error_msg)
{
    // Validate configuration before writing
    if (!Validate(config, error_msg))
    {
        return false;
    }

    FILE *file = fopen(filename, "w");
    if (!file)
    {
        snprintf(error_msg, MAX_ERROR_LEN, "Could not open file for writing: %s", filename);
        return false;
    }

    // Write oxide parameters
    fprintf(file, "%.6e %.6e %.6e %.6e\n",
            config->oxideParams.thickness,
            config->oxideParams.relPermittivity,
            config->oxideParams.vBottom,
            config->oxideParams.vTop);

    // Write number of traps
    fprintf(file, "%d\n", config->numTraps);

    // Write trap positions
    fprintf(file, "{");
    for (int i = 0; i < config->numTraps; i++)
    {
        fprintf(file, "%.6e%s",
                config->trapPositions[i],
                i < config->numTraps - 1 ? ", " : "");
    }
    fprintf(file, "}\n");

    // Write occupation probabilities
    fprintf(file, "{");
    for (int i = 0; i < config->numTraps; i++)
    {
        fprintf(file, "%.6f%s",
                config->occupationProbs[i],
                i < config->numTraps - 1 ? ", " : "");
    }
    fprintf(file, "}\n");

    fclose(file);
    return true;
}