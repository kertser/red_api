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

// RED for RZ-300 (HydraQual algorithm)
double RED_RZ_300_HDR(double Flow, double UVT, double P, double Eff, double D1Log, uint32_t NLamps) {
    // The function is for HDR validation model

    /*
    #General scheme for HDR calculator:

    RED = 10a * (P/100)b * (Q)c * (1/Abs)d * * (UVS)e * 10(f/abs) * Ng
    abs = -LOG(UVT/100)

    UVS = D1Log average inactivation
    N = number of lamps
    Flow units - [m3/h]
    */

    bool legacy = false; // legacy RED function

    // Dose Coefficents:
    const double a = 1.9723460342;
    const double b = 1.1342181946;
    const double c = -0.89447843101;
    const double d = 1.6521724013;
    const double e = 0.1018468648;
    const double f = -0.0060928379303;
    const double g = 1.1855262222;

    const double VF_A = 1.006642717; // VF calculation
    const double VF_B = 6.181224702; // VF calculation

    const double LampPower = 4200; // [W]
    const double minFlow = 30; //[m^3h]
    const double maxFlow = 500; //[m^3h]
    
    double RED;
    if (NLamps>2)
        RED = pow(10,a) * pow((P / 100 * Eff / 100) , b) * pow((Flow / 0.2271) , c) * pow((1 / (-log10(UVT / 100))) , d) * pow(
                    D1Log , e) * pow(10 , (f / (-log10(UVT / 100)))) * (NLamps / 2 * pow(2 , g));
    else
        RED = pow(10,a) * pow((P / 100 * Eff / 100) , b) * pow((Flow / 0.2271) , c) * pow((1 / (-log10(UVT / 100))) , d) * pow(
                    D1Log , e) * pow(10 , (f / (-log10(UVT / 100)))) * pow(NLamps , g);
    return RED;
}

// System function selector
REDFunction getREDFunction(char *systemType) {
    if (strcmp(systemType, "RZ-104-11") == 0) return RED_RZ_104_11;
    else if (strcmp(systemType, "RZ-300-HDR") == 0) return RED_RZ_300_HDR;
    else return NULL;
}