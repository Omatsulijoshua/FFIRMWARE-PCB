#include "diagnostics_manager.h"

#include "esp_log.h"
#include "esp_timer.h"

static const char *TAG = "diagnostics";

esp_err_t diagnostics_manager_init(void)
{
    ESP_LOGI(TAG, "diagnostics initialized");
    return ESP_OK;
}

void diagnostics_manager_record_fault(sdhs_fault_code_t fault)
{
    ESP_LOGE(TAG, "fault recorded: %s at %lld us",
             sdhs_fault_to_string(fault), esp_timer_get_time());
}

void diagnostics_manager_publish_status(void)
{
    /*
     * Later phases publish status to BLE notifications, LAN clients, and
     * persistent fault/cycle logs from here.
     */
}

