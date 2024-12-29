// red_api.c

#include "red_api.h"
#include "systems_header.h"
#include "system_config.h"
#include <string.h>
#include <stdlib.h>

// Function to return the entire list of systems
const char** ListOfSupportedSystems(size_t* size) {
    return get_supported_systems(size);
}

// System RED function selector
REDFunction getREDFunction(char *systemType) {
    // Verify that the system exists in our configuration
    system_config_t config;
    if (!get_system_config(systemType, &config)) {
        return NULL;
    }

    // Map system types to their RED functions
    if (strcmp(systemType, "RZ-104-11") == 0) return RED_RZ_104_11;
    else if (strcmp(systemType, "RZ-104-12") == 0) return RED_RZ_104_12;
    else if (strcmp(systemType, "RZ-163-11") == 0) return RED_RZ_163_1L;
    else if (strcmp(systemType, "RZ-163-12") == 0) return RED_RZ_163_ML;
    else if (strcmp(systemType, "RZ-163-13") == 0) return RED_RZ_163_ML;
    else if (strcmp(systemType, "RZ-163-14") == 0) return RED_RZ_163_ML;
    else if (strcmp(systemType, "RZ-163HP-11") == 0) return RED_RZ_163HP_1L;
    else if (strcmp(systemType, "RZ-163HP-12") == 0) return RED_RZ_163HP_ML;
    else if (strcmp(systemType, "RZ-163HP-13") == 0) return RED_RZ_163HP_ML;
    else if (strcmp(systemType, "RZ-163HP-14") == 0) return RED_RZ_163HP_ML;
    else if (strcmp(systemType, "RZ-163UHP-11") == 0) return RED_RZ_163_UHP;
    else if (strcmp(systemType, "RZ-163UHP-12") == 0) return RED_RZ_163_UHP;
    else if (strcmp(systemType, "RZ-163UHP-13") == 0) return RED_RZ_163_UHP;
    else if (strcmp(systemType, "RZ-163UHP-14") == 0) return RED_RZ_163_UHP;
    else if (strcmp(systemType, "RZ-300-HDR") == 0) return RED_RZ_300_HDR;
    else if (strcmp(systemType, "RS-104") == 0) return RED_RS_104;
    else if (strcmp(systemType, "RZB-300") == 0) return RED_RZB_300;
    else if (strcmp(systemType, "RZM-350-8") == 0) return RED_RZM_350_8;
    else if (strcmp(systemType, "RZM-350-5") == 0) return RED_RZM_350_8; // Same as 8 lamps, but 5
    else if (strcmp(systemType, "RZM-200-5") == 0) return RED_RZM_200_5;
    else if (strcmp(systemType, "RZM-200-3") == 0) return RED_RZM_200_5; // Same as 5 lamps, but 3
    else if (strcmp(systemType, "RZM-200-2") == 0) return RED_RZM_200_5; // Same as 5 lamps, but 2
    else if (strcmp(systemType, "RZMW-350-11") == 0) return RED_RZMW_350_11;
    else if (strcmp(systemType, "RZMW-350-7") == 0) return RED_RZMW_350_11; // Same as 11 lamps, but 7

    return NULL;
}

// Returns a standard number of lamps per system
uint32_t getNLamps(char *systemType) {
    // Get lamp count from JSON configuration
    return get_lamp_count(systemType);
}