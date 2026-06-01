#include "safety_manager.h"

#include "app_config.h"
#include "diagnostics_manager.h"
#include "fan_manager.h"
#include "sensor_manager.h"

static sdhs_fault_code_t s_fault = SDHS_FAULT_NONE;
static bool s_heater_permitted;

esp_err_t safety_manager_init(void)
{
    s_fault = SDHS_FAULT_NONE;
    s_heater_permitted = false;
    return ESP_OK;
}

void safety_manager_update(void)
{
    s_heater_permitted = false;

    if (!sensor_manager_is_fresh(SDHS_SENSOR_STALE_TIMEOUT_MS)) {
        safety_manager_latch_fault(SDHS_FAULT_SENSOR_STALE);
        return;
    }

    sdhs_sensor_sample_t sample = sensor_manager_get_latest();
    if (!sample.valid || sample.outlet_temp_c < -20.0f || sample.outlet_temp_c > 125.0f) {
        safety_manager_latch_fault(SDHS_FAULT_TEMP_SENSOR_INVALID);
        return;
    }

    if (sample.humidity_rh < 0.0f || sample.humidity_rh > 100.0f) {
        safety_manager_latch_fault(SDHS_FAULT_HUMIDITY_SENSOR_INVALID);
        return;
    }

    if (sample.outlet_temp_c >= SDHS_TEMP_HARD_LIMIT_C ||
        sample.heater_zone_temp_c >= 90.0f) {
        safety_manager_latch_fault(SDHS_FAULT_OVERTEMP);
        return;
    }

    if (!fan_manager_feedback_ok()) {
        safety_manager_latch_fault(SDHS_FAULT_FAN_FAILURE);
        return;
    }

    if (s_fault == SDHS_FAULT_NONE) {
        s_heater_permitted = true;
    }
}

bool safety_manager_heater_permitted(void)
{
    return s_heater_permitted && s_fault == SDHS_FAULT_NONE;
}

bool safety_manager_self_test_passed(void)
{
    safety_manager_update();
    return s_fault == SDHS_FAULT_NONE;
}

sdhs_fault_code_t safety_manager_get_fault(void)
{
    return s_fault;
}

void safety_manager_clear_fault(void)
{
    s_fault = SDHS_FAULT_NONE;
    s_heater_permitted = false;
}

void safety_manager_latch_fault(sdhs_fault_code_t fault)
{
    if (fault != SDHS_FAULT_NONE && s_fault == SDHS_FAULT_NONE) {
        s_fault = fault;
        diagnostics_manager_record_fault(fault);
    }
}

