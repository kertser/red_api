// red_api.c

#include "red_api.h"
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <RZ_104_11.h>

//List of supported systems
static const char* supportedSystems[] = {
        "RZ-104-11",
        "RZ-300-HDR",
};

// Coefficients structure for the new-type RED function - will be used later
struct RED_coefficients
{
    double alpha,beta,gama;
    double A,B,C;
};

// Function to return the entire list of systems
const char** ListOfSupportedSystems(size_t* size) {
    *size = sizeof(supportedSystems) / sizeof(supportedSystems[0]);
    return supportedSystems;
}

// System function selector
REDFunction getREDFunction(char *systemType) {
    if (strcmp(systemType, "RZ-104-11") == 0) return RED_RZ_104_11;
    else if (strcmp(systemType, "RZ-300-HDR") == 0) return RED_RZ_300_HDR;
    else return NULL;
}