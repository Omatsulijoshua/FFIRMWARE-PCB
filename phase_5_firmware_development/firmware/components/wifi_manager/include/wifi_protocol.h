#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "sdhs_types.h"

#define SDHS_WIFI_MAX_SSID_LEN 32
#define SDHS_WIFI_MAX_PASSWORD_LEN 64

typedef enum {
    SDHS_WIFI_STATE_UNPROVISIONED = 0,
    SDHS_WIFI_STATE_PROVISIONED_DISCONNECTED,
    SDHS_WIFI_STATE_CONNECTING,
    SDHS_WIFI_STATE_CONNECTED,
    SDHS_WIFI_STATE_RECOVERY,
} sdhs_wifi_state_t;

typedef struct {
    char ssid[SDHS_WIFI_MAX_SSID_LEN + 1];
    char password[SDHS_WIFI_MAX_PASSWORD_LEN + 1];
} sdhs_wifi_credentials_t;

sdhs_program_t wifi_protocol_program_from_string(const char *program);
const char *wifi_protocol_state_to_string(sdhs_wifi_state_t state);

