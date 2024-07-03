#include <stdio.h>
#include <windows.h>
#include <stdint.h>

typedef double (*REDFunction)(double, double, double, double[], double[], double, uint32_t); // RED with UVT254 and UVT215
typedef REDFunction (*GetREDFunctionType)(char *);

int main() {
    // Usage example

    // declare system type
    char systemType[] = "RZM-350-8"; // System Type

    // Declare the parameters:
    #define NLAMPS 8 //has to be static for array definition in C
    uint32_t NLamps = NLAMPS; // Number of Lamps initialized
    double Flow = 100; //[m^3/h]
    double UVT = 95; //[%-1cm]
    double UVT215 = -1; //[%-1cm] or -1 if NaN
    double P[NLAMPS] = {100,100,100,100,100,100,100,100}; // [%], defined for every lamp
    double Eff[NLAMPS] = {80,80,80,80,80,80,80,80}; //[%], defined for every lamp
    double D1Log = 18; // [mJ/cm^2]

    //Change the path accordingly
    HMODULE hDll = LoadLibrary("libred_api.dll");
    if (hDll == NULL) {
        fprintf(stderr, "Unable to load DLL\n");
        return 1;
    }

    // Declare and get the function pointer
    const char** (*getSupportedSystemsPtr)(size_t* size);
    getSupportedSystemsPtr = (const char** (*)(size_t*))GetProcAddress(hDll, "ListOfSupportedSystems");
    if (getSupportedSystemsPtr == NULL) {
        fprintf(stderr, "Unable to find function ListOfSupportedSystems\n");
        FreeLibrary(hDll);
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

    // Declare and get the function pointer
    GetREDFunctionType getREDFunction = (GetREDFunctionType)GetProcAddress(hDll, "getREDFunction");
    if (getREDFunction == NULL) {
        fprintf(stderr, "Unable to find function getREDFunction\n");
        FreeLibrary(hDll);
        return 1;
    }

    // Estimate the RED system type presence
    REDFunction redFunction = getREDFunction(systemType);
    if (redFunction == NULL) {
        fprintf(stderr, "Error: Unknown system type\n");
        FreeLibrary(hDll);
        return 2;
    }

    // Solve for RED, using previously declared parameters
    double result = redFunction(Flow, UVT, UVT215, P, Eff, D1Log, NLamps);
    printf("Calculated RED for %s = %f\n", systemType, result);

    FreeLibrary(hDll);
    return 0;
}