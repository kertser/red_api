#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>  // Added for size_t

// Structure to hold system configuration
typedef struct {
    uint32_t lamp_count;
    double lamp_power;
    // Flow limits
    double min_flow;
    double vertical_min_flow;
    double max_flow;
    // UVT limits
    double min_uvt;
    double max_uvt;
    // Drive limits
    double min_drive;
    double max_drive;
    // Efficiency limits
    double min_efficiency;
    double max_efficiency;
} system_config_t;

// Function declarations
bool init_system_config(void);
void cleanup_system_config(void);
const char** get_supported_systems(size_t* count);
bool get_system_config(const char* system_type, system_config_t* config);
uint32_t get_lamp_count(const char* system_type);
double get_lamp_power(const char* system_type);
bool validate_parameters(const char* system_type, double flow, double uvt,
                        double power, double efficiency, bool is_vertical);

#endif // SYSTEM_CONFIG_H