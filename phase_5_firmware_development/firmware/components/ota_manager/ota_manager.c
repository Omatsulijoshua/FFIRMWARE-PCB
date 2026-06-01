#include "ota_manager.h"

#include <string.h>

#include "app_config.h"
#include "cJSON.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "safety_manager.h"
#include "state_machine.h"
#include "wifi_manager.h"

static const char *TAG = "ota_manager";
static sdhs_ota_status_t s_status;

static bool is_hex_sha256(const char *sha)
{
    if (sha == NULL || strlen(sha) != SDHS_OTA_SHA256_HEX_LEN) {
        return false;
    }
    for (size_t i = 0; i < SDHS_OTA_SHA256_HEX_LEN; ++i) {
        bool digit = sha[i] >= '0' && sha[i] <= '9';
        bool lower = sha[i] >= 'a' && sha[i] <= 'f';
        bool upper = sha[i] >= 'A' && sha[i] <= 'F';
        if (!digit && !lower && !upper) {
            return false;
        }
    }
    return true;
}

static void set_status(sdhs_ota_state_t state, sdhs_ota_error_t error, uint8_t progress)
{
    s_status.state = state;
    s_status.error = error;
    s_status.progress_percent = progress;
}

static bool state_allows_ota(void)
{
    sdhs_status_t status = state_machine_get_status();
    return status.state == SDHS_STATE_IDLE || status.state == SDHS_STATE_COMPLETE;
}

static esp_err_t fetch_manifest(const char *url, char *buffer, size_t buffer_len)
{
    esp_http_client_config_t config = {
        .url = url,
        .timeout_ms = 15000,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        return ESP_FAIL;
    }

    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        esp_http_client_cleanup(client);
        return err;
    }

    int total = 0;
    while (total < (int)buffer_len - 1) {
        int read = esp_http_client_read(client, buffer + total, (int)buffer_len - 1 - total);
        if (read < 0) {
            err = ESP_FAIL;
            break;
        }
        if (read == 0) {
            break;
        }
        total += read;
    }
    buffer[total] = '\0';
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
    return err;
}

static const char *json_get_string(cJSON *root, const char *name)
{
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root, name);
    return cJSON_IsString(item) ? item->valuestring : NULL;
}

