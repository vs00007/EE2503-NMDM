#include <stdio.h>
#include <stdlib.h>
#include "include/deprecated/config.h"

// int testFile()
// {
//     TrapConfig config;
//     char error_msg[MAX_ERROR_LEN];

//     // Allocate memory for arrays
//     config.numTraps = 0;  
//     config.trapPositions = NULL;
//     config.occupationProbs = NULL;

//     // Read configuration from file
//     if (!Read("data/trap.txt", &config, error_msg))
//     {
//         printf("Error reading configuration: %s\n", error_msg);
//         // free(caonfig.trapPositions);
//         free(config.occupationProbs);
//         return 1;
//     }

//     // Print configuration details
//     printf("Oxide Parameters:\n");
//     printf("  Thickness: %.6e m\n", config.oxideParams.thickness);
//     printf("  Relative Permittivity: %.6f\n", config.oxideParams.relPermittivity);
//     printf("  Bottom Voltage: %.6f V\n", config.oxideParams.vBottom);
//     printf("  Top Voltage: %.6f V\n", config.oxideParams.vTop);

//     printf("\nTraps:\n");
//     for (int i = 0; i < config.numTraps; i++)
//     {
//         printf("  Trap %d: position = %.6e m, probability = %.6f\n",
//                i + 1, config.trapPositions[i], config.occupationProbs[i]);
//     }

//     // Create modified configuration
//     TrapConfig modifiedConfig = config;
//     modifiedConfig.oxideParams.vTop = 2.0;

//     // Validate modified configuration
//     if (!Validate(&modifiedConfig, error_msg))
//     {
//         printf("Error validating modified configuration: %s\n", error_msg);
//         free(config.trapPositions);
//         free(config.occupationProbs);
//         return 1;
//     }

//     // Write modified configuration to new file
//     if (!Write("trap_modified.txt", &modifiedConfig, error_msg))
//     {
//         printf("Error writing configuration: %s\n", error_msg);
//         free(config.trapPositions);
//         free(config.occupationProbs);
//         return 1;
//     }

//     printf("\nModified configuration saved to 'trap_modified.txt'\n");

//     // Clean up
//     free(config.trapPositions);
//     free(config.occupationProbs);

//     return 0;
// }