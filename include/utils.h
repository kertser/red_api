//
// Created by kerts on 1/24/2024.
// Helper functions and utils
//

#ifndef APICALCULATOR_UTILS_H
#define APICALCULATOR_UTILS_H

double round_1(double value); // round of a value to 1 digit
double round_n(double value, int n); // round of a value to n digits
double max(double a, double b);
double min(double a, double b); // minimum of 2 value
double avg(int n, ...); // average of arbitrary number of values (n indicates this number)

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

#endif //APICALCULATOR_UTILS_H
