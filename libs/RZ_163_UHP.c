//
// Created by kerts on 2/14/2024.
// RZ-163-UHP RED calculation
//

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"

// RED for RZ-163-UHP (new implementation)
double RED_RZ_163_UHP(double Flow, double UVT254, double UVT215, double P, double Eff, double D1Log, uint32_t NLamps) {
    // Implementation for RZ-163-UHP (new implementation)
    // This is an updated version for UHP with fixed UVT254>96[%-1cm]
    // This implementation can take 254nm and 215nm UVT
    //if there is no data about UVT215, -1 value shall be delivered

    /*
        General scheme:

        RED_HL=10^A*(P/100)^(B+C*A254+D*A254^2)*Q^(E+F*A254+G*A254^2)*(1/A254)^(H+I*A254+J*A254^2)*UVS^(K+L*A254+M*A254^2)*NOL^N
        RED_LL=10^A'*(P/100)^(B'+C'*A215+D'*A215^2)*Q^(E'+F'*A215+G'*A215^2)*(1/A215)^(H'+I'*A215+J'*A215^2)*UVS^(K'+L'*A215+M'*A215^2)*NOL^N'
        RED = RED_LL+RED_HL
        abs = -LOG(UVT/100)
        UVS = D1Log average inactivation
        N = number of lamps
        Flow units - [m3/h] - in this calculator converted to us_gpm for calculation
    */

    bool legacy = false; // new implementation

    // General Dose Coefficients
    const double AH = 6.68586909613977;
    const double BH = 0.931505620533178;
    const double CH = -0.762243989524516;
    const double DH = 2.45902603509457;
    const double EH = -0.904980935576177;
    const double FH = 1.3572137958403;
    const double GH = -5.64328172563936;
    const double HH = -0.924600741065049;
    const double IH = -27.9378034190639;
    const double JH = 49.2460500117299;
    const double KH = 0.125320453051158;
    const double LH = 1.30623706193718;
    const double MH = -13.3376614904183;
    const double NH = 1.20023530288472;

    const double AL = 7.3638E+00;
    const double BL = 1.1023E+00;
    const double CL = -1.4006E+01;
    const double DL = 8.4624E+01;
    const double EL = -3.1899E-01;
    const double FL = 0.0000E+00;
    const double GL = -1.1846E+00;
    const double HL = -2.6847E+00;
    const double IL = -4.6157E+01;
    const double JL = 6.6242E+01;
    const double KL = 6.0676E-01;
    const double LL = 4.7538E+00;
    const double ML = -1.9015E+01;
    const double NL = 5.1284E-01;

    const double LampPower = 3000; //[W]
    const double minFlow = 10; //m3h
    const double maxFlow = 3000; //m3h

    // UVT Coefficients for estimated calculation of UVT215 from UVT254:
    const double UVT215_A = 0.2804;
    const double UVT215_B = 0.0609;
    // reinitialize the UVT215 by approximate values if unknown
    if (UVT215 == -1) UVT215 = round_1(UVT215_A * exp(UVT215_B * UVT254));

    double A254 = -log10(UVT254/100);
    double A215 = -log10(UVT215/100);
    Flow = Flow*4.402868; // in this specific case it is a conversion to gpm

    double RED; // just declaration

    if (UVT254 < 96) {
        double RED_HL=pow(10,AH)*pow(((Eff/100)*(P/100)),(BH+CH*A254+DH*pow(A254,2)))*pow(Flow,(EH+FH*A254+GH*pow(A254,2)))*pow((1/A254),(HH+IH*A254+JH*pow(A254,2)))*
                pow(D1Log,(KH+LH*A254+MH*pow(A254,2)))*pow(NLamps,NH);
        double RED_LL=pow(10,AL)*pow(((Eff/100)*(P/100)),(BL+CL*A215+DL*pow(A215,2)))*pow(Flow,(EL+FL*A215+GL*pow(A215,2)))*pow((1/A215),(HL+IL*A215+JL*pow(A215,2)))*
                pow(D1Log,(KL+LL*A215+ML*pow(A215,2)))*pow(NLamps,NL);
        RED = RED_HL+RED_LL;
    }

    else{
        double A254_96 = -log10(96.0/100);
        double A215_96 = -log10(96.0/100);

        double RED_HL96 = pow(10,AH)*pow((Eff/100)*(P/100),(BH+CH*A254_96+DH*pow(A254_96,2)))*pow(Flow,(EH+FH*A254_96+GH*pow(A254_96,2)))*pow((1/A254_96),(HH+IH*A254_96+JH*pow(A254_96,2)))*
                pow(D1Log,(KH+LH*A254_96+MH*pow(A254_96,2)))*pow(NLamps,NH);
        double RED_LL96 = pow(10,AL)*pow((Eff/100)*(P/100),(BL+CL*A215_96+DL*pow(A215_96,2)))*pow(Flow,(EL+FL*A215_96+GL*pow(A215_96,2)))*pow((1/A215_96),(HL+IL*A215_96+JL*pow(A215_96,2)))*
                pow(D1Log,(KL+LL*A215_96+ML*pow(A215_96,2)))*pow(NLamps,NL);

        double RED96 = (RED_HL96+RED_LL96); // might be rounded to 2 digits
        RED = RED96 * pow(1.3,(UVT254-96));
    }

    return round_1(RED);

}

// VF RED for RZ-163-UHP
double VF_RED(double RED){
    // fixed BRED, bias at 3-log, conservative

    const double BRED_fixed = 1.2428; //Fixed BRED for any UVT - for 3-log?
    const double STDEV = 6.2103; // standard deviation for the calculated
    const double tVal = 1.9771; // t-value for the calculated

    double UIN=STDEV*tVal/(RED);

    //VF=(RED)/((1+UIN)*BRED_fixed)
    double VF=((1+UIN)*BRED_fixed);

    return round_1(RED/VF);
}

//VF RED for criptosporidium virus
double VF_RED_cripto(double RED,double UVT, double LI){

    const double STDEV = 6.2103; // standard deviation for the calculated
    const double tVal = 1.9771; // t-value for the calculated

    double log_abc[] = {0.5,1,1.5,2,2.5,3,3.5,4}; // Choose the desired LI
    // required_dose = [1.6,2.5,3.9,5.8,8.5,12,15,22] #Required per LI

    // These are a,b,c constants to calculate the BRED per log-inactivation of target (Criptosporidium)
    double a[] = {-7.956807,-14.617616,-12.844755,-10.18365,-7.038886,-4.375281,-3.261568,-0.060176};
    double b[] = {4.896899,7.185111,5.999709,4.46189,2.957013,1.766574,1.277332,0.12976};
    double c[] = {0.993118,1.031131,1.071125,1.090283,1.083221,1.065181,1.053526,1.019059};
    double A254 = -log(UVT/100);

    int sizeof_a = sizeof(a)/sizeof(a[0]);
    int sizeof_b = sizeof(b)/sizeof(b[0]);
    int sizeof_c = sizeof(c)/sizeof(c[0]);

    double a1 = a[findIndex(a,sizeof_a,LI)];
    double b1 = b[findIndex(b, sizeof_b, LI)];
    double c1 = c[findIndex(c, sizeof_c, LI)];

    double UIN = STDEV * tVal/(RED);
    double BRED = a1*pow(A254,2)+b1*A254+c1;

    double VF=((1+UIN)*BRED);
    return round_1(RED/VF); // Return the credited dose after the validation factor
}