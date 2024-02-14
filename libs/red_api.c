// red_api.c

#include "red_api.h"
#include "systems_header.h"
#include <string.h>
#include "utils.h"

// Coefficients structure for the new-type RED function - will be used later
struct RED_coefficients
{
    double alpha,beta,gama;
    double A,B,C;
};

// Function to return the entire list of systems
const char** ListOfSupportedSystems(size_t* size) {
    int count = 0;

    char** supportedSystems = loadSupportedSystems(SUPPORTED_SYSTEMS_PATH, &count); // from utils.c

    *size = count; // Set the size for the caller
    return (const char **) supportedSystems; // Cast to const char** when returning
}

// System RED function selector
REDFunction getREDFunction(char *systemType) {
    if (strcmp(systemType, "RZ-104-11") == 0) return RED_RZ_104_11;
    else if (strcmp(systemType, "RZ-104-12") == 0) return RED_RZ_104_12;
    else if (strcmp(systemType, "RZ-163-11") == 0) return RED_RZ_163_1L;
    else if (strcmp(systemType, "RZ-163-12") == 0) return RED_RZ_163_ML;
    else if (strcmp(systemType, "RZ-163-13") == 0) return RED_RZ_163_ML;
    else if (strcmp(systemType, "RZ-163-14") == 0) return RED_RZ_163_ML;
    else if (strcmp(systemType, "RZ-163HP-11") == 0) return RED_RZ_163HP_1L;
    else if (strcmp(systemType, "RZ-163HP-12") == 0) return RED_RZ_163HP_ML;
    else if (strcmp(systemType, "RZ-163HP-13") == 0) return RED_RZ_163HP_ML;
    else if (strcmp(systemType, "RZ-163HP-14") == 0) return RED_RZ_163HP_ML;
    else if (strcmp(systemType, "RZ-300-HDR") == 0) return RED_RZ_300_HDR;
    else return NULL;
}