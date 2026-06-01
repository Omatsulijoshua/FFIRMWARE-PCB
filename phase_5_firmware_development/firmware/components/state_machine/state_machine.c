#include "state_machine.h"

#include "app_config.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "fan_manager.h"
#include "heater_manager.h"
#include "program_profiles.h"
#include "safety_manager.h"
#include "sensor_manager.h"

static const char *TAG = "state_machine";

typedef struct {
    sdhs_state_t state;
    sdhs_program_t program;
    uint32_t started_ms;
    uint32_t cooldown_started_ms;
    uint32_t duration_seconds;
    uint8_t fan_percent;
    float target_temp_c;
    bool heater_enabled;
    bool humidity_auto_stop;
    uint8_t humidity_dry_ticks;
    sdhs_completion_reason_t completion_reason;
} sm_ctx_t;

static sm_ctx_t s_ctx;

static uint32_t now_ms(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

static uint32_t elapsed_seconds(void)
{
    if (s_ctx.started_ms == 0) {
        return 0;
    }
    return (now_ms() - s_ctx.started_ms) / 1000U;
}

static void transition(sdhs_state_t next)
{
    if (s_ctx.state != next) {
        ESP_LOGI(TAG, "%s -> %s", sdhs_state_to_string(s_ctx.state), sdhs_state_to_string(next));
        s_ctx.state = next;
    }
}

static void stop_outputs(void)
{
    heater_manager_force_off();
    fan_manager_set_target(0);
}

static bool cycle_has_heat(void)
{
    return s_ctx.heater_enabled &&
        (s_ctx.state == SDHS_STATE_PREHEAT ||
         s_ctx.state == SDHS_STATE_DRYING ||
         s_ctx.state == SDHS_STATE_HYGIENE ||
         s_ctx.state == SDHS_STATE_COOLDOWN);
}

static void enter_cooldown(sdhs_completion_reason_t reason)
{
    s_ctx.completion_reason = reason;
    s_ctx.cooldown_started_ms = now_ms();
    heater_manager_force_off();
    fan_manager_set_target(60);
    transition(SDHS_STATE_COOLDOWN);
}

static void enter_complete(sdhs_completion_reason_t reason)
{
    s_ctx.completion_reason = reason;
    s_ctx.cooldown_started_ms = 0;
    stop_outputs();
    transition(SDHS_STATE_COMPLETE);
}

esp_err_t state_machine_init(void)
{
    s_ctx.state = SDHS_STATE_STARTUP;
    s_ctx.program = SDHS_PROGRAM_NONE;
    s_ctx.duration_seconds = 0;
    s_ctx.completion_reason = SDHS_COMPLETE_NONE;
    stop_outputs();

    if (safety_manager_self_test_passed()) {
        transition(SDHS_STATE_IDLE);
    } else {
        transition(SDHS_STATE_FAULT);
    }

    return ESP_OK;
}

esp_err_t state_machine_start(const sdhs_start_command_t *cmd)
{
    if (cmd == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if (s_ctx.state != SDHS_STATE_IDLE && s_ctx.state != SDHS_STATE_COMPLETE) {
        return ESP_ERR_INVALID_STATE;
    }

    if (safety_manager_get_fault() != SDHS_FAULT_NONE) {
        transition(SDHS_STATE_FAULT);
        return ESP_ERR_INVALID_STATE;
    }

    s_ctx.program = cmd->program;
    s_ctx.duration_seconds = cmd->duration_seconds == 0 ?
        SDHS_DEFAULT_MAX_CYCLE_SECONDS : cmd->duration_seconds;
    s_ctx.fan_percent = cmd->fan_percent > 100 ? 100 : cmd->fan_percent;
    s_ctx.target_temp_c = cmd->target_temp_c;
    s_ctx.heater_enabled = cmd->heater_enabled;
    s_ctx.humidity_auto_stop = cmd->humidity_auto_stop;
    s_ctx.started_ms = now_ms();
    s_ctx.cooldown_started_ms = 0;
    s_ctx.humidity_dry_ticks = 0;
    s_ctx.completion_reason = SDHS_COMPLETE_NONE;

    fan_manager_set_target(s_ctx.fan_percent);
    heater_manager_request(s_ctx.target_temp_c, false);

    if (cmd->program == SDHS_PROGRAM_FAN_ONLY || !cmd->heater_enabled) {
        transition(SDHS_STATE_DRYING);
    } else if (cmd->program == SDHS_PROGRAM_HYGIENE) {
        transition(SDHS_STATE_HYGIENE);
    } else {
        transition(SDHS_STATE_PREHEAT);
    }

    return ESP_OK;
}

esp_err_t state_machine_start_program(sdhs_program_t program, uint32_t requested_seconds)
{
    sdhs_start_command_t cmd = program_profiles_make_start(program, requested_seconds);
    if (cmd.program == SDHS_PROGRAM_NONE) {
        return ESP_ERR_INVALID_ARG;
    }
    return state_machine_start(&cmd);
}

void state_machine_stop(void)
{
    if (cycle_has_heat()) {
        enter_cooldown(SDHS_COMPLETE_USER_STOP);
    } else if (s_ctx.state == SDHS_STATE_DRYING) {
        enter_complete(SDHS_COMPLETE_USER_STOP);
    } else {
        stop_outputs();
        s_ctx.program = SDHS_PROGRAM_NONE;
        s_ctx.started_ms = 0;
        s_ctx.completion_reason = SDHS_COMPLETE_NONE;
        transition(SDHS_STATE_IDLE);
    }
}

void state_machine_clear_fault(void)
{
    safety_manager_clear_fault();
    if (safety_manager_self_test_passed()) {
        transition(SDHS_STATE_IDLE);
    }
}

static bool humidity_complete(void)
{
    if (!s_ctx.humidity_auto_stop) {
        s_ctx.humidity_dry_ticks = 0;
        return false;
    }

    sdhs_sensor_sample_t sample = sensor_manager_get_latest();
    if (sample.valid && sample.humidity_rh <= 35.0f && elapsed_seconds() > 15U * 60U) {
        if (s_ctx.humidity_dry_ticks < 5) {
            s_ctx.humidity_dry_ticks++;
        }
    } else {
        s_ctx.humidity_dry_ticks = 0;
    }

    return s_ctx.humidity_dry_ticks >= 5;
}

void state_machine_tick(void)
{
    sdhs_fault_code_t fault = safety_manager_get_fault();
    if (fault != SDHS_FAULT_NONE) {
        stop_outputs();
        s_ctx.completion_reason = SDHS_COMPLETE_FAULT;
        transition(SDHS_STATE_FAULT);
        return;
    }

    sdhs_sensor_sample_t sample = sensor_manager_get_latest();
    uint32_t elapsed = elapsed_seconds();

    switch (s_ctx.state) {
    case SDHS_STATE_IDLE:
    case SDHS_STATE_COMPLETE:
        stop_outputs();
        break;

    case SDHS_STATE_PREHEAT:
        fan_manager_set_target(s_ctx.fan_percent);
        heater_manager_request(s_ctx.target_temp_c, s_ctx.heater_enabled);
        if (sample.valid && sample.outlet_temp_c >= (s_ctx.target_temp_c - 3.0f)) {
            transition(SDHS_STATE_DRYING);
        }
        if (elapsed >= s_ctx.duration_seconds) {
            enter_cooldown(SDHS_COMPLETE_TIMER);
        }
        break;

    case SDHS_STATE_DRYING:
        fan_manager_set_target(s_ctx.fan_percent);
        heater_manager_request(s_ctx.target_temp_c, s_ctx.heater_enabled);
        if (humidity_complete()) {
            enter_cooldown(SDHS_COMPLETE_HUMIDITY_DRY);
        } else if (elapsed >= s_ctx.duration_seconds) {
            enter_cooldown(SDHS_COMPLETE_TIMER);
        }
        break;

    case SDHS_STATE_HYGIENE:
        fan_manager_set_target(s_ctx.fan_percent);
        heater_manager_request(s_ctx.target_temp_c, true);
        if (elapsed >= s_ctx.duration_seconds) {
            enter_cooldown(SDHS_COMPLETE_TIMER);
        }
        break;

    case SDHS_STATE_COOLDOWN:
        heater_manager_force_off();
        fan_manager_set_target(60);
        uint32_t cooldown_elapsed = s_ctx.cooldown_started_ms == 0 ?
            0 : (now_ms() - s_ctx.cooldown_started_ms) / 1000U;
        if ((sample.valid && sample.outlet_temp_c <= SDHS_TEMP_COOLDOWN_COMPLETE_C) ||
            cooldown_elapsed >= 15U * 60U) {
            enter_complete(s_ctx.completion_reason == SDHS_COMPLETE_NONE ?
                SDHS_COMPLETE_COOLDOWN_DONE : s_ctx.completion_reason);
        }
        break;

    case SDHS_STATE_FAULT:
    case SDHS_STATE_STARTUP:
    default:
        stop_outputs();
        break;
    }
}

sdhs_status_t state_machine_get_status(void)
{
    sdhs_sensor_sample_t sample = sensor_manager_get_latest();
    uint32_t elapsed = elapsed_seconds();
    uint32_t remaining = elapsed >= s_ctx.duration_seconds ? 0 : s_ctx.duration_seconds - elapsed;

    sdhs_status_t status = {
        .state = s_ctx.state,
        .program = s_ctx.program,
        .fault = safety_manager_get_fault(),
        .elapsed_seconds = elapsed,
        .remaining_seconds = remaining,
        .outlet_temp_c = sample.outlet_temp_c,
        .humidity_rh = sample.humidity_rh,
        .fan_percent = fan_manager_get_target(),
        .heater_on = heater_manager_is_on(),
        .completion_reason = s_ctx.completion_reason,
    };
    return status;
}
