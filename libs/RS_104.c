//
// Created by kerts on 2/14/2024.
// RS-104 RED calculation
//

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"

// RED for RS-104
double RED_RS_104(double Flow, double UVT, double UVT215, double P, double Eff, double D1Log, uint32_t NLamps) {
    // Implementation for RS-104

    bool legacy = false; // legacy RED function

    // General Formula:
    // TUF(Q,UVT,D1Log,ρ)=A*(Q^B)*(D1Log^C)*EXP(D*(UVT/100)))
    // Davg(P,Q,UVT)=A*((P^α)/(Q^β))*EXP(γ*(UVT/100))

    // Dose Coefficients:

    const double Z1 = 0.00013676525437778;
    const double alfa = 1.82647844522826;
    const double beta  = 1;
    const double gama = 8.92498098157656;

    const double A1 = 0.264395667227658;
    const double B1 = 0.0913821393032035;
    const double C1 = 0.0712062582569228;
    const double D1 = 0.777359750748686;

    const double LampPower = 1050; //[W]
    const double minFlow = 10; //m3h
    const double maxFlow = 500; //m3h

    // Logics:
    /*
    P = Power in [%]
    Eff1 = Efficiency in [%]
    Flow in [m^3/h]
    UVT in [%-1cm]
    D1Log in [mJ/cm^2] - 1-Log dose for a specific pathogen
    NLamps = 1 (single lamp only)
    */

    double AvgDose = Z1*((pow((P*Eff/100),alfa))/(pow(Flow,beta)))*exp(gama*(UVT/100));
    double TUF = A1*(pow(Flow,B1))*(pow(D1Log,C1))*exp(D1*(UVT/100));
    if (TUF >1) TUF=1; // TUF<1 always

    return(round_1(AvgDose*TUF));

}
