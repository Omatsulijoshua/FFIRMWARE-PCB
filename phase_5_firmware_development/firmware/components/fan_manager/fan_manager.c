#include "fan_manager.h"

#include "app_config.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"

static const char *TAG = "fan_manager";
static uint8_t s_target_percent;

esp_err_t fan_manager_init(void)
{
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = 25000,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer));

    ledc_channel_config_t channel = {
        .gpio_num = SDHS_FAN_PWM_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel));

    gpio_config_t tach = {
        .pin_bit_mask = 1ULL << SDHS_FAN_TACH_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&tach));

    ESP_LOGI(TAG, "fan PWM initialized");
    return ESP_OK;
}

void fan_manager_set_target(uint8_t percent)
{
    s_target_percent = percent > 100 ? 100 : percent;
}

void fan_manager_update(void)
{
    uint32_t duty = (uint32_t)((s_target_percent * 1023U) / 100U);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

uint8_t fan_manager_get_target(void)
{
    return s_target_percent;
}

bool fan_manager_feedback_ok(void)
{
    /*
     * Replace with tach pulse measurement after final fan selection.
     * For now, assume feedback is OK when a commanded fan is connected.
     */
    return true;
}

