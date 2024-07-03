//
// Created by kerts on 7/03/2024.
// RZM-200-5 RED calculation
//

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"

// RED for RZM-200_5 Single Module
double RED_RZM_200_5(double Flow, double UVT, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps) {
    // Implementation for a single module of RZM-200-5

    /*
    General Formula:
    TUF(Q,UVT,D1Log,ρ)=A1*(Q^B1)*(D1Log^C1)*EXP(D1*(UVT/100)))
    Davg(P,Q,UVT)=Z1*((P^α)/(Q^β))*EXP(γ*(UVT/100))
    */

    // Logics
    /*
    P = Power in [%]
    Eff1 = Efficiency in [%]
    Flow in [m^3/h]
    UVT in [%-1cm]
    D1Log in [mJ/cm^2] - 1-Log dose for a specific pathogen
    NLamps = 1 (single lamp)
    */

    bool legacy = false; // legacy RED function

    // pull the lamp power and efficiency values from the input arrays
    double P1 = P[0];
    double P2 = P[1];
    double P3 = P[2];
    double P4 = P[3];
    double P5 = P[4];

    double Eff1 = Eff[0];
    double Eff2 = Eff[1];
    double Eff3 = Eff[2];
    double Eff4 = Eff[3];
    double Eff5 = Eff[4];

    double Dose_VF = 1.3; // Validtion Factor (used here)

    // Dose Coefficients:
    // 2 lamps:
    double Z1 = 0.060339841231047;
    double alfa = 1;
    double beta = 1;
    double gama = 3.37360015072348;

    double A1 = 0.462185705660002;
    double B1 = 0.0749097124175999;
    double C1 = 0.0592308260592836;
    double D1 = 0.0870074638311847;

    double powerDensity = 171.43; // [W/cm]
    double arcLength = 39.8; //[cm]
    double LampPower = round_1(powerDensity*arcLength); // [W] 171.43 W/cm on 39.8cm arc-length

    //This averaging is temporary and shall be changed on a more complicated model
    double P0 = (P1+P2+P3+P4+P5)/5;
    double Eff0 = (Eff1+Eff2+Eff3+Eff4+Eff5)/5;

    double Davg = Dose_VF*Z1*pow(((arcLength*powerDensity*NLamps)*(P0/100)*(Eff0/100)),alfa)/pow(Flow,beta)*exp(gama*(UVT/100));
    double TUF = min(A1*pow(Flow,B1)*pow(D1Log,C1)*exp(D1*(UVT/100)),1); // TUF<1 always

    return round_1(Davg*TUF);

}

double HeadLoss_RZM200(double Flow,uint32_t NLamps) {
    // Calculate HeadLoss[cmH2O]

    double HeadLossFactor = 0.0101971621298; // converting from Pa to cmH2O
    double C_Flow1 = 0.11049;
    double C_Flow2 = 2.03679;

    if (NLamps == 5) // 5L
        return round_n(HeadLossFactor/1000*(pow(C_Flow1*Flow,2)+C_Flow2*Flow), 2);
    else if (NLamps == 3) // 3L - placeholder
        return round_n(HeadLossFactor/1000*(pow(C_Flow1*Flow,2)+C_Flow2*Flow), 2);
    else if (NLamps == 1) // 2L - placeholder
        return round_n(HeadLossFactor/1000*(pow(C_Flow1*Flow, 2)+C_Flow2*Flow), 2);
    else return 0;
}

//
// Created by kerts on 7/3/2024.
//
