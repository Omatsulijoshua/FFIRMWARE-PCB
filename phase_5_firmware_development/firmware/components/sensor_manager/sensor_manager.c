#include "sensor_manager.h"

#include "app_config.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char *TAG = "sensor_manager";

static sdhs_sensor_sample_t s_latest = {
    .outlet_temp_c = 25.0f,
    .humidity_rh = 50.0f,
    .heater_zone_temp_c = 25.0f,
    .timestamp_ms = 0,
    .valid = false,
};

static uint32_t now_ms(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

esp_err_t sensor_manager_init(void)
{
    i2c_config_t cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDHS_I2C_SDA_GPIO,
        .scl_io_num = SDHS_I2C_SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = SDHS_I2C_FREQ_HZ,
    };

    ESP_ERROR_CHECK(i2c_param_config(SDHS_I2C_PORT, &cfg));
    ESP_ERROR_CHECK(i2c_driver_install(SDHS_I2C_PORT, I2C_MODE_MASTER, 0, 0, 0));

    ESP_LOGI(TAG, "I2C sensor bus initialized");
    return ESP_OK;
}

esp_err_t sensor_manager_update(void)
{
    /*
     * Hardware-specific SHT41/TMP117 transactions land here.
     * Until the PCB and sensor addresses are finalized, keep the control
     * stack testable with valid placeholder readings.
     */
    s_latest.timestamp_ms = now_ms();
    s_latest.valid = true;
    return ESP_OK;
}

sdhs_sensor_sample_t sensor_manager_get_latest(void)
{
    return s_latest;
}

bool sensor_manager_is_fresh(uint32_t timeout_ms)
{
    if (!s_latest.valid) {
        return false;
    }
    return (now_ms() - s_latest.timestamp_ms) <= timeout_ms;
}

