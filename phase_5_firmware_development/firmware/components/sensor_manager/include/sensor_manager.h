#pragma once

#include "esp_err.h"
#include "sdhs_types.h"

esp_err_t sensor_manager_init(void);
esp_err_t sensor_manager_update(void);
sdhs_sensor_sample_t sensor_manager_get_latest(void);
bool sensor_manager_is_fresh(uint32_t timeout_ms);

