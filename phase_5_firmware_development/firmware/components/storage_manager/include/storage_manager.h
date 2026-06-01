#pragma once

#include <stddef.h>

#include "esp_err.h"

esp_err_t storage_manager_init(void);
esp_err_t storage_manager_factory_reset(void);
esp_err_t storage_manager_save_wifi_credentials(const char *ssid, const char *password);
esp_err_t storage_manager_load_wifi_credentials(char *ssid, size_t ssid_len, char *password, size_t password_len);
