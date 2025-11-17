//
// Created by kerts on 17/11/2025.
//

#ifndef APICALCULATOR_WW_11_H
#define APICALCULATOR_WW_11_H

// RED for WasteWater system, Tracy's algorythm, custom D1log (linear)
double RED_WW_11(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps);
// RED for Waste-Water system, Tracy's algorythm, for T1 pathogen (non-linear D1log)
double RED_WW_11_T1(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps);
// RED for Waste-Water system, Tracy's algorythm, for MS2 pathogen (non-linear D1log)
double RED_WW_11_MS2(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps);

#endif //APICALCULATOR_WW_11_H