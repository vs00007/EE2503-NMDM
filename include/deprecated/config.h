#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

// Maximum error message length
#define MAX_ERROR_LEN 256
// Maximum line length in config file
#define MAX_LINE_LEN 1024
// Maximum number of traps
#define MAX_TRAPS 1000

typedef struct {
    double thickness;
    double relPermittivity;
    double vBottom;
    double vTop;
} OxideParams;

typedef struct {
    OxideParams oxideParams;
    int numTraps;
    double* trapPositions;
    double* occupationProbs;
} TrapConfig;

bool Validate(const TrapConfig* config, char* error_msg);
bool Read(const char* filename, TrapConfig* config, char* error_msg);
bool Write(const char* filename, const TrapConfig* config, char* error_msg);

#endif // CONFIG_H