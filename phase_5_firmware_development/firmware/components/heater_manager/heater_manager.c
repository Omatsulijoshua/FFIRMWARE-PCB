#include "heater_manager.h"

#include "app_config.h"
#include "driver/gpio.h"
#include "safety_manager.h"

static bool s_requested;
static bool s_on;
static float s_target_temp_c;

esp_err_t heater_manager_init(void)
{
    gpio_config_t cfg = {
        .pin_bit_mask = 1ULL << SDHS_HEATER_EN_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&cfg));
    heater_manager_force_off();
    return ESP_OK;
}

void heater_manager_request(float target_temp_c, bool enable)
{
    s_target_temp_c = target_temp_c;
    s_requested = enable;
}

void heater_manager_force_off(void)
{
    s_requested = false;
    s_on = false;
    gpio_set_level(SDHS_HEATER_EN_GPIO, 0);
}

void heater_manager_update(void)
{
    if (s_requested && safety_manager_heater_permitted()) {
        s_on = true;
        gpio_set_level(SDHS_HEATER_EN_GPIO, 1);
    } else {
        s_on = false;
        gpio_set_level(SDHS_HEATER_EN_GPIO, 0);
    }
}

bool heater_manager_is_on(void)
{
    return s_on;
}

float heater_manager_get_target_temp(void)
{
    return s_target_temp_c;
}

