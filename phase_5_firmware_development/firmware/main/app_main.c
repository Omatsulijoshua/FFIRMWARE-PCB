#include "app_config.h"

#include "ble_manager.h"
#include "diagnostics_manager.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_task_wdt.h"
#include "fan_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "heater_manager.h"
#include "local_api.h"
#include "nvs_flash.h"
#include "ota_manager.h"
#include "safety_manager.h"
#include "sensor_manager.h"
#include "state_machine.h"
#include "storage_manager.h"
#include "wifi_manager.h"

static const char *TAG = "sdhs_main";

static void control_task(void *arg)
{
    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));

    while (true) {
        sensor_manager_update();
        safety_manager_update();
        state_machine_tick();
        fan_manager_update();
        heater_manager_update();
        diagnostics_manager_publish_status();

        ESP_ERROR_CHECK(esp_task_wdt_reset());
        vTaskDelay(pdMS_TO_TICKS(SDHS_CONTROL_TASK_PERIOD_MS));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Smart Drying & Hygiene System firmware %s (%s)",
             SDHS_FIRMWARE_VERSION, SDHS_HARDWARE_REVISION);

    esp_err_t nvs_ret = nvs_flash_init();
    if (nvs_ret == ESP_ERR_NVS_NO_FREE_PAGES || nvs_ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    } else {
        ESP_ERROR_CHECK(nvs_ret);
    }

    ESP_ERROR_CHECK(storage_manager_init());
    ESP_ERROR_CHECK(diagnostics_manager_init());
    ESP_ERROR_CHECK(sensor_manager_init());
    ESP_ERROR_CHECK(sensor_manager_update());
    ESP_ERROR_CHECK(fan_manager_init());
    ESP_ERROR_CHECK(heater_manager_init());
    ESP_ERROR_CHECK(safety_manager_init());
    ESP_ERROR_CHECK(state_machine_init());
    ESP_ERROR_CHECK(wifi_manager_init());
    (void)wifi_manager_connect_saved();
    ESP_ERROR_CHECK(local_api_init());
    ESP_ERROR_CHECK(ble_manager_init());
    ESP_ERROR_CHECK(ota_manager_init());

    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_ota_img_states_t ota_state = ESP_OTA_IMG_UNDEFINED;
    if (esp_ota_get_state_partition(running, &ota_state) == ESP_OK &&
        ota_state == ESP_OTA_IMG_PENDING_VERIFY) {
        ESP_LOGI(TAG, "OTA image pending verification");
        ESP_ERROR_CHECK(ota_manager_validate_pending_boot());
    }

    xTaskCreate(control_task, "control_task", 6144, NULL, 8, NULL);
}
