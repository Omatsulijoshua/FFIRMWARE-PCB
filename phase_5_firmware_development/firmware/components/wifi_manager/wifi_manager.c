#include "wifi_manager.h"

#include <stdbool.h>
#include <string.h>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "storage_manager.h"

static const char *TAG = "wifi_manager";
static sdhs_wifi_state_t s_state = SDHS_WIFI_STATE_UNPROVISIONED;
static int8_t s_rssi = 0;

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "WiFi station started");
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        s_state = SDHS_WIFI_STATE_PROVISIONED_DISCONNECTED;
        ESP_LOGW(TAG, "WiFi disconnected, retrying");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        s_state = SDHS_WIFI_STATE_CONNECTED;
        ESP_LOGI(TAG, "WiFi connected, IP: " IPSTR, IP2STR(&event->ip_info.ip));

        wifi_ap_record_t ap_info;
        if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
            s_rssi = ap_info.rssi;
        }
    }
}

esp_err_t wifi_manager_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi manager initialized");
    return ESP_OK;
}

esp_err_t wifi_manager_save_credentials(const char *ssid, const char *password)
{
    esp_err_t err = storage_manager_save_wifi_credentials(ssid, password);
    if (err == ESP_OK) {
        s_state = SDHS_WIFI_STATE_PROVISIONED_DISCONNECTED;
        ESP_LOGI(TAG, "WiFi credentials saved for SSID '%s'", ssid);
    }
    return err;
}

esp_err_t wifi_manager_connect_saved(void)
{
    sdhs_wifi_credentials_t creds = {0};
    esp_err_t err = storage_manager_load_wifi_credentials(
        creds.ssid, sizeof(creds.ssid), creds.password, sizeof(creds.password));
    if (err != ESP_OK) {
        s_state = SDHS_WIFI_STATE_UNPROVISIONED;
        ESP_LOGW(TAG, "no saved WiFi credentials: %s", esp_err_to_name(err));
        return err;
    }

    wifi_config_t wifi_config = {0};
    strlcpy((char *)wifi_config.sta.ssid, creds.ssid, sizeof(wifi_config.sta.ssid));
    strlcpy((char *)wifi_config.sta.password, creds.password, sizeof(wifi_config.sta.password));
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    s_state = SDHS_WIFI_STATE_CONNECTING;
    ESP_LOGI(TAG, "connecting to saved SSID '%s'", creds.ssid);
    return esp_wifi_connect();
}

bool wifi_manager_is_connected(void)
{
    return s_state == SDHS_WIFI_STATE_CONNECTED;
}

sdhs_wifi_state_t wifi_manager_get_state(void)
{
    return s_state;
}

int8_t wifi_manager_get_rssi(void)
{
    return s_rssi;
}
