#pragma once

#include "esp_err.h"
#include "ota_types.h"

esp_err_t ota_manager_init(void);
esp_err_t ota_manager_start_update(const char *manifest_url);
sdhs_ota_status_t ota_manager_get_status(void);
esp_err_t ota_manager_validate_pending_boot(void);
