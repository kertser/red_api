//
// Created by kerts on 1/24/2024.
// Helper functions and utils
//

#ifndef APICALCULATOR_UTILS_H
#define APICALCULATOR_UTILS_H

// Helper function for math
/**
 * Calculate floating value rounded to 1.
 *
 * @param value Input
 * @return rounded value.
 */
double round_1(double value);

// Helper function to dynamically load the list of supported systems
/**
 *
 * @param filename
 * @param count
 * @return supportedSystems
 */
char** loadSupportedSystems(const char* filename, int* count);

// Helper function to find index of array element
/**
 *
 * @param arr
 * @param size
 * @param value
 * @return index
 */
int findIndex(double arr[], int size, double value);

// Helper function min(double a, double b)
/**
 *
 * @param a
 * @param b
 * @return min
 */
double min(double a, double b);

#endif //APICALCULATOR_UTILS_H
