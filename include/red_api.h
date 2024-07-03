// red_api.h

#ifndef RED_API_H
#define RED_API_H

#include <stdint.h> // for uint32_t

// Function signature type for RED functions
// typical for (double Flow, double UVT254, double UVT215, double P[], double Eff[], double D1Log, uint32_t NLamps);
typedef double (*REDFunction)(double, double, double, double[], double[], double, uint32_t);

// List of supported systems
/**
 * returning the list of systems, supported by this API
 * 
 * @return List of UV systems.
 */
const char** ListOfSupportedSystems(size_t* size);

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
