#pragma once

#include "esp_err.h"
#include "sdhs_types.h"

esp_err_t state_machine_init(void);
void state_machine_tick(void);
esp_err_t state_machine_start(const sdhs_start_command_t *cmd);
esp_err_t state_machine_start_program(sdhs_program_t program, uint32_t requested_seconds);
void state_machine_stop(void);
void state_machine_clear_fault(void);
sdhs_status_t state_machine_get_status(void);
