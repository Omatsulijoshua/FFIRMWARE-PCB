#pragma once

#include <stdbool.h>

#include "esp_err.h"
#include "sdhs_types.h"

esp_err_t safety_manager_init(void);
void safety_manager_update(void);
bool safety_manager_heater_permitted(void);
bool safety_manager_self_test_passed(void);
sdhs_fault_code_t safety_manager_get_fault(void);
void safety_manager_clear_fault(void);
void safety_manager_latch_fault(sdhs_fault_code_t fault);

