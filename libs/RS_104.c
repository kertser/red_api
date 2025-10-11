//
// Created by kerts on 2/14/2024.
// RS-104 RED calculation
//

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "utils.h"
#include "system_config.h"

// RED for RS104
double RED_RS_104(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps) {
    // Implementation for RS104

    bool legacy = false; // legacy RED function

    // pull the lamp power and efficiency values from the input arrays
    double P1 = P[0];
    double Eff1 = Eff[0];

    // General Formula:
    // TUF(Q,UVT,D1Log,ρ)=A*(Q^B)*(D1Log^C)*EXP(D*(UVT/100)))
    // Davg(P,Q,UVT)=A*((P^α)/(Q^β))*EXP(γ*(UVT/100))

    // Dose Coefficients:

    // Get system configuration - construct system name from NLamps
    char system_name[20];
    snprintf(system_name, sizeof(system_name), "RS104-1%u", NLamps);

    system_config_t config;
    if (!get_system_config(system_name, &config)) {
        return -1;
    }

    // General Coefficients from configuration
    const double LampPower = config.lamp_power;
    const double minFlow = config.min_flow;
    const double maxFlow = config.max_flow;
    const double minUVT = config.min_uvt;
    const double maxUVT = config.max_uvt;
    const double minDrive = config.min_drive;
    const double maxDrive = config.max_drive;
    const double minEff = config.min_efficiency;
    const double maxEff = config.max_efficiency;

    const double Z1 = 0.00013676525437778;
    const double alfa = 1.82647844522826;
    const double beta  = 1;
    const double gama = 8.92498098157656;

    const double A1 = 0.264395667227658;
    const double B1 = 0.0913821393032035;
    const double C1 = 0.0712062582569228;
    const double D1 = 0.777359750748686;

    // Logics:
    /*
    P = Power in [%]
    Eff1 = Efficiency in [%]
    Flow in [m^3/h]
    UVT in [%-1cm]
    D1Log in [mJ/cm^2] - 1-Log dose for a specific pathogen
    NLamps = 1 (single lamp only)
    */

    double AvgDose = Z1*((pow((P1*Eff1/100),alfa))/(pow(Flow,beta)))*exp(gama*(UVT/100));
    double TUF = A1*(pow(Flow,B1))*(pow(D1Log,C1))*exp(D1*(UVT/100));
    if (TUF >1) TUF=1; // TUF<1 always

    // Validation before calculation
    if (UVT < minUVT || UVT > maxUVT ||
        P1 < minDrive || P1 > maxDrive ||
        Flow < minFlow || Flow > maxFlow ||
        Eff1 < minEff || Eff1 > maxEff) {
        return -1; // Return error if parameters are out of range
    }

    return(round_1(AvgDose*TUF));

}
