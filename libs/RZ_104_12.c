//
// Created by kerts on 2/7/2024.
// RZ-104-12 RED calculation
//

#include <math.h>
#include <stdbool.h>
#include "utils.h"
#include "system_config.h"

// RED for RZ-104-12 (legacy)
double RED_RZ_104_12(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps) {
    // Implementation for RZ-104-11

    bool legacy = true; // legacy RED function

    // pull the lamp power and efficiency values from the input arrays
    double P1 = P[0]; // assuming the same power for all lamps
    double Eff1 = Eff[0]; // assuming the same efficiency for all lamps

    // Get system configuration
    system_config_t config;
    if (!get_system_config("RZ-104-12", &config)) {
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

    const double minLeff = 50; //[%]
    const double maxLeff = 100; //[%]
    const double maxLampSpecificPower = 110; //[W/cm]

    // Additional Constants
    const double TAD0 = 550;
    const double TUF0 = 7.819318768;
    const double TUF1 = -0.185634817;
    const double TUF2 = 0.001174675451;
    const double TUF_minimum = 0.05;

    const double n_coeff_0 = -338.153201108684;
    const double n_coeff_1 = 6.784162382758;
    const double n_coeff_2 = -0.030026377486;
    const double n_limit = 12.47;

    const double L_eff_coeff_0 = 943.537556579581;
    const double L_eff_coeff_1 = -53.479800936044;
    const double L_eff_coeff_2 = 1.119891116076;
    const double L_eff_coeff_3 = -0.010289398513;
    const double L_eff_coeff_4 = 0.000035131549;

    const double NL0 = 0.353922234366007;
    const double NL1 = 0.01090976049075;
    const double NL2 = -0.000044489828344;
    const double eta_g = 0.117982558336766;
    const double UVT_step = 75;
    const double n_delta_ext = 5;

    const double q_min = 0.1;

    const double eta_coupling = 1;
    const double L_react_L_eff_ratio = 3.5;
    const double Unit_converter_1 = 1/3.6;
    const double Unit_converter_2 = 157.725; //(m3h to MGD)

    // Preparation towards the calculation:

    double NLF = NL0+NL1*P1+NL2*pow(P1,2);
    double Eta_UV = eta_g*eta_coupling*(Eff1/100)*NLF;
    double PQR = NLamps*(LampPower*(P1/100))/Flow;
    double L_eff_step = L_eff_coeff_0+L_eff_coeff_1*UVT_step+L_eff_coeff_2*pow(UVT_step,2)
                        +L_eff_coeff_3*pow(UVT_step,3)+L_eff_coeff_4*pow(UVT_step,4);
    double alfa_step =-log(UVT_step/100);
    double alfa = -log(UVT/100);
    double L_eff_scale = L_eff_step*alfa_step;

    double L_eff;
    if (UVT>UVT_step)
        L_eff = L_eff_coeff_0+L_eff_coeff_1*UVT+L_eff_coeff_2*pow(UVT,2)+L_eff_coeff_3*pow(UVT,3)+L_eff_coeff_4*pow(UVT,4);
    else
        L_eff = (L_eff_scale/alfa);

    double TAD = (1/Unit_converter_1)*Eta_UV*PQR*L_eff; //[mJ/cm^2]
    double q_step = TUF0+TUF1*UVT_step+TUF2*pow(UVT_step,2);
    double q_slope = (q_min-q_step)/UVT_step;

    double q;
    if (UVT > UVT_step)
        q = TUF0+TUF1*UVT+TUF2*pow(UVT,2);
    else
        q = q_step+q_slope*(UVT_step-UVT);

    double n_step = n_coeff_0+n_coeff_1*UVT_step+n_coeff_2*pow(UVT_step,2);
    double n_slope = n_delta_ext/UVT_step;

    double n_intermediate;
    if (UVT > UVT_step)
        n_intermediate = n_coeff_0+n_coeff_1*UVT+n_coeff_2*pow(UVT,2);
    else
        n_intermediate = n_step+n_slope*(UVT_step-UVT);

    double n;
    if (n_intermediate > n_limit)
        n = n_intermediate;
    else
        n = n_limit;

    double TAD_TAD0 = pow(((TAD0-TAD)/TAD0),2);

    double TAD_N;
    if (TAD <= TAD0)
        TAD_N = pow(TAD_TAD0,n);
    else
        TAD_N = 0;

    double TUF_inter=q+(1-q)*TAD_N;

    double TUF;
    if (TUF_inter < TUF_minimum)
        TUF = TUF_minimum;
    else
        TUF = TUF_inter;

    // Validation before calculation
    if (UVT < minUVT || UVT > maxUVT ||
        P1 < minDrive || P1 > maxDrive ||
        Flow < minFlow || Flow > maxFlow ||
        Eff1 < minEff || Eff1 > maxEff) {
        return -1; // Return error if parameters are out of range
    }

    double RED;
    if ((UVT < minUVT) | (UVT > maxUVT) | (P1 < minDrive) | (Flow < minFlow) | (Eff1 < minLeff))
        RED = -1; //'Error' - pay attention to minimum UVT requirements
    else
        RED = TUF*TAD;

    return round_1(RED);

}
