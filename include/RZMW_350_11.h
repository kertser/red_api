//
// Created by kerts on 7/8/2024.
//

#ifndef APICALCULATOR_RZMW_350_11_H
#define APICALCULATOR_RZMW_350_11_H

// RED for RZM-200-5
double RED_RZMW_350_11(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps);

// Head Loss function for RZM-200-5
double HeadLoss_RZMW350(double Flow,uint32_t NLamps);

#endif //APICALCULATOR_RZMW_350_1_H
