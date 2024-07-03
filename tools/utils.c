//
// Created by kerts on 1/24/2024.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> // for variable arguments

#include "utils.h"

// Helper function - rounding to one decimal place
double round_1(double value) {
    return round(value * 10) / 10;
}

// Helper function - rounding to precision n
double round_n(double value, int n) {
    return round(value * pow(10, n)) / pow(10, n);
}

// Function to dynamically load the list of supported systems from a file
char** loadSupportedSystems(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    char** supportedSystems = NULL;
    char line[256]; // Adjust size as necessary
    *count = 0;

    while (fgets(line, sizeof(line), file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        // Allocate memory for new entry
        char** temp = realloc(supportedSystems, (*count + 1) * sizeof(char*));
        if (!temp) {
            perror("Failed to allocate memory");
            // Cleanup already allocated memory before failing
            for (int i = 0; i < *count; ++i) {
                free(supportedSystems[i]);
            }
            free(supportedSystems);
            fclose(file);
            return NULL;
        }
        supportedSystems = temp;

        // Allocate memory for the string
        supportedSystems[*count] = malloc(strlen(line) + 1);
        if (!supportedSystems[*count]) {
            perror("Failed to allocate memory for string");
            // Cleanup
            for (int i = 0; i <= *count; ++i) {
                free(supportedSystems[i]);
            }
            free(supportedSystems);
            fclose(file);
            return NULL;
        }

        // Copy the line into the newly allocated memory
        strcpy(supportedSystems[*count], line);
        (*count)++;
    }

    fclose(file);
    return supportedSystems;
}

// Function to return an index of the array
int findIndex(double arr[], int size, double value) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return i;
        }
    }
    return -1;
}

// Helper function for minimum:
double min(double a, double b) {
    return a < b ? a : b;
}

// Function to calculate the average of a variable number of arguments
double avg(int num, ...) {
    /*
     * Example of usage: double average_value = avg(3, 1.0, 2.0, 3.0);
     */

    va_list args;
    double sum = 0.0;

    // Initialize the argument list
    va_start(args, num);

    // Sum all the arguments
    for (int i = 0; i < num; i++) {
        sum += va_arg(args, double);
    }

    // Clean up the argument list
    va_end(args);

    // Calculate the average
    if (num == 0) return 0; // Avoid division by zero
    return sum / num;
}
