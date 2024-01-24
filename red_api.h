// red_api.h

#ifndef RED_API_H
#define RED_API_H

#include <stdint.h>

// Function signature type for RED functions
typedef double (*REDFunction)(double, double, double, double, double, uint32_t);

// List of supported systems
/**
 * returning the list of systems, supported by this API
 * 
 * @return List of UV systems.
 */
const char** ListOfSupportedSystems(size_t* size);

// RED function for RZ-104-11
/**
 * Calculate RED for SystemType1.
 * 
 * @param Flow The flow rate.
 * @param UVT Ultraviolet transmittance.
 * @param P Power.
 * @param Eff Efficiency.
 * @param D1Log 1-Log inactivation.
 * @param NLamps Number of lamps.
 * @return Calculated RED value.
 */
double RED_RZ_104_11(double Flow, double UVT, double P, double Eff, double D1Log, uint32_t NLamps);

// RED function for SystemType2
/**
 * Calculate RED for SystemType2.
 * 
 * @param Flow The flow rate.
 * @param UVT Ultraviolet transmittance.
 * @param P Power.
 * @param Eff Efficiency.
 * @param D1Log 1-Log inactivation.
 * @param NLamps Number of lamps.
 * @return Calculated RED value.
 */
double RED_RZ_300_HDR(double Flow, double UVT, double P, double Eff, double D1Log, uint32_t NLamps);

// Function to select the appropriate RED function
/**
 * Selects the appropriate RED calculation function based on system type.
 * 
 * @param systemType The system type identifier.
 * @return Function pointer to the selected RED function.
 */
REDFunction getREDFunction(char *systemType);

// Function to select the appropriate RED function
/**
 * Selects the appropriate RED calculation function based on system type.
 *
 * @param systemType The system type identifier.
 * @return Function pointer to the selected RED function.
 */

#endif // RED_API_H
