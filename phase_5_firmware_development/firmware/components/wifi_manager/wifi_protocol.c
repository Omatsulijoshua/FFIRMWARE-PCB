#include "wifi_protocol.h"

#include <string.h>

typedef struct {
    const char *name;
    sdhs_program_t program;
} program_name_t;

static const program_name_t s_program_names[] = {
    { "SHOES", SDHS_PROGRAM_SHOES },
    { "HELMET", SDHS_PROGRAM_HELMET },
    { "GLOVES", SDHS_PROGRAM_GLOVES },
    { "SPORTS_GEAR", SDHS_PROGRAM_SPORTS_GEAR },
    { "TECHNICAL_GEAR", SDHS_PROGRAM_TECHNICAL_GEAR },
    { "CLOTHING", SDHS_PROGRAM_CLOTHING },
    { "HYGIENE", SDHS_PROGRAM_HYGIENE },
    { "FAN_ONLY", SDHS_PROGRAM_FAN_ONLY },
};

sdhs_program_t wifi_protocol_program_from_string(const char *program)
{
    if (program == NULL) {
        return SDHS_PROGRAM_NONE;
    }

    for (size_t i = 0; i < sizeof(s_program_names) / sizeof(s_program_names[0]); ++i) {
        if (strcmp(program, s_program_names[i].name) == 0) {
            return s_program_names[i].program;
        }
    }

    return SDHS_PROGRAM_NONE;
}

const char *wifi_protocol_state_to_string(sdhs_wifi_state_t state)
{
    switch (state) {
    case SDHS_WIFI_STATE_UNPROVISIONED: return "UNPROVISIONED";
    case SDHS_WIFI_STATE_PROVISIONED_DISCONNECTED: return "PROVISIONED_DISCONNECTED";
    case SDHS_WIFI_STATE_CONNECTING: return "CONNECTING";
    case SDHS_WIFI_STATE_CONNECTED: return "CONNECTED";
    case SDHS_WIFI_STATE_RECOVERY: return "RECOVERY";
    default: return "UNKNOWN";
    }
}

