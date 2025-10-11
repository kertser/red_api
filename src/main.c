#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "system_config.h"

#ifdef _WIN32
    #include <windows.h>
    #define LIB_HANDLE HMODULE
    #define LOAD_LIBRARY(name) LoadLibrary(name)
    #define GET_PROC_ADDRESS GetProcAddress
    #define FREE_LIBRARY FreeLibrary
    #define LIB_NAME "red_api.dll"
#else
#include <dlfcn.h>
    #define LIB_HANDLE void*
    #define LOAD_LIBRARY(name) dlopen(name, RTLD_LAZY)
    #define GET_PROC_ADDRESS dlsym
    #define FREE_LIBRARY dlclose
    #define LIB_NAME "./libred_api.so"
#endif

typedef double (*REDFunction)(double, double, double, double[], double[], double, uint32_t);
typedef REDFunction (*GetREDFunctionType)(char *);
typedef uint32_t (*GetNLampsType)(const char*);

// Function to clean up resources
static void cleanup_resources(LIB_HANDLE handle, double *P, double *Eff) {
    if (P) free(P);
    if (Eff) free(Eff);
    if (handle) FREE_LIBRARY(handle);
    cleanup_system_config();
}

int main() {
    // Example usage of the RED calculation library
    LIB_HANDLE handle = NULL;
    double *P = NULL;
    double *Eff = NULL;
    int return_code = 0;

    // Initialize system configuration
    if (!init_system_config()) {
        fprintf(stderr, "Failed to load system configuration\n");
        return 1;
    }

    // Load the library
    handle = LOAD_LIBRARY(LIB_NAME);
    if (!handle) {
#ifdef _WIN32
        fprintf(stderr, "Failed to load library. Error code: %lu\n", GetLastError());
#else
        fprintf(stderr, "Failed to load library: %s\n", dlerror());
#endif
        cleanup_system_config();
        return 1;
    }

    // Get system configuration
    const char *systemType = "RZMW350-11";
    system_config_t config;
    if (!get_system_config(systemType, &config)) {
        fprintf(stderr, "Failed to get configuration for system %s\n", systemType);
        cleanup_resources(handle, NULL, NULL);
        return 1;
    }

    // Get number of lamps
    uint32_t NLamps = get_lamp_count(systemType);
    if (NLamps == 0) {
        fprintf(stderr, "Failed to get lamp count for system %s\n", systemType);
        cleanup_resources(handle, NULL, NULL);
        return 1;
    }

    // Set operational parameters
    const double Flow = 100.0;        // [m³/h]
    const double UVT = 95.0;          // [%-1cm]
    const double UVT215 = -1.0;       // [%-1cm] or -1 if NaN
    const double Power = 100.0;       // [%]
    const double Efficiency = 80.0;   // [%]
    const double D1Log = 18.0;        // [mJ/cm²]

    // Validate parameters
    if (!validate_parameters(systemType, Flow, UVT, Power, Efficiency)) {
        fprintf(stderr, "Parameters out of valid range for system %s\n", systemType);
        cleanup_resources(handle, NULL, NULL);
        return 1;
    }

    // Allocate memory for lamp arrays
    P = (double *)calloc(NLamps, sizeof(double));
    Eff = (double *)calloc(NLamps, sizeof(double));

    if (!P || !Eff) {
        fprintf(stderr, "Memory allocation failed\n");
        cleanup_resources(handle, P, Eff);
        return 1;
    }

    // Initialize lamp arrays
    for (uint32_t i = 0; i < NLamps; i++) {
        P[i] = Power;
        Eff[i] = Efficiency;
    }

    // Print supported systems
    size_t size;
    const char** systems = get_supported_systems(&size);
    if (systems) {
        printf("\nSupported systems:\n");
        for (size_t i = 0; i < size; ++i) {
            printf("- %s\n", systems[i]);
        }
        printf("------------------\n\n");
    }

    // Get RED calculation function
    GetREDFunctionType getREDFunction = (GetREDFunctionType)GET_PROC_ADDRESS(handle, "getREDFunction");
    if (!getREDFunction) {
        fprintf(stderr, "Unable to find function getREDFunction\n");
        cleanup_resources(handle, P, Eff);
        return 1;
    }

    // Get specific RED function for system type
    REDFunction redFunction = getREDFunction((char *)systemType);
    if (!redFunction) {
        fprintf(stderr, "Error: Unknown system type\n");
        cleanup_resources(handle, P, Eff);
        return 1;
    }

    // Calculate RED
    double result = redFunction(Flow, UVT, UVT215, P, Eff, D1Log, NLamps);

    // Print results
    printf("System Configuration:\n");
    printf("-------------------\n");
    printf("Model: %s\n", systemType);
    printf("Lamp Power: %.0f W\n", get_lamp_power(systemType));
    printf("Number of Lamps: %u\n", NLamps);
    printf("\nOperational Parameters:\n");
    printf("---------------------\n");
    printf("Flow Rate: %.1f m³/h\n", Flow);
    printf("UVT: %.1f %%\n", UVT);
    printf("Power: %.1f %%\n", Power);
    printf("Efficiency: %.1f %%\n", Efficiency);
    printf("\nCalculation Result:\n");
    printf("-----------------\n");
    printf("RED Value: %.1f mJ/cm²\n", result);

    // Cleanup and exit
    cleanup_resources(handle, P, Eff);
    return return_code;
}