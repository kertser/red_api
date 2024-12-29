#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t lamp_count;
    double lamp_power;
    double min_flow;
    double max_flow;
    double min_uvt;
    double max_uvt;
} system_config_t;

bool init_system_config(const char* json_path);
void cleanup_system_config(void);
const char** get_supported_systems(size_t* count);
bool get_system_config(const char* system_type, system_config_t* config);
uint32_t get_lamp_count(const char* system_type);
double get_lamp_power(const char* system_type);
bool validate_parameters(const char* system_type, double flow, double uvt,
                         double power, double efficiency);

#endif // SYSTEM_CONFIG_H