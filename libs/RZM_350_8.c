//
// Created by kerts on 7/3/2024.
// RZM-350-8 RED calculation
//

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"

/*
General Formula:
Davg(P,Q,UVT)=A*((P^α)/(Q^β))*EXP(γ*(UVT/100))
TUF(Q,UVT,D1Log,ρ)=A*(Q^B)*(D1Log^C)*EXP(D*(UVT/100)))
Schematic lamp placement:

 """
2		8
    1
3		7
    5
4		6
"""
*/

// RED for RZM-350-8 Single Module
double RED_RZM_350_8(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps) {
    // Implementation for a single module of RZM-350-8 (Regular System)
    // Flow in [m3/hour]
    // UVT in [%-1cm]
    // UVT215 in [%-1cm]
    // P in [kW]
    // Eff in [%]
    // D1Log in [mJ/cm2]
    // NLamps in # of inits

    bool legacy = false; // legacy RED function

    // pull the lamp power and efficiency values from the input arrays
    double P1 = P[0];
    double P2 = P[1];
    double P3 = P[2];
    double P4 = P[3];
    double P5 = P[4];
    double P6 = P[5];
    double P7 = P[6];
    double P8 = P[7];

    double Dose_VF = 1.7; // regular systems Validtion Factor (used here)
    double Dose_VF_marine = 1.3; // marine systems Validation Factor


    // May be redundant due to RED model file usage
    if (NLamps == 5){
        P6=0;
        P7=0;
        P8=0;
    }

    double Eff1 = Eff[0];
    double Eff2 = Eff[1];
    double Eff3 = Eff[2];
    double Eff4 = Eff[3];
    double Eff5 = Eff[4];
    double Eff6 = Eff[5];
    double Eff7 = Eff[6];
    double Eff8 = Eff[7];


    // C-flow [bar/(m^3/hour)^2]
    double C_Flow1_11 = 0.01318957;
    double C_Flow2_11 = 0.90015788;
    double C_Flow1_12 = 0.01318957;
    double C_Flow2_12 = 0.90015788;

    // Dose Coefficients
    // RZM-11: - Base
    double LampPower = 12000; //[W]
    double minFlow = 50; //m3h
    double maxFlow = 1000; //m3h

    double Z_base = 0.610173974032547;
    double alfa_base = 1;
    double beta_base = 1;
    double gama_base = 2.80917232123146;

    double A_base = 0.388730011750849;
    double B_base = 0.0553499298263537;
    double C_base = 0.0522199065009149;
    double D_base = 0.505744414519666;

    // L1:
    double Z_1L = 0.0460004215693581;
    double alfa_1L = 1;
    double beta_1L = 1;
    double gama_1L = 3.47822521230922;

    double A1L = 0.0029330354013571;
    double B1L = 0.559110762585846;
    double C1L = 0.551485708959622;
    double D1L = 0.191231787550938;

    // L2:
    double Z_2L = 0.0460004215693581;
    double alfa_2L = 1;
    double beta_2L = 1;
    double gama_2L = 3.47822521230922;

    double A2L = 0.00140897975230076;
    double B2L = 0.61078312588621;
    double C2L = 0.635391483015667;
    double D2L = 0;

    // L3:
    double Z_3L = 0.0527635268600559;
    double alfa_3L = 1;
    double beta_3L = 1;
    double gama_3L = 3.92565591029147;

    double A3L = 0.00769435233290522;
    double B3L = 0.404441425629256;
    double C3L = 0.334340940578571;
    double D3L = 1.40297958816377;

    // L4: - like L2 (symmetry)
    double Z_4L = Z_2L;
    double alfa_4L = alfa_2L;
    double beta_4L = beta_2L;
    double gama_4L = gama_2L;

    double A4L = A2L;
    double B4L = B2L;
    double C4L = C2L;
    double D4L = D2L;

    // L5: - like L1 (symmetry)
    double Z_5L = Z_1L;
    double alfa_5L = alfa_1L;
    double beta_5L = beta_1L;
    double gama_5L = gama_1L;

    double A5L = A1L;
    double B5L = B1L;
    double C5L = C1L;
    double D5L = D1L;

    // L7:
    double Z_7L = 0.0509388164423499;
    double alfa_7L = 1;
    double beta_7L = 1;
    double gama_7L = 3.61098810710419;

    double A7L = 0.027549827752193;
    double B7L = 0.233436748808719;
    double C7L = 0.272108471414808;
    double D7L = 1.36580422402348;

    // L8:
    double Z_8L = 0.0537133653112592;
    double alfa_8L = 1;
    double beta_8L = 1;
    double gama_8L = 3.04568395486429;

    double A8L = 0.00171727284713336;
    double B8L = 0.642189398962404;
    double C8L = 0.591566437213566;
    double D8L = 0;

    // L6: - like L8 (symmetry)
    double Z_6L = Z_8L;
    double alfa_6L = alfa_8L;
    double beta_6L = beta_8L;
    double gama_6L = gama_8L;

    double A6L = A8L;
    double B6L = B8L;
    double C6L = C8L;
    double D6L = D8L;



    double Davg_base = Z_base*(pow(((avg(8,P1*Eff1,P2*Eff2,P3*Eff3,P4*Eff4,P5*Eff5,P6*Eff6,P7*Eff7,P8*Eff8)/100)/100*LampPower),alfa_base)/(pow(Flow,beta_base)))*exp(gama_base*UVT/100);
    double TUF_base = A_base*(pow(Flow,B_base))*(pow(D1Log,C_base))*exp(D_base*(UVT/100));
    if (TUF_base > 1) TUF_base = 1; // Prevent the situation, where base Track Uniformity is larger than 1

    double Davg1L = Z_1L*(pow((((P1*Eff1-avg(8,P1*Eff1,P2*Eff2,P3*Eff3,P4*Eff4,P5*Eff5,P6*Eff6,P7*Eff7,P8*Eff8))/100)/100*LampPower),alfa_1L)/(pow(Flow,beta_1L)))*exp(gama_1L*(UVT/100));
    double TUF1L = A1L*(pow(Flow,B1L))*(pow(D1Log,C1L))*exp(D1L*(UVT/100));
    if (TUF1L < 0) TUF1L = 0;

    double Davg2L = Z_2L*(pow((((P2*Eff2-avg(8,P1*Eff1,P2*Eff2,P3*Eff3,P4*Eff4,P5*Eff5,P6*Eff6,P7*Eff7,P8*Eff8))/100)/100*LampPower),alfa_2L)/(pow(Flow,beta_2L)))*exp(gama_2L*(UVT/100));
    double TUF2L = A2L*(pow(Flow,B2L))*(pow(D1Log,C2L))*exp(D2L*(UVT/100));
    if (TUF2L < 0) TUF1L = 0;

    double Davg3L = Z_3L*(pow((((P3*Eff3-avg(8,P1*Eff1,P2*Eff2,P3*Eff3,P4*Eff4,P5*Eff5,P6*Eff6,P7*Eff7,P8*Eff8))/100)/100*LampPower),alfa_3L)/(pow(Flow,beta_3L)))*exp(gama_3L*(UVT/100));
    double TUF3L = A3L*(pow(Flow,B3L))*(pow(D1Log,C3L))*exp(D3L*(UVT/100));
    if (TUF3L < 0) TUF3L = 0;

    double Davg4L = Z_4L*(pow((((P4*Eff4-avg(8,P1*Eff1,P2*Eff2,P3*Eff3,P4*Eff4,P5*Eff5,P6*Eff6,P7*Eff7,P8*Eff8))/100)/100*LampPower),alfa_4L)/(pow(Flow,beta_4L)))*exp(gama_4L*(UVT/100));
    double TUF4L = A4L*(pow(Flow,B4L))*(pow(D1Log,C4L))*exp(D4L*(UVT/100));
    if (TUF4L < 0) TUF4L = 0;

    double Davg5L = Z_5L*(pow((((P5*Eff5-avg(8,P1*Eff1,P2*Eff2,P3*Eff3,P4*Eff4,P5*Eff5,P6*Eff6,P7*Eff7,P8*Eff8))/100)/100*LampPower),alfa_5L)/(pow(Flow,beta_5L)))*exp(gama_5L*(UVT/100));
    double TUF5L = A5L*(pow(Flow,B5L))*(pow(D1Log,C5L))*exp(D5L*(UVT/100));
    if (TUF5L < 0) TUF5L = 0;

    double Davg6L = Z_6L*(pow((((P6*Eff6-avg(8,P1*Eff1,P2*Eff2,P3*Eff3,P4*Eff4,P5*Eff5,P6*Eff6,P7*Eff7,P8*Eff8))/100)/100*LampPower),alfa_6L)/(pow(Flow,beta_6L)))*exp(gama_6L*(UVT/100));
    double TUF6L = A6L*(pow(Flow,B6L))*(pow(D1Log,C6L))*exp(D6L*(UVT/100));
    if (TUF6L < 0) TUF6L = 0;

    double Davg7L = Z_7L*(pow((((P7*Eff7-avg(8,P1*Eff1,P2*Eff2,P3*Eff3,P4*Eff4,P5*Eff5,P6*Eff6,P7*Eff7,P8*Eff8))/100)/100*LampPower),alfa_7L)/(pow(Flow,beta_7L)))*exp(gama_7L*(UVT/100));
    double TUF7L = A7L*(pow(Flow,B7L))*(pow(D1Log,C7L))*exp(D7L*(UVT/100));
    if (TUF7L < 0) TUF7L = 0;

    double Davg8L = Z_8L*(pow((((P8*Eff8-avg(8,P1*Eff1,P2*Eff2,P3*Eff3,P4*Eff4,P5*Eff5,P6*Eff6,P7*Eff7,P8*Eff8))/100)/100*LampPower),alfa_8L)/(pow(Flow,beta_8L)))*exp(gama_8L*(UVT/100));
    double TUF8L = A8L*(pow(Flow,B8L))*(pow(D1Log,C8L))*exp(D8L*(UVT/100));
    if (TUF8L < 0) TUF8L = 0;

    double RED_Base = Davg_base*TUF_base;
    double RED = RED_Base + (Davg1L*TUF1L + Davg2L*TUF2L + Davg3L*TUF3L + Davg4L*TUF4L + Davg5L*TUF5L+ Davg6L*TUF6L + Davg7L*TUF7L + Davg8L*TUF8L);

    // alternative code:
    RED = Davg_base*TUF_base;

    // Pay attention that the Regular System factor is used in RED calculation. Marine System VF is different.
    if (NLamps == 8)
        return round_1(RED * Dose_VF);
    else if (NLamps == 16) // This is a simplification, but should be valid in this stage
        return round_1(2*RED * Dose_VF);
    else return 0;
}

double HeadLoss(double Flow,uint32_t NLamps) {
    // Calculate HeadLoss[cmH2O] = HeadLossFactor*C_Flow*Flow^2

    // C-flow [bar/(m^3/hour)^2]
    double C_Flow1_11 = 0.01318957;
    double C_Flow2_11 = 0.90015788;
    double C_Flow1_12 = 0.01318957;
    double C_Flow2_12 = 0.90015788;

    double HeadLossFactor = 0.01; // To match the old calculator
    if (NLamps == 8) return round_n(HeadLossFactor*(C_Flow1_11*pow(Flow,2)+C_Flow2_11*Flow)/100,2);
    else if (NLamps == 16) return round_n(HeadLossFactor*2*(C_Flow1_12*pow(Flow,2)+C_Flow2_12*Flow)/100,2);
    else return 0;
}