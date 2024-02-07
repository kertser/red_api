// red_api.c

#include "red_api.h"
#include "RZ_104_11.h"
#include "RZ_300_HDR.h"
#include <string.h>

// Coefficients structure for the new-type RED function - will be used later
struct RED_coefficients
{
    double alpha,beta,gama;
    double A,B,C;
};

char** loadSupportedSystems(const char* filename, int* count);

// Function to return the entire list of systems
const char** ListOfSupportedSystems(size_t* size) {
    int count = 0;

    char** supportedSystems = loadSupportedSystems(SUPPORTED_SYSTEMS_PATH, &count);

    *size = count; // Set the size for the caller
    return (const char **) supportedSystems; // Cast to const char** when returning
}

// System function selector
REDFunction getREDFunction(char *systemType) {
    if (strcmp(systemType, "RZ-104-11") == 0) return RED_RZ_104_11;
    else if (strcmp(systemType, "RZ-300-HDR") == 0) return RED_RZ_300_HDR;
    else return NULL;
}