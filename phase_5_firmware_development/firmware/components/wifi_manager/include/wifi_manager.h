#pragma once

#include <stdbool.h>

#include "esp_err.h"
#include "wifi_protocol.h"

esp_err_t wifi_manager_init(void);
esp_err_t wifi_manager_save_credentials(const char *ssid, const char *password);
esp_err_t wifi_manager_connect_saved(void);
bool wifi_manager_is_connected(void);
sdhs_wifi_state_t wifi_manager_get_state(void);
int8_t wifi_manager_get_rssi(void);
