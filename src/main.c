#include <stdio.h>
#include <windows.h>
#include <stdint.h>

typedef double (*REDFunction)(double, double, double, double, double, double, uint32_t); // RED with UVT254 and UVT215
typedef REDFunction (*GetREDFunctionType)(char *);

int main() {
    // Usage example

    //char systemType[] = "RZ-300-HDR"; // System Type
    char systemType[] = "RZ-163UHP-12"; // System Type
    double Flow = 100, UVT = 95, UVT215 = -1, P = 100, Eff = 100, D1Log = 18; // General settings
    uint32_t NLamps = 2; // Number of Lamps

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