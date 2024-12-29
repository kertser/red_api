#include "system_config.h"
#include <json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct json_object* systems_json = NULL;
static char** supported_systems = NULL;
static size_t systems_count = 0;

bool init_system_config(const char* json_path) {
    // Read and parse JSON file
    systems_json = json_object_from_file(json_path);
    if (!systems_json) {
        fprintf(stderr, "Failed to parse JSON file: %s\n", json_path);
        return false;
    }

    // Get the supported_systems object
    struct json_object* systems_obj;
    if (!json_object_object_get_ex(systems_json, "supported_systems", &systems_obj)) {
        fprintf(stderr, "No 'supported_systems' key in JSON\n");
        json_object_put(systems_json);
        return false;
    }

    // Count systems and allocate memory
    systems_count = json_object_object_length(systems_obj);
    supported_systems = malloc(systems_count * sizeof(char*));
    if (!supported_systems) {
        json_object_put(systems_json);
        return false;
    }

    // Store system names
    size_t index = 0;
    json_object_object_foreach(systems_obj, key, val) {
        supported_systems[index] = strdup(key);
        index++;
    }

    return true;
}

void cleanup_system_config(void) {
    if (systems_json) {
        json_object_put(systems_json);
        systems_json = NULL;
    }

    if (supported_systems) {
        for (size_t i = 0; i < systems_count; i++) {
            free(supported_systems[i]);
        }
        free(supported_systems);
        supported_systems = NULL;
    }
    systems_count = 0;
}

const char** get_supported_systems(size_t* count) {
    if (count) {
        *count = systems_count;
    }
    return (const char**)supported_systems;
}

bool get_system_config(const char* system_type, system_config_t* config) {
    if (!systems_json || !config || !system_type) {
        return false;
    }

    // Initialize config with default values
    memset(config, 0, sizeof(system_config_t));

    struct json_object* supported_systems_obj;
    if (!json_object_object_get_ex(systems_json, "supported_systems", &supported_systems_obj)) {
        return false;
    }

    // Get specific system configuration
    struct json_object* system_obj;
    if (!json_object_object_get_ex(supported_systems_obj, system_type, &system_obj)) {
        return false;
    }

    // Get lamp configuration
    struct json_object* lamp_obj;
    if (json_object_object_get_ex(system_obj, "lamp", &lamp_obj)) {
        struct json_object* temp;

        if (json_object_object_get_ex(lamp_obj, "count", &temp)) {
            // Handle the case where count might be "variable"
            if (json_object_is_type(temp, json_type_string)) {
                config->lamp_count = 0; // or some default value for variable
            } else {
                config->lamp_count = json_object_get_int(temp);
            }
        }

        if (json_object_object_get_ex(lamp_obj, "power", &temp)) {
            if (!json_object_is_type(temp, json_type_null)) {
                config->lamp_power = json_object_get_double(temp);
            }
        }
    }

    // Get operational limits if they exist
    struct json_object* op_limits_obj;
    if (json_object_object_get_ex(system_obj, "operational_limits", &op_limits_obj)) {
        // Get flow limits
        struct json_object* flow_obj;
        if (json_object_object_get_ex(op_limits_obj, "flow", &flow_obj)) {
            struct json_object* temp;
            if (json_object_object_get_ex(flow_obj, "min", &temp)) {
                config->min_flow = json_object_get_double(temp);
            }
            if (json_object_object_get_ex(flow_obj, "max", &temp)) {
                config->max_flow = json_object_get_double(temp);
            }
        }

        // Get UVT limits
        struct json_object* uvt_obj;
        if (json_object_object_get_ex(op_limits_obj, "uvt", &uvt_obj)) {
            struct json_object* temp;
            if (json_object_object_get_ex(uvt_obj, "min", &temp)) {
                config->min_uvt = json_object_get_double(temp);
            }
            if (json_object_object_get_ex(uvt_obj, "max", &temp)) {
                config->max_uvt = json_object_get_double(temp);
            }
        } else {
            // Default UVT limits if not specified
            config->min_uvt = 75.0;
            config->max_uvt = 98.0;
        }

        // Get drive limits
        struct json_object* drive_obj;
        if (json_object_object_get_ex(op_limits_obj, "drive", &drive_obj)) {
            struct json_object* temp;
            if (json_object_object_get_ex(drive_obj, "min", &temp)) {
                config->min_drive = json_object_get_double(temp);
            }
            if (json_object_object_get_ex(drive_obj, "max", &temp)) {
                config->max_drive = json_object_get_double(temp);
            }
        }

        // Get efficiency limits
        struct json_object* efficiency_obj;
        if (json_object_object_get_ex(op_limits_obj, "efficiency", &efficiency_obj)) {
            struct json_object* temp;
            if (json_object_object_get_ex(efficiency_obj, "min", &temp)) {
                config->min_efficiency = json_object_get_double(temp);
            }
            if (json_object_object_get_ex(efficiency_obj, "max", &temp)) {
                config->max_efficiency = json_object_get_double(temp);
            }
        }
    }

    return true;
}

uint32_t get_lamp_count(const char* system_type) {
    system_config_t config;
    if (get_system_config(system_type, &config)) {
        return config.lamp_count;
    }
    return 0;
}

double get_lamp_power(const char* system_type) {
    system_config_t config;
    if (get_system_config(system_type, &config)) {
        return config.lamp_power;
    }
    return 0.0;
}

bool validate_parameters(const char* system_type, double flow, double uvt,
                         double power, double efficiency) {
    system_config_t config;
    if (!get_system_config(system_type, &config)) {
        return false;
    }

    // Check if flow limits exist and validate
    if (config.min_flow > 0 && config.max_flow > 0) {
        if (flow < config.min_flow || flow > config.max_flow) {
            return false;
        }
    }

    // Check if UVT limits exist and validate
    if (config.min_uvt > 0 && config.max_uvt > 0) {
        if (uvt < config.min_uvt || uvt > config.max_uvt) {
            return false;
        }
    }

    // Check drive limits if they exist
    if (config.min_drive > 0 && config.max_drive > 0) {
        if (power < config.min_drive || power > config.max_drive) {
            return false;
        }
    } else {
        // Standard power validation if no specific limits
        if (power < 0.0 || power > 100.0) {
            return false;
        }
    }

    // Check efficiency limits if they exist
    if (config.min_efficiency > 0 && config.max_efficiency > 0) {
        if (efficiency < config.min_efficiency || efficiency > config.max_efficiency) {
            return false;
        }
    } else {
        // Standard efficiency validation if no specific limits
        if (efficiency < 0.0 || efficiency > 100.0) {
            return false;
        }
    }

    return true;
}