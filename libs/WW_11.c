//
// Created by kerts on 11/17/2025.
//

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "system_config.h"
#include "utils.h"

// RED for Waste-Water system, Tracy's algorythm, custom D1log (linear)
double RED_WW_11(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps) {
    bool legacy = false;
    const uint32_t NBanks = NLamps / 11;

    const double A = 2.01325936619027;
    const double B = -0.617516338335465;
    const double C = 0.743441109603567;
    const double D = 0.618318746177698;
    const double E = -14.5701310748311;
    const double F = 0.985367251612378;

    const double Tolerance = 0.001;
    const long MaxIterations = 1000;

    double S100 = 1;
    double S50 = 0.78;

    // Get system configuration - construct system name from NBanks
    char system_name[20];
    snprintf(system_name, sizeof(system_name), "WW11-%u", NBanks);

    system_config_t config;
    if (!get_system_config(system_name, &config)) {
        return -1; // System not found
    }

    double P1 = P[0];
    double Eff1 = Eff[0];
    double Peff = P1 * Eff1 / 100.0;

    const double minFlow = config.min_flow;
    const double maxFlow = config.max_flow;
    const double minUVT = config.min_uvt;
    const double maxUVT = config.max_uvt;
    const double minDrive = config.min_drive;
    const double maxDrive = config.max_drive;
    const double minEff = config.min_efficiency;
    const double maxEff = config.max_efficiency;

    if (UVT < minUVT || UVT > maxUVT ||
        P1 < minDrive || P1 > maxDrive ||
        Eff1 < minEff || Eff1 > maxEff ||
        Flow < minFlow || Flow > maxFlow) {
        return -1; // Parameters out of range
    }

    // Initial guess:
    double LI = 2.0;

    // Calculate basic parameters:
    double S_S0 = ((S100 - S50) / (100.0 - 50.0)) * Peff + (S50 - ((S100 - S50) / (100.0 - 50.0)) * 50.0);
    double UVA = -log10(UVT / 100.0);
    double Qlamp = Flow / 11; // lamps per module = 11

    for (int i = 1; i <= MaxIterations; i++) {

        double LI_prev = LI;
        double RED = D1Log * LI;
        double CalcLI = RED / LI;

        LI = pow(10.0, A) * pow(UVA, B) *
                 pow((S_S0) / (Qlamp * CalcLI), (C + D * pow(UVA, 2))) *
                 pow(10.0, (E * ((S_S0) / (Qlamp * CalcLI)))) *
                 pow(NBanks, F);

        if (fabs(LI - LI_prev) < Tolerance) {
            return round_1(RED);
        }
    }

    return -1; // Did not converge
}

// Validated RED for Waste-Water system, Tracy's algorythm, for MS2 pathogen (non-linear D1log)
double RED_WW_11_MS2(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps) {
    bool legacy = false;
    const uint32_t NBanks = NLamps / 11;

    const double A = 2.01325936619027;
    const double B = -0.617516338335465;
    const double C = 0.743441109603567;
    const double D = 0.618318746177698;
    const double E = -14.5701310748311;
    const double F = 0.985367251612378;

    const double Tolerance = 0.001;
    const long MaxIterations = 1000;

    const double MS2_A = 17.8919468343278;
    const double MS2_B = 1.58574991597831;

    double S100 = 1;
    double S50 = 0.78;

    // Get system configuration - construct system name from NBanks
    char system_name[20];
    snprintf(system_name, sizeof(system_name), "WW11-%u", NBanks);

    system_config_t config;
    if (!get_system_config(system_name, &config)) {
        return -1; // System not found
    }

    double P1 = P[0];
    double Eff1 = Eff[0];
    double Peff = P1 * Eff1 / 100.0;

    const double minFlow = config.min_flow;
    const double maxFlow = config.max_flow;
    const double minUVT = config.min_uvt;
    const double maxUVT = config.max_uvt;
    const double minDrive = config.min_drive;
    const double maxDrive = config.max_drive;
    const double minEff = config.min_efficiency;
    const double maxEff = config.max_efficiency;

    if (UVT < minUVT || UVT > maxUVT ||
        P1 < minDrive || P1 > maxDrive ||
        Eff1 < minEff || Eff1 > maxEff ||
        Flow < minFlow || Flow > maxFlow) {
        return -1; // Parameters out of range
    }

    // Initial guess:
    double LI_MS2 = 2.0;

    // Calculate basic parameters:
    double S_S0 = ((S100 - S50) / (100.0 - 50.0)) * Peff + (S50 - ((S100 - S50) / (100.0 - 50.0)) * 50.0);
    double UVA = -log10(UVT / 100.0);
    double Qlamp = Flow / 11; // lamps per module = 11

    for (int i = 1; i <= MaxIterations; i++) {

        double LI_MS2_prev = LI_MS2;
        double RED_MS2 = MS2_A * LI_MS2 + MS2_B * pow(LI_MS2, 2);
        double CalcLI_MS2 = RED_MS2 / LI_MS2;

        LI_MS2 = pow(10.0, A) * pow(UVA, B) *
                 pow((S_S0) / (Qlamp * CalcLI_MS2), (C + D * pow(UVA, 2))) *
                 pow(10.0, (E * ((S_S0) / (Qlamp * CalcLI_MS2)))) *
                 pow(NBanks, F);

        if (fabs(LI_MS2 - LI_MS2_prev) < Tolerance) {
            return round_1(RED_MS2);
        }
    }

    return -1; // Did not converge
}

// Validated RED for Waste-Water system, Tracy's algorythm, for T1 pathogen (non-linear D1log)
double RED_WW_11_T1(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps) {
    bool legacy = false;
    const uint32_t NBanks = NLamps / 11;

    const double A = 2.01325936619027;
    const double B = -0.617516338335465;
    const double C = 0.743441109603567;
    const double D = 0.618318746177698;
    const double E = -14.5701310748311;
    const double F = 0.985367251612378;

    const double Tolerance = 0.001;
    const long MaxIterations = 1000;

    const double T1_A = 4.61013226651316;
    const double T1_B = 0;

    double S100 = 1;
    double S50 = 0.78;

    // Get system configuration - construct system name from NBanks
    char system_name[20];
    snprintf(system_name, sizeof(system_name), "WW11-%u", NBanks);

    system_config_t config;
    if (!get_system_config(system_name, &config)) {
        return -1; // System not found
    }

    double P1 = P[0];
    double Eff1 = Eff[0];
    double Peff = P1 * Eff1 / 100.0;

    const double minFlow = config.min_flow;
    const double maxFlow = config.max_flow;
    const double minUVT = config.min_uvt;
    const double maxUVT = config.max_uvt;
    const double minDrive = config.min_drive;
    const double maxDrive = config.max_drive;
    const double minEff = config.min_efficiency;
    const double maxEff = config.max_efficiency;

    if (UVT < minUVT || UVT > maxUVT ||
        P1 < minDrive || P1 > maxDrive ||
        Eff1 < minEff || Eff1 > maxEff ||
        Flow < minFlow || Flow > maxFlow) {
        return -1; // Parameters out of range
    }

    // Initial guess:
    double LI_T1 = 2.0;

    // Calculate basic parameters:
    double S_S0 = ((S100 - S50) / (100.0 - 50.0)) * Peff + (S50 - ((S100 - S50) / (100.0 - 50.0)) * 50.0);
    double UVA = -log10(UVT / 100.0);
    double Qlamp = Flow / 11; // lamps per module = 11

    for (int i = 1; i <= MaxIterations; i++) {

        double LI_T1_prev = LI_T1;
        double RED_T1 = T1_A * LI_T1 + T1_B * pow(LI_T1, 2);
        double CalcLI_T1 = RED_T1 / LI_T1;

        LI_T1 = pow(10.0, A) * pow(UVA, B) *
                pow((S_S0) / (Qlamp * CalcLI_T1), (C + D * pow(UVA, 2))) *
                pow(10.0, (E * ((S_S0) / (Qlamp * CalcLI_T1)))) *
                pow(NBanks, F);

        if (fabs(LI_T1 - LI_T1_prev) < Tolerance) {
            return round_1(RED_T1);
        }
    }

    return -1; // Did not converge
}