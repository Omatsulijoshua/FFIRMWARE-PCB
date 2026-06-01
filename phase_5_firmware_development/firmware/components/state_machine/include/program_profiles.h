#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "sdhs_types.h"

typedef struct {
    sdhs_program_t program;
    float target_temp_c;
    uint8_t fan_percent;
    uint32_t default_duration_seconds;
    uint32_t max_duration_seconds;
    bool heater_enabled;
    bool humidity_auto_stop;
} sdhs_program_profile_t;

const sdhs_program_profile_t *program_profiles_get(sdhs_program_t program);
sdhs_start_command_t program_profiles_make_start(sdhs_program_t program, uint32_t requested_seconds);

