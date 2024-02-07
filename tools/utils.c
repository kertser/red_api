//
// Created by kerts on 1/24/2024.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function - rounding to one decimal place
double round_1(double value) {
    return round(value * 10) / 10;
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