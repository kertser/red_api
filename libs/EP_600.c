//
// Created by kerts on 09/11/2025.
// EP600 RED calculation
//

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "system_config.h"
#include "utils.h"

// RED for EP600
double RED_EP_600(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps) {

    /*
    General Formula:
    Davg(P,Q,UVT)=A*((P^α)/(Q^β))*(1/(LN(100/UVT)))^γ
    TUF(Q,UVT,D1Log,ρ)=A*LN((B*Q)*(1/ρ)*(D1Log^C)*EXP(-D*(100/UVT)))
    Schematic lamp placement:
    <<(L1) (L3)>>
    <<(L2) (L4)>>


    Logic:
    P1 = Power-L1 in [%]
    P2 = Power-L2 in [%]
    P3 = Power-L3 in [%]
    P4 = Power-L4 in [%]
    Eff1 = Efficiency-L1 in [%]
    Eff2 = Efficiency-L2 in [%]
    Eff3 = Efficiency-L3 in [%]
    Eff4 = Efficiency-L4 in [%]
    Flow in [m^3/h]
    UVT in [%-1cm]
    D1Log in [mJ/cm^2] - 1-Log dose for a specific pathogen
    NLamps - Number of lamps in total (2 or 4)
    */

    bool legacy = false;

    // Get system configuration - construct system name from NLamps
    char system_name[20];
    snprintf(system_name, sizeof(system_name), "EP600-%u", NLamps);

    system_config_t config;
    if (!get_system_config(system_name, &config)) {
        return -1;
    }

    // Pull the lamp power and efficiency values from the input arrays
    double P1 = P[0];
    double P2 = P[1];  // Always exists for both 2 and 4 lamp configs
    double P3 = (NLamps == 4) ? P[2] : 0;
    double P4 = (NLamps == 4) ? P[3] : 0;

    double Eff1 = Eff[0];
    double Eff2 = Eff[1];  // Always exists for both 2 and 4 lamp configs
    double Eff3 = (NLamps == 4) ? Eff[2] : 0;
    double Eff4 = (NLamps == 4) ? Eff[3] : 0;


    // Dose Coefficients:
    // EP-4L: - Base
    const double Z1 = 1.22061316462651;
    const double alfa4L = 1;
    const double beta4L = 1;
    const double gama4L = 0.873010931124947;

    const double A1 = 0.1162607858509;
    const double B1 = 0.371030699431417;
    const double C1 = 1.22077503287731;
    const double D1 = 3.73561624639757;

    // EP-4L: Single lamp "lead"
    const double Z1_lead = 0.404420945319522;
    const double alfa4L_lead = 1;
    const double beta4L_lead = 1;
    const double gama4L_lead = 0.83535465366486;

    const double A1_lead = 0.148510660115206;
    const double B1_lead = 0.0123403695243628;
    const double C1_lead = 0.87263620883785;
    const double D1_lead = 1.91929195322967;

    // EP-4L: Single lamp "lag"
    const double Z1_lag = 0.300850569870035;
    const double alfa4L_lag = 1;
    const double beta4L_lag = 1;
    const double gama4L_lag = 0.912852990058308;

    const double A1_lag = 0.147655833107015;
    const double B1_lag = 0.00492768115732762;
    const double C1_lag = 0.942395843866067;
    const double D1_lag = 1.21088165728498;

    // EP-2L: Base
    const double Z2 = 0.612763221395606;
    const double alfa2L = 1;
    const double beta2L = 1;
    const double gama2L = 0.812375160491567;

    const double A2 = 0.128121865880793;
    const double B2 = 1.36377755754535;
    const double C2 = 1.0721706738148;
    const double D2 = 4.17084478397804;

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

    // Validation before calculation - validate all active lamps
    if (UVT < minUVT || UVT > maxUVT ||
        P1 < minDrive || P1 > maxDrive ||
        Eff1 < minEff || Eff1 > maxEff ||
        Flow < minFlow || Flow > maxFlow) {
        return -1;
    }

    // Additional validation for multi-lamp systems
    if (NLamps >= 2) {
        if (P2 < minDrive || P2 > maxDrive || Eff2 < minEff || Eff2 > maxEff) {
            return -1;
        }
    }
    if (NLamps >= 3) {
        if (P3 < minDrive || P3 > maxDrive || Eff3 < minEff || Eff3 > maxEff) {
            return -1;
        }
    }
    if (NLamps >= 4) {
        if (P4 < minDrive || P4 > maxDrive || Eff4 < minEff || Eff4 > maxEff) {
            return -1;
        }
    }

    // Calculate effective power for each lamp (P*Eff/100)
    double P1_eff = P1 * Eff1;
    double P2_eff = P2 * Eff2;
    double P3_eff = P3 * Eff3;
    double P4_eff = P4 * Eff4;

    // Find minimum effective power
    double minPower;
    if (NLamps == 4) {
        minPower = fmin(fmin(P1_eff, P2_eff), fmin(P3_eff, P4_eff));
    } else if (NLamps == 2) {
        minPower = fmin(P1_eff, P2_eff);
    } else {
        return -1; // Invalid lamp count
    }

    double lnUVT = log(100.0 / UVT);

    // Calculate AvgDose components
    double DAvgBase;

    if (NLamps == 4) {
        // DAvgBase for 4 lamps
        DAvgBase = Z1 * pow((minPower / 100.0 / 100.0 * LampPower), alfa4L) /
                   pow(Flow, beta4L) * pow(1.0 / lnUVT, gama4L);
    } else { // NLamps == 2
        // DAvgBase for 2 lamps
        DAvgBase = Z2 * pow((minPower / 100.0 / 100.0 * LampPower), alfa2L) /
                   pow(Flow, beta2L) * pow(1.0 / lnUVT, gama2L);
    }

    // Calculate TUFBase
    double TUFBase;
    if (NLamps == 4) {
        TUFBase = A1 * log((B1 * Flow) * pow(D1Log, C1) * exp(-D1 * (100.0 / UVT)));
    } else { // NLamps == 2
        TUFBase = A2 * log((B2 * Flow) * pow(D1Log, C2) * exp(-D2 * (100.0 / UVT)));
    }

    // Calculate final RED (Base only, not considering non-equal lamp power)
    double RED = DAvgBase * TUFBase;

    return round_1(RED);
}

