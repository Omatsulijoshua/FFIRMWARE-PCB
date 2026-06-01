#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"

esp_err_t fan_manager_init(void);
void fan_manager_set_target(uint8_t percent);
void fan_manager_update(void);
uint8_t fan_manager_get_target(void);
bool fan_manager_feedback_ok(void);

