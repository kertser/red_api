#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

int main() {
    // declare system type
    char systemType[] = "RZM-350-8"; // System Type

    // Load the library
    LIB_HANDLE handle = LOAD_LIBRARY(LIB_NAME);
    if (!handle) {
#ifdef _WIN32
        fprintf(stderr, "Failed to load library. Error code: %lu\n", GetLastError());
#else
        fprintf(stderr, "Failed to load library: %s\n", dlerror());
#endif
        return 1;
    }

    // Get getNLamps function
    GetNLampsType getNLampsFunc = (GetNLampsType)GET_PROC_ADDRESS(handle, "getNLamps");
    if (!getNLampsFunc) {
        fprintf(stderr, "Unable to find function getNLamps\n");
        FREE_LIBRARY(handle);
        return 1;
    }

    // Declare the parameters
    uint32_t NLamps = 0; // 0 is for automatic detection
    // Get the number of lamps from the system type, unless manually set
    if (NLamps == 0) {
        NLamps = getNLampsFunc(systemType);
    }

    double Flow = 100; //[m^3/h]
    double UVT = 95; //[%-1cm]
    double UVT215 = -1; //[%-1cm] or -1 if NaN
    double Power = 100; //[%], defined for every lamp
    double Efficiency = 80; //[%], defined for every lamp
    double D1Log = 18; // [mJ/cm^2]

    // Allocate memory
    double *P = (double *)malloc(NLamps * sizeof(double));
    double *Eff = (double *)malloc(NLamps * sizeof(double));

    // Check if memory allocation was successful
    if (P == NULL || Eff == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        FREE_LIBRARY(handle);
        return 1;
    }

    // Initialize the P and Eff values
    for (uint32_t i = 0; i < NLamps; i++) {
        P[i] = Power;
        Eff[i] = Efficiency;
    }

    // Get supported systems function
    const char** (*getSupportedSystemsPtr)(size_t*);
    getSupportedSystemsPtr = (const char** (*)(size_t*))GET_PROC_ADDRESS(handle, "ListOfSupportedSystems");
    if (!getSupportedSystemsPtr) {
        fprintf(stderr, "Unable to find function ListOfSupportedSystems\n");
        free(P);
        free(Eff);
        FREE_LIBRARY(handle);
        return 1;
    }

    // Print the list of supported systems
    size_t size;
    const char** systems = getSupportedSystemsPtr(&size);
    printf("Supported systems:\n");
    for (size_t i = 0; i < size; ++i) {
        printf("%s\n", systems[i]);
    }
    printf("------------------\n");

    // Get RED function
    GetREDFunctionType getREDFunction = (GetREDFunctionType)GET_PROC_ADDRESS(handle, "getREDFunction");
    if (!getREDFunction) {
        fprintf(stderr, "Unable to find function getREDFunction\n");
        free(P);
        free(Eff);
        FREE_LIBRARY(handle);
        return 1;
    }

    // Get RED function for specific system type
    REDFunction redFunction = getREDFunction(systemType);
    if (!redFunction) {
        fprintf(stderr, "Error: Unknown system type\n");
        free(P);
        free(Eff);
        FREE_LIBRARY(handle);
        return 2;
    }

    // Calculate RED
    double result = redFunction(Flow, UVT, UVT215, P, Eff, D1Log, NLamps);
    printf("Calculated RED for %s = %.1f\n", systemType, result);

    // Cleanup
    free(P);
    free(Eff);
    FREE_LIBRARY(handle);
    return 0;
}