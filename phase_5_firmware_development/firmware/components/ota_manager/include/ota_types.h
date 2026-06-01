#pragma once

#include <stdbool.h>
#include <stdint.h>

#define SDHS_OTA_MAX_URL_LEN 256
#define SDHS_OTA_MAX_VERSION_LEN 24
#define SDHS_OTA_SHA256_HEX_LEN 64

typedef enum {
    SDHS_OTA_STATE_IDLE = 0,
    SDHS_OTA_STATE_CHECKING,
    SDHS_OTA_STATE_DOWNLOADING,
    SDHS_OTA_STATE_VERIFYING,
    SDHS_OTA_STATE_PENDING_REBOOT,
    SDHS_OTA_STATE_VALIDATING_BOOT,
    SDHS_OTA_STATE_SUCCESS,
    SDHS_OTA_STATE_FAILED,
    SDHS_OTA_STATE_ROLLBACK,
} sdhs_ota_state_t;

typedef enum {
    SDHS_OTA_ERROR_NONE = 0,
    SDHS_OTA_ERROR_WIFI,
    SDHS_OTA_ERROR_ACTIVE_CYCLE,
    SDHS_OTA_ERROR_MANIFEST_DOWNLOAD,
    SDHS_OTA_ERROR_MANIFEST_INVALID,
    SDHS_OTA_ERROR_HARDWARE_MISMATCH,
    SDHS_OTA_ERROR_VERSION,
    SDHS_OTA_ERROR_DOWNLOAD,
    SDHS_OTA_ERROR_VERIFY,
    SDHS_OTA_ERROR_ROLLBACK,
} sdhs_ota_error_t;

typedef struct {
    char product[16];
    char version[SDHS_OTA_MAX_VERSION_LEN];
    char build[32];
    char hardware_min[24];
    char hardware_max[24];
    char image_url[SDHS_OTA_MAX_URL_LEN];
    uint32_t image_size;
    char sha256[SDHS_OTA_SHA256_HEX_LEN + 1];
    bool required;
} sdhs_ota_manifest_t;

typedef struct {
    sdhs_ota_state_t state;
    sdhs_ota_error_t error;
    uint8_t progress_percent;
    char current_version[SDHS_OTA_MAX_VERSION_LEN];
    char target_version[SDHS_OTA_MAX_VERSION_LEN];
} sdhs_ota_status_t;

const char *ota_state_to_string(sdhs_ota_state_t state);
const char *ota_error_to_string(sdhs_ota_error_t error);

