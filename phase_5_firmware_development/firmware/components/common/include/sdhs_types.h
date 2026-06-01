#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    SDHS_STATE_STARTUP = 0,
    SDHS_STATE_IDLE,
    SDHS_STATE_PREHEAT,
    SDHS_STATE_DRYING,
    SDHS_STATE_HYGIENE,
    SDHS_STATE_COOLDOWN,
    SDHS_STATE_COMPLETE,
    SDHS_STATE_FAULT,
} sdhs_state_t;

typedef enum {
    SDHS_PROGRAM_NONE = 0,
    SDHS_PROGRAM_SHOES,
    SDHS_PROGRAM_HELMET,
    SDHS_PROGRAM_GLOVES,
    SDHS_PROGRAM_SPORTS_GEAR,
    SDHS_PROGRAM_TECHNICAL_GEAR,
    SDHS_PROGRAM_CLOTHING,
    SDHS_PROGRAM_HYGIENE,
    SDHS_PROGRAM_FAN_ONLY,
} sdhs_program_t;

typedef enum {
    SDHS_FAULT_NONE = 0,
    SDHS_FAULT_SENSOR_STALE,
    SDHS_FAULT_TEMP_SENSOR_INVALID,
    SDHS_FAULT_HUMIDITY_SENSOR_INVALID,
    SDHS_FAULT_OVERTEMP,
    SDHS_FAULT_FAN_FAILURE,
    SDHS_FAULT_WATCHDOG_RESET,
    SDHS_FAULT_STORAGE,
    SDHS_FAULT_OTA,
} sdhs_fault_code_t;

typedef enum {
    SDHS_COMPLETE_NONE = 0,
    SDHS_COMPLETE_TIMER,
    SDHS_COMPLETE_HUMIDITY_DRY,
    SDHS_COMPLETE_USER_STOP,
    SDHS_COMPLETE_FAULT,
    SDHS_COMPLETE_COOLDOWN_DONE,
} sdhs_completion_reason_t;

typedef enum {
    SDHS_FAULT_CLASS_ADVISORY = 0,
    SDHS_FAULT_CLASS_RECOVERABLE,
    SDHS_FAULT_CLASS_LATCHED,
    SDHS_FAULT_CLASS_SERVICE_REQUIRED,
} sdhs_fault_class_t;

typedef struct {
    float outlet_temp_c;
    float humidity_rh;
    float heater_zone_temp_c;
    uint32_t timestamp_ms;
    bool valid;
} sdhs_sensor_sample_t;

typedef struct {
    sdhs_state_t state;
    sdhs_program_t program;
    sdhs_fault_code_t fault;
    uint32_t elapsed_seconds;
    uint32_t remaining_seconds;
    float outlet_temp_c;
    float humidity_rh;
    uint8_t fan_percent;
    bool heater_on;
    sdhs_completion_reason_t completion_reason;
} sdhs_status_t;

typedef struct {
    sdhs_program_t program;
    uint32_t duration_seconds;
    uint8_t fan_percent;
    float target_temp_c;
    bool heater_enabled;
    bool humidity_auto_stop;
} sdhs_start_command_t;

const char *sdhs_state_to_string(sdhs_state_t state);
const char *sdhs_program_to_string(sdhs_program_t program);
const char *sdhs_fault_to_string(sdhs_fault_code_t fault);
const char *sdhs_completion_to_string(sdhs_completion_reason_t reason);
