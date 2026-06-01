#include "storage_manager.h"

#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include <string.h>

static const char *TAG = "storage";

esp_err_t storage_manager_init(void)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open("sdhs", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "failed to open NVS namespace: %s", esp_err_to_name(err));
        return err;
    }
    nvs_close(handle);
    ESP_LOGI(TAG, "storage initialized");
    return ESP_OK;
}

esp_err_t storage_manager_factory_reset(void)
{
    ESP_LOGW(TAG, "factory reset requested");
    return nvs_flash_erase();
}

esp_err_t storage_manager_save_wifi_credentials(const char *ssid, const char *password)
{
    if (ssid == NULL || password == NULL || strlen(ssid) == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open("sdhs", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        return err;
    }

    err = nvs_set_str(handle, "wifi_ssid", ssid);
    if (err == ESP_OK) {
        err = nvs_set_str(handle, "wifi_pass", password);
    }
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }

    nvs_close(handle);
    return err;
}

esp_err_t storage_manager_load_wifi_credentials(char *ssid, size_t ssid_len, char *password, size_t password_len)
{
    if (ssid == NULL || password == NULL || ssid_len == 0 || password_len == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open("sdhs", NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return err;
    }

    size_t required_ssid = ssid_len;
    size_t required_pass = password_len;
    err = nvs_get_str(handle, "wifi_ssid", ssid, &required_ssid);
    if (err == ESP_OK) {
        err = nvs_get_str(handle, "wifi_pass", password, &required_pass);
    }

    nvs_close(handle);
    return err;
}

