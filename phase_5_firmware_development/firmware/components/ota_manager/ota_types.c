#include "ota_types.h"

const char *ota_state_to_string(sdhs_ota_state_t state)
{
    switch (state) {
    case SDHS_OTA_STATE_IDLE: return "idle";
    case SDHS_OTA_STATE_CHECKING: return "checking";
    case SDHS_OTA_STATE_DOWNLOADING: return "downloading";
    case SDHS_OTA_STATE_VERIFYING: return "verifying";
    case SDHS_OTA_STATE_PENDING_REBOOT: return "pending_reboot";
    case SDHS_OTA_STATE_VALIDATING_BOOT: return "validating_boot";
    case SDHS_OTA_STATE_SUCCESS: return "success";
    case SDHS_OTA_STATE_FAILED: return "failed";
    case SDHS_OTA_STATE_ROLLBACK: return "rollback";
    default: return "unknown";
    }
}

const char *ota_error_to_string(sdhs_ota_error_t error)
{
    switch (error) {
    case SDHS_OTA_ERROR_NONE: return "none";
    case SDHS_OTA_ERROR_WIFI: return "wifi";
    case SDHS_OTA_ERROR_ACTIVE_CYCLE: return "active_cycle";
    case SDHS_OTA_ERROR_MANIFEST_DOWNLOAD: return "manifest_download";
    case SDHS_OTA_ERROR_MANIFEST_INVALID: return "manifest_invalid";
    case SDHS_OTA_ERROR_HARDWARE_MISMATCH: return "hardware_mismatch";
    case SDHS_OTA_ERROR_VERSION: return "version";
    case SDHS_OTA_ERROR_DOWNLOAD: return "download";
    case SDHS_OTA_ERROR_VERIFY: return "verify";
    case SDHS_OTA_ERROR_ROLLBACK: return "rollback";
    default: return "unknown";
    }
}

