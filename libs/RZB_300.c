//
// Created by kerts on 2/14/2024.
// RZB300 RED calculation
//

#include <math.h>
#include <stdbool.h>
#include "utils.h"
#include "system_config.h"

// RED for RZB300 Single Module
double RED_RZB_300(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps) {
    // Implementation for a single module of RZB

    bool legacy = false; // legacy RED function

    // pull the lamp power and efficiency values from the input arrays
    double P1 = P[0];
    double P2 = P[1];
    double Eff1 = Eff[0];
    double Eff2 = Eff[1];

    // General Formula:
    // Davg(P,Q,UVT)=A*((P^α)/(Q^β))*(1/(LN(100/UVT)))^γ
    // TUF(Q,UVT,D1Log,ρ)=A*(Q^B)*(D1Log^C)*EXP(D*(UVT/100)))

    // Dose Coefficients:
    // 2 lamps:
    const double Z1 = 1.73388373339374;
    const double alfa2L = 0.793100554872894;
    const double beta2L = 0.84248663116176;
    const double gama2L = 0.654305893279355;

    const double A1 = 0.0696569303096432;
    const double B1 = 0.0803790453053116;
    const double C1 = 0.11643116418818;
    const double D1 = 1.89455003428108;

    // for lampd different power, when they are running at different drive (unused in general case)
    const double Z2 = 0.395152724682959;
    const double alfa1L = 1;
    const double beta1L = 1;
    const double gama1L = 0.691674473925155;

    const double A2 = 0.000121301900463444;
    const double B2 = 0.302772713177836;
    const double C2 = 0.369722425245504;
    const double D2 = 5.91491910054795;

    // Get system configuration
    system_config_t config;
    if (!get_system_config("RZB300", &config)) {
        return -1; // Return error if configuration cannot be loaded
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

    // General Logic:
    /*
    P1 = Power-L1 in [%]
    P2 = Power-L2 in [%]
    Eff1 = Efficiency-L1 in [%]
    Eff2 = Efficiency-L2 in [%]
    Flow in [m^3/h]
    UVT in [%-1cm]
    D1Log in [mJ/cm^2] - 1-Log dose for a specific pathogen
    NLamps - Number of lamps in total (pay attention, that in single module it is 2)
    */


    double AvgDose = Z1 * pow((min(P1*Eff1/100,P2*Eff2/100)/100*LampPower*2),alfa2L) / pow(Flow,beta2L) * pow(1/(log(100/UVT)),gama2L);
    double TUF = A1 * pow(Flow,B1) * pow(D1Log,C1) * exp(D1*(UVT/100));

    // Validation before calculation
    if (UVT < minUVT || UVT > maxUVT ||
        P1 < minDrive || P1 > maxDrive ||
        Flow < minFlow || Flow > maxFlow ||
        Eff1 < minEff || Eff1 > maxEff) {
        return -1; // Return error if parameters are out of range
    }

    return round_1((NLamps/2)*AvgDose*TUF);

}