static esp_err_t parse_manifest(const char *json, sdhs_ota_manifest_t *manifest)
{
    cJSON *root = cJSON_Parse(json);
    if (root == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    memset(manifest, 0, sizeof(*manifest));
    const char *product = json_get_string(root, "product");
    const char *version = json_get_string(root, "version");
    const char *build = json_get_string(root, "build");
    const char *hardware_min = json_get_string(root, "hardwareMin");
    const char *hardware_max = json_get_string(root, "hardwareMax");
    const char *image_url = json_get_string(root, "imageUrl");
    const char *sha256 = json_get_string(root, "sha256");
    cJSON *image_size = cJSON_GetObjectItemCaseSensitive(root, "imageSize");
    cJSON *required = cJSON_GetObjectItemCaseSensitive(root, "required");

    if (product == NULL || version == NULL || build == NULL ||
        hardware_min == NULL || hardware_max == NULL ||
        image_url == NULL || sha256 == NULL || !cJSON_IsNumber(image_size)) {
        cJSON_Delete(root);
        return ESP_ERR_INVALID_ARG;
    }

    strlcpy(manifest->product, product, sizeof(manifest->product));
    strlcpy(manifest->version, version, sizeof(manifest->version));
    strlcpy(manifest->build, build, sizeof(manifest->build));
    strlcpy(manifest->hardware_min, hardware_min, sizeof(manifest->hardware_min));
    strlcpy(manifest->hardware_max, hardware_max, sizeof(manifest->hardware_max));
    strlcpy(manifest->image_url, image_url, sizeof(manifest->image_url));
    strlcpy(manifest->sha256, sha256, sizeof(manifest->sha256));
    manifest->image_size = (uint32_t)image_size->valuedouble;
    manifest->required = cJSON_IsBool(required) ? cJSON_IsTrue(required) : false;

    cJSON_Delete(root);
    return ESP_OK;
}

static esp_err_t validate_manifest(const sdhs_ota_manifest_t *manifest)
{
    if (strcmp(manifest->product, "sdhs") != 0) {
        return ESP_ERR_INVALID_ARG;
    }
    if (manifest->image_size == 0 || !is_hex_sha256(manifest->sha256)) {
        return ESP_ERR_INVALID_ARG;
    }
    if (strncmp(manifest->image_url, "https://", 8) != 0) {
        return ESP_ERR_INVALID_ARG;
    }
    if (strcmp(SDHS_HARDWARE_REVISION, manifest->hardware_min) < 0 ||
        strcmp(SDHS_HARDWARE_REVISION, manifest->hardware_max) > 0) {
        return ESP_ERR_NOT_SUPPORTED;
    }
    if (strcmp(SDHS_FIRMWARE_VERSION, manifest->version) >= 0) {
        return ESP_ERR_INVALID_STATE;
    }
    return ESP_OK;
}

esp_err_t ota_manager_init(void)
{
    memset(&s_status, 0, sizeof(s_status));
    s_status.state = SDHS_OTA_STATE_IDLE;
    s_status.error = SDHS_OTA_ERROR_NONE;
    strlcpy(s_status.current_version, SDHS_FIRMWARE_VERSION, sizeof(s_status.current_version));
    ESP_LOGI(TAG, "OTA manager initialized");
    return ESP_OK;
}

esp_err_t ota_manager_validate_pending_boot(void)
{
    set_status(SDHS_OTA_STATE_VALIDATING_BOOT, SDHS_OTA_ERROR_NONE, 100);
    if (safety_manager_self_test_passed()) {
        esp_err_t err = esp_ota_mark_app_valid_cancel_rollback();
        if (err == ESP_OK || err == ESP_ERR_OTA_VALIDATE_FAILED) {
            set_status(SDHS_OTA_STATE_SUCCESS, SDHS_OTA_ERROR_NONE, 100);
            return ESP_OK;
        }
        return err;
    }

    set_status(SDHS_OTA_STATE_ROLLBACK, SDHS_OTA_ERROR_ROLLBACK, 0);
    return esp_ota_mark_app_invalid_rollback_and_reboot();
}

sdhs_ota_status_t ota_manager_get_status(void)
{
    return s_status;
}

esp_err_t ota_manager_start_update(const char *manifest_url)
{
    if (manifest_url == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!wifi_manager_is_connected()) {
        ESP_LOGW(TAG, "OTA rejected: WiFi not connected");
        set_status(SDHS_OTA_STATE_FAILED, SDHS_OTA_ERROR_WIFI, 0);
        return ESP_ERR_INVALID_STATE;
    }

    if (!state_allows_ota()) {
        ESP_LOGW(TAG, "OTA rejected: active cycle");
        set_status(SDHS_OTA_STATE_FAILED, SDHS_OTA_ERROR_ACTIVE_CYCLE, 0);
        return ESP_ERR_INVALID_STATE;
    }

    set_status(SDHS_OTA_STATE_CHECKING, SDHS_OTA_ERROR_NONE, 0);

    char manifest_json[1024];
    esp_err_t err = fetch_manifest(manifest_url, manifest_json, sizeof(manifest_json));
    if (err != ESP_OK) {
        set_status(SDHS_OTA_STATE_FAILED, SDHS_OTA_ERROR_MANIFEST_DOWNLOAD, 0);
        return err;
    }

    sdhs_ota_manifest_t manifest;
    err = parse_manifest(manifest_json, &manifest);
    if (err != ESP_OK) {
        set_status(SDHS_OTA_STATE_FAILED, SDHS_OTA_ERROR_MANIFEST_INVALID, 0);
        return err;
    }

    err = validate_manifest(&manifest);
    if (err == ESP_ERR_NOT_SUPPORTED) {
        set_status(SDHS_OTA_STATE_FAILED, SDHS_OTA_ERROR_HARDWARE_MISMATCH, 0);
        return err;
    }
    if (err == ESP_ERR_INVALID_STATE) {
        set_status(SDHS_OTA_STATE_FAILED, SDHS_OTA_ERROR_VERSION, 0);
        return err;
    }
    if (err != ESP_OK) {
        set_status(SDHS_OTA_STATE_FAILED, SDHS_OTA_ERROR_MANIFEST_INVALID, 0);
        return err;
    }

    strlcpy(s_status.target_version, manifest.version, sizeof(s_status.target_version));
    set_status(SDHS_OTA_STATE_DOWNLOADING, SDHS_OTA_ERROR_NONE, 10);

    esp_http_client_config_t http_config = {
        .url = manifest.image_url,
        .timeout_ms = 15000,
    };
    esp_https_ota_config_t ota_config = {
        .http_config = &http_config,
    };

    ESP_LOGI(TAG, "starting OTA to version %s from %s", manifest.version, manifest.image_url);
    err = esp_https_ota(&ota_config);
    if (err != ESP_OK) {
        set_status(SDHS_OTA_STATE_FAILED, SDHS_OTA_ERROR_DOWNLOAD, 0);
        return err;
    }

    set_status(SDHS_OTA_STATE_PENDING_REBOOT, SDHS_OTA_ERROR_NONE, 100);
    return ESP_OK;
}
