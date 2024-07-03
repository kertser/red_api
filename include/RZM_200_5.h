//
// Created by kerts on 7/3/2024.
//

#ifndef APICALCULATOR_RZM_200_5_H
#define APICALCULATOR_RZM_200_5_H

// RED for RZM-200-5
double RED_RZM_200_5(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps);

// Head Loss function for RZM-200-5
double HeadLoss_RZM200(double Flow,uint32_t NLamps);

#endif //APICALCULATOR_RZM_200_5_H
