#include <stdio.h>
#include <windows.h>
#include <stdint.h>

typedef double (*REDFunction)(double, double, double, double, double, uint32_t);
typedef REDFunction (*GetREDFunctionType)(char *);

void printLastError() {
    DWORD errorMessageID = GetLastError();
    if(errorMessageID == 0) {
        return; // No error message has been recorded
    }

    LPSTR messageBuffer = NULL;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    printf("Error: %s\n", messageBuffer);
    LocalFree(messageBuffer);
}

int main() {
    // Usage example
    char systemType[] = "RZ-104-11";
    double Flow = 100, UVT = 92, P = 100, Eff = 100, D1Log = 18;
    uint32_t NLamps = 1;

    HMODULE hDll = LoadLibrary("X:\\CPP projects\\API calculator\\libAPICalculator.dll");
    if (hDll == NULL) {
        fprintf(stderr, "Unable to load DLL\n");
        return 1;
    }

    GetREDFunctionType getREDFunction = (GetREDFunctionType)GetProcAddress(hDll, "getREDFunction");
    if (getREDFunction == NULL) {
        fprintf(stderr, "Unable to find getREDFunction\n");
        FreeLibrary(hDll);
        return 1;
    }

    REDFunction redFunction = getREDFunction(systemType);
    if (redFunction == NULL) {
        fprintf(stderr, "Error: Unknown system type\n");
        FreeLibrary(hDll);
        return 2;
    }

    double result = redFunction(Flow, UVT, P, Eff, D1Log, NLamps);
    printf("Calculated RED = %f\n", result);

    FreeLibrary(hDll);
    return 0;
}