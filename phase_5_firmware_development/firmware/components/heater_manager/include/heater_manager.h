#pragma once

#include <stdbool.h>

#include "esp_err.h"

esp_err_t heater_manager_init(void);
void heater_manager_request(float target_temp_c, bool enable);
void heater_manager_force_off(void);
void heater_manager_update(void);
bool heater_manager_is_on(void);
float heater_manager_get_target_temp(void);

