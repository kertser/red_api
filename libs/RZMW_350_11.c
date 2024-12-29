//
// Created by kerts on 7/8/2024.
// RZMW-350-11 RED calculation
//

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"
#include "system_config.h"
#include <string.h>

// RED for RZMW-350_11 Single Module
double RED_RZMW_350_11(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps) {
    // Implementation for a single module of RZM-200-5

    /*
    General Formula:
    TUF(Q,UVT,D1Log,ρ)=A1*(Q^B1)*(D1Log^C1)*EXP(D1*(UVT/100)))
    Davg(P,Q,UVT)=Z1*((P^α)/(Q^β))*EXP(γ*(UVT/100))
    */

    // Logics
    /*
    P = Power in [%]
    Eff1 = Efficiency in [%]
    Flow in [m^3/h]
    UVT in [%-1cm]
    D1Log in [mJ/cm^2] - 1-Log dose for a specific pathogen
    NLamps = 11
    */

    bool legacy = false; // legacy RED function

    double Dose_VF = 1.21; // Validation Factor (used here)
    double Dose_Correction_Factor = 1; // Dose correction

    // Some specific values for the RZMW RED function
    if (NLamps == 11) Dose_Correction_Factor = 1.0;
    if (NLamps == 7) Dose_Correction_Factor = 0.86;

    // Dose Coefficients:
    // 5 Lamps, but for the moment is valid for 3 and 2 lamps too (fix later)
    double Z1 = 0.0282245386468839;
    double alfa = 1;
    double beta = 1;
    double gama = 4.516177254185;

    double A1 = 0.465456286359808;
    double B1 = 0.0540907147468616;
    double C1 = 0.0504334999703928;
    double D1 = 0.22829306629508;

    double powerDensity = 171.43; // [W/cm]
    double arcLength = 70; //[cm]
    //double LampPower = round_1(powerDensity*arcLength); // [W] 171.43 W/cm on 70cm arc-length

    // Get system configuration
    char system_name[20]; // Buffer for final concatenated string

    // Determine NLampsText based on NLamps
    if (NLamps == 11) {
        strcpy(system_name, "RZMW-350-11");
    } else if (NLamps == 7) {
        strcpy(system_name, "RZMW-350-7");
    } else {
        strcpy(system_name, "");
    }

    system_config_t config;
    if (!get_system_config(system_name, &config)) {
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

    double P0 = 0;
    double Eff0 = 0;

    for (int i = 0; i < NLamps; i++) {
        P0 += P[i];
        Eff0 += Eff[i];
    }
    P0 /= NLamps; // Average Power
    Eff0 /= NLamps; // Average Efficiency

    if (NLamps != 11 && NLamps != 7) return -1; // Invalid number of lamps

    double Davg = Dose_VF*Z1*pow(((arcLength*powerDensity*NLamps)*(P0/100)*(Eff0/100)),alfa)/pow(Flow,beta)*exp(gama*(UVT/100));
    double TUF = min(A1*pow(Flow,B1)*pow(D1Log,C1)*exp(D1*(UVT/100)),1); // TUF<1 always

    // Validation before calculation
    if (UVT < minUVT || UVT > maxUVT ||
        P0 < minDrive || P0 > maxDrive ||
        Flow < minFlow || Flow > maxFlow ||
        Eff0 < minEff || Eff0 > maxEff) {
        return -1; // Return error if parameters are out of range
    }

    return round_1(Davg*TUF*Dose_Correction_Factor);
}

double HeadLoss_RZMW350(double Flow,uint32_t NLamps) {
    // Calculate HeadLoss[cmH2O]

    double HeadLossFactor = 0.0101971621298; // converting from Pa to cmH2O
    double C_Flow1 = 1; // Wrong, shall be changed
    double C_Flow2 = 1; // Wrong, shall be changed

    //TODO: Update the CFlow factors
    if (NLamps == 11) // 11L
        return round_n(HeadLossFactor/1000*(pow(C_Flow1*Flow,2)+C_Flow2*Flow), 2);
    else if (NLamps == 7) // 7L - placeholder
        return round_n(HeadLossFactor/1000*(pow(C_Flow1*Flow,2)+C_Flow2*Flow), 2);

    else return 0;
}

//
// Created by kerts on 7/8/2024.
//