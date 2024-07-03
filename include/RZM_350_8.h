//
// Created by kerts on 7/3/2024.
//

#ifndef APICALCULATOR_RZM_350_8_H
#define APICALCULATOR_RZM_350_8_H

// RED for RZM-350-8 single module (8 lamps)
double RED_RZM_350_8(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps);

// Head Loss function for RZM-350-8
double HeadLoss(double Flow,uint32_t NLamps);

#endif //APICALCULATOR_RZM_350_8_H
