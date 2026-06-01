#include "sdhs_types.h"

const char *sdhs_state_to_string(sdhs_state_t state)
{
    switch (state) {
    case SDHS_STATE_STARTUP: return "STARTUP";
    case SDHS_STATE_IDLE: return "IDLE";
    case SDHS_STATE_PREHEAT: return "PREHEAT";
    case SDHS_STATE_DRYING: return "DRYING";
    case SDHS_STATE_HYGIENE: return "HYGIENE";
    case SDHS_STATE_COOLDOWN: return "COOLDOWN";
    case SDHS_STATE_COMPLETE: return "COMPLETE";
    case SDHS_STATE_FAULT: return "FAULT";
    default: return "UNKNOWN";
    }
}

const char *sdhs_program_to_string(sdhs_program_t program)
{
    switch (program) {
    case SDHS_PROGRAM_NONE: return "NONE";
    case SDHS_PROGRAM_SHOES: return "SHOES";
    case SDHS_PROGRAM_HELMET: return "HELMET";
    case SDHS_PROGRAM_GLOVES: return "GLOVES";
    case SDHS_PROGRAM_SPORTS_GEAR: return "SPORTS_GEAR";
    case SDHS_PROGRAM_TECHNICAL_GEAR: return "TECHNICAL_GEAR";
    case SDHS_PROGRAM_CLOTHING: return "CLOTHING";
    case SDHS_PROGRAM_HYGIENE: return "HYGIENE";
    case SDHS_PROGRAM_FAN_ONLY: return "FAN_ONLY";
    default: return "UNKNOWN";
    }
}

const char *sdhs_fault_to_string(sdhs_fault_code_t fault)
{
    switch (fault) {
    case SDHS_FAULT_NONE: return "NONE";
    case SDHS_FAULT_SENSOR_STALE: return "SENSOR_STALE";
    case SDHS_FAULT_TEMP_SENSOR_INVALID: return "TEMP_SENSOR_INVALID";
    case SDHS_FAULT_HUMIDITY_SENSOR_INVALID: return "HUMIDITY_SENSOR_INVALID";
    case SDHS_FAULT_OVERTEMP: return "OVERTEMP";
    case SDHS_FAULT_FAN_FAILURE: return "FAN_FAILURE";
    case SDHS_FAULT_WATCHDOG_RESET: return "WATCHDOG_RESET";
    case SDHS_FAULT_STORAGE: return "STORAGE";
    case SDHS_FAULT_OTA: return "OTA";
    default: return "UNKNOWN";
    }
}

const char *sdhs_completion_to_string(sdhs_completion_reason_t reason)
{
    switch (reason) {
    case SDHS_COMPLETE_NONE: return "NONE";
    case SDHS_COMPLETE_TIMER: return "TIMER";
    case SDHS_COMPLETE_HUMIDITY_DRY: return "HUMIDITY_DRY";
    case SDHS_COMPLETE_USER_STOP: return "USER_STOP";
    case SDHS_COMPLETE_FAULT: return "FAULT";
    case SDHS_COMPLETE_COOLDOWN_DONE: return "COOLDOWN_DONE";
    default: return "UNKNOWN";
    }
}

