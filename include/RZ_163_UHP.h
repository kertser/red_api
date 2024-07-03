//
// Created by kerts on 2/14/2024.
//

#ifndef APICALCULATOR_RZ_163_UHP_H
#define APICALCULATOR_RZ_163_UHP_H

// RED for RZ-163-UHP (new implementation)
double RED_RZ_163_UHP(double Flow, double UVT254, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps);

// VF-RED calculation for RZ-163-UHP
double VF_RED(double RED);

#endif //APICALCULATOR_RZ_163_UHP_H
