//
// Created by kerts on 2/07/2024.
// RZ-300-HDR RED calculation
//

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

// RED for RZ-300 (HydraQual algorithm)
double RED_RZ_300_HDR(double Flow, double UVT, double P[], double Eff[], double D1Log, uint32_t NLamps) {
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

    // pull the lamp power and efficiency values from the input arrays
    double P1 = P[0]; // assuming same power for all lamps
    double Eff1 = Eff[0]; // assuming same efficiency for all lamps

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
        RED = pow(10,a) * pow((P1 / 100 * Eff1 / 100) , b) * pow((Flow / 0.2271) , c) * pow((1 / (-log10(UVT / 100))) , d) * pow(
                D1Log , e) * pow(10 , (f / (-log10(UVT / 100)))) * (NLamps / 2 * pow(2 , g));
    else
        RED = pow(10,a) * pow((P1 / 100 * Eff1 / 100) , b) * pow((Flow / 0.2271) , c) * pow((1 / (-log10(UVT / 100))) , d) * pow(
                D1Log , e) * pow(10 , (f / (-log10(UVT / 100)))) * pow(NLamps , g);
    return RED;
}
