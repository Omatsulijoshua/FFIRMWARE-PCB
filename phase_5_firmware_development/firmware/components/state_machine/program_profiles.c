#include "program_profiles.h"

#include "app_config.h"

#include <stddef.h>

#define MINUTES(x) ((x) * 60U)

static const sdhs_program_profile_t s_profiles[] = {
    { SDHS_PROGRAM_SHOES, 50.0f, 80, MINUTES(120), MINUTES(240), true, true },
    { SDHS_PROGRAM_HELMET, 42.0f, 60, MINUTES(60), MINUTES(120), true, true },
    { SDHS_PROGRAM_GLOVES, 45.0f, 70, MINUTES(90), MINUTES(180), true, true },
    { SDHS_PROGRAM_SPORTS_GEAR, 55.0f, 90, MINUTES(180), MINUTES(360), true, true },
    { SDHS_PROGRAM_TECHNICAL_GEAR, 38.0f, 50, MINUTES(90), MINUTES(240), true, true },
    { SDHS_PROGRAM_CLOTHING, 45.0f, 70, MINUTES(120), MINUTES(300), true, true },
    { SDHS_PROGRAM_HYGIENE, 62.0f, 80, MINUTES(45), MINUTES(90), true, false },
    { SDHS_PROGRAM_FAN_ONLY, 0.0f, 70, MINUTES(60), MINUTES(480), false, false },
};

const sdhs_program_profile_t *program_profiles_get(sdhs_program_t program)
{
    for (size_t i = 0; i < sizeof(s_profiles) / sizeof(s_profiles[0]); ++i) {
        if (s_profiles[i].program == program) {
            return &s_profiles[i];
        }
    }
    return NULL;
}

sdhs_start_command_t program_profiles_make_start(sdhs_program_t program, uint32_t requested_seconds)
{
    const sdhs_program_profile_t *profile = program_profiles_get(program);
    sdhs_start_command_t cmd = {
        .program = SDHS_PROGRAM_NONE,
    };

    if (profile == NULL) {
        return cmd;
    }

    uint32_t duration = requested_seconds == 0 ?
        profile->default_duration_seconds : requested_seconds;
    if (duration > profile->max_duration_seconds) {
        duration = profile->max_duration_seconds;
    }
    if (duration > SDHS_DEFAULT_MAX_CYCLE_SECONDS) {
        duration = SDHS_DEFAULT_MAX_CYCLE_SECONDS;
    }

    cmd.program = profile->program;
    cmd.duration_seconds = duration;
    cmd.fan_percent = profile->fan_percent;
    cmd.target_temp_c = profile->target_temp_c;
    cmd.heater_enabled = profile->heater_enabled;
    cmd.humidity_auto_stop = profile->humidity_auto_stop;
    return cmd;
}
