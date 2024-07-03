//
// Created by kerts on 1/24/2024.
// Helper functions and utils
//

#ifndef APICALCULATOR_UTILS_H
#define APICALCULATOR_UTILS_H

// Helper function to have a round of a value to 1 digit
/**
 *
 * @param value
 * @return round_1
 */
double round_1(double value); // round of a value to 1 digit

// Helper function to have a round of a value to n digits
/**
 *
 * @param value
 * @param n
 * @return round_n
 */
double round_n(double value, int n); // round of a value to n digits

// Helper function max(double a, double b)
/**
 *
 * @param a
 * @param b
 * @return max
 */
double max(double a, double b);

// Helper function min(double a, double b)
/**
 *
 * @param a
 * @param b
 * @return min
 */
double min(double a, double b); // minimum of 2 value

// Helper function avg(int n, ...) to calculate average of arbitrary number of values
/**
 *
 * @param n
 * @param ...
 * @return avg
 */
double avg(int n, ...);
/**
 *
 * @param a
 * @param b
 * @return min
 */
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

#endif //APICALCULATOR_UTILS_H
