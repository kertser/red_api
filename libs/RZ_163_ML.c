//
// Created by kerts on 2/14/2024.
// RZ-163-ML RED calculation
//

#include <math.h>
#include <stdbool.h>
#include "utils.h"
#include "system_config.h"
#include <stdio.h>
#include <string.h>

// RED for RZ-163-ML (legacy)
double RED_RZ_163_ML(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps) {
    // Implementation for RZ-163-ML

    bool legacy = true; // legacy RED function

    // pull the lamp power and efficiency values from the input arrays
    double P1 = P[0]; // assuming same power for all lamps
    double Eff1 = Eff[0]; // assuming same efficiency for all lamps

    // Get system configuration
    char NLampsText[3]; // Buffer for NLampsText (2 characters + null terminator)
    char system_name[20]; // Buffer for final concatenated string

    // Determine NLampsText based on NLamps
    if (NLamps == 2) {
        strcpy(NLampsText, "12");
    } else if (NLamps == 3) {
        strcpy(NLampsText, "13");
    } else if (NLamps == 4) {
        strcpy(NLampsText, "14");
    } else {
        strcpy(NLampsText, "");
    }
    snprintf(system_name, sizeof(system_name), "RZ-163-%s", NLampsText);

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

    const double minLeff = 50; //[%]
    const double maxLeff = 100; //[%]
    const double maxLampSpecificPower = 150; //[W/cm]

    // Additional Constants
    const double TAD0 = 500;
    const double TUF0 = 4.0112222222;
    const double TUF1 = -0.0882222222;
    const double TUF2 = 0.0005555556;
    const double TUF_minimum = 0.05;

    const double n_coeff_0 = -9.2390722222;
    const double n_coeff_1 = 0.2899138889;
    const double n_coeff_2 = -0.0018263889;
    const double n_limit = 1;

    const double L_eff_coeff_0 = 1418.38465650608;
    const double L_eff_coeff_1 = -79.6704348572632;
    const double L_eff_coeff_2 = 1.66066189728908;
    const double L_eff_coeff_3 = -0.015253483883655;
    const double L_eff_coeff_4 = 0.000052239759634;

    const double NL0 = 0.229714750061484;
    const double NL1 = 0.015132695105295;
    const double NL2 = -0.000074298426059;
    const double eta_g = 0.1156;
    const double UVT_step = 75;
    const double n_delta_ext = 16;

    const double q_min = 0.1;

    const double eta_coupling = 1;
    const double L_react_L_eff_ratio = 3.5;
    const double Unit_converter_1 = 1/3.6;
    const double Unit_converter_2 = 157.725; //(m3h to MGD)

    // Preparation towards the calculation:

    double NLF = NL0+NL1*P1+NL2*pow(P1,2);
    double Eta_UV = eta_g*eta_coupling*(Eff1/100)*NLF;
    double PQR = NLamps * (LampPower*(P1/100))/Flow;
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
