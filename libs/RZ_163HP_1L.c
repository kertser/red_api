//
// Created by kerts on 2/14/2024.
// RZ-163-11 HP RED calculation
//

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"

// RED for RZ-163HP-1L (legacy)
double RED_RZ_163HP_1L(double Flow, double UVT, double P, double Eff, double D1Log, uint32_t NLamps) {
    // Implementation for RZ-163HP-1L

    bool legacy = true; // legacy RED function

    // General Coefficients
    const double LampPower = 2325; //[W] - 110W/cm for 95mm arc-length lamp
    const double minFlow = 4; //[m3h];
    const double maxFlow = 2000; //[m3h]
    const double minUVT = 79; //[%-1cm]
    const double maxUVT = 96.6; //[%-1cm]
    const double minDrive = 25; //[%]
    const double maxDrive = 100; //[%]
    const double minLeff = 50; //[%]
    const double maxLeff = 100; //[%]
    const double maxLampSpecificPower = 150; //[W/cm]

    // Additional Constants
    const double TAD0 = 500;
    const double TUF0 = 4.0112222222;
    const double TUF1 = -0.0882222222;
    const double TUF2 = 0.0005555556;
    const double TUF_minimum = 0.05;

    const double n_coeff_0 = 18.9956903731179;
    const double n_coeff_1 = -0.150039130073;
    const double n_coeff_2 = -0.000377119685;
    const double n_limit = 1;

    const double L_eff_coeff_0 = 1271.8934153816;
    const double L_eff_coeff_1 = -72.4457478963446;
    const double L_eff_coeff_2 = 1.52438553120442;
    const double L_eff_coeff_3 = -0.014089865266356;
    const double L_eff_coeff_4 = 0.000048456108395;

    const double NL0 = 0.122959698122207;
    const double NL1 = 0.015753059974049;
    const double NL2 = -0.000069826569553;
    const double eta_g = 0.1161;
    const double UVT_step = 75;
    const double n_delta_ext = 16;

    const double q_min = 0.1;

    const double eta_coupling = 1;
    const double L_react_L_eff_ratio = 3.5;
    const double Unit_converter_1 = 1/3.6;
    const double Unit_converter_2 = 157.725; //(m3h to MGD)

    // Preparation towards the calculation:

    double NLF = NL0+NL1*P+NL2*pow(P,2);
    double Eta_UV = eta_g*eta_coupling*(Eff/100)*NLF;
    double PQR = (LampPower*(P/100))/Flow;
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

    NLF = NL0+NL1*P+NL2*pow(P,2);

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

    double RED;
    if ((UVT < minUVT) | (UVT > maxUVT) | (P < minDrive) | (Flow < minFlow) | (Eff < minLeff))
        RED = -1; //'Error' - pay attention to minimum UVT requirements
    else
        RED = TUF*TAD;

    return round_1(RED);

}
