#pragma once

#include <stdint.h>

#include "esp_err.h"

esp_err_t ble_manager_init(void);
esp_err_t ble_manager_handle_control_write(const uint8_t *data, uint16_t len);
esp_err_t ble_manager_build_status_payload(uint8_t *out, uint16_t out_len);
