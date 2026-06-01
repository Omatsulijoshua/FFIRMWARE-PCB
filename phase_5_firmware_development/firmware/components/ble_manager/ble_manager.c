#include "ble_manager.h"

#include "ble_protocol.h"
#include "esp_log.h"
#include "state_machine.h"

static const char *TAG = "ble_manager";
static sdhs_ble_command_response_t s_last_response;

esp_err_t ble_manager_init(void)
{
    /*
     * Phase 7 will define the complete GATT table.
     * Required services:
     * - Device information
     * - Control commands
     * - Live status notifications
     * - WiFi provisioning
     * - Diagnostics
     * - OTA control/recovery
     */
    ESP_LOGI(TAG, "BLE manager initialized as protocol scaffold");
    return ESP_OK;
}

static esp_err_t handle_decoded_command(const sdhs_ble_control_command_t *cmd)
{
    switch (cmd->command_id) {
    case SDHS_BLE_CMD_START_PROGRAM:
        return state_machine_start_program(cmd->program, cmd->duration_seconds);

    case SDHS_BLE_CMD_STOP:
        state_machine_stop();
        return ESP_OK;

    case SDHS_BLE_CMD_CLEAR_FAULT:
        state_machine_clear_fault();
        return ESP_OK;

    case SDHS_BLE_CMD_START_CUSTOM: {
        sdhs_start_command_t start = {
            .program = cmd->program,
            .duration_seconds = cmd->duration_seconds,
            .fan_percent = cmd->fan_percent,
            .target_temp_c = ((float)cmd->target_temp_centi_c) / 100.0f,
            .heater_enabled = (cmd->flags & 0x01) != 0,
            .humidity_auto_stop = (cmd->flags & 0x02) != 0,
        };
        return state_machine_start(&start);
    }

    case SDHS_BLE_CMD_ACK_COMPLETE:
        state_machine_stop();
        return ESP_OK;

    default:
        return ESP_ERR_INVALID_ARG;
    }
}

esp_err_t ble_manager_handle_control_write(const uint8_t *data, uint16_t len)
{
    sdhs_ble_control_command_t cmd;
    if (!ble_protocol_decode_control(data, len, &cmd)) {
        s_last_response.command_id = 0;
        s_last_response.result = SDHS_BLE_RESULT_INVALID_LENGTH;
        ESP_LOGW(TAG, "invalid BLE control payload length: %u", len);
        return ESP_ERR_INVALID_SIZE;
    }

    esp_err_t err = handle_decoded_command(&cmd);
    s_last_response.command_id = cmd.command_id;
    if (err == ESP_OK) {
        s_last_response.result = SDHS_BLE_RESULT_OK;
    } else if (err == ESP_ERR_INVALID_STATE) {
        s_last_response.result = SDHS_BLE_RESULT_INVALID_STATE;
    } else if (err == ESP_ERR_INVALID_ARG) {
        s_last_response.result = SDHS_BLE_RESULT_INVALID_COMMAND;
    } else {
        s_last_response.result = SDHS_BLE_RESULT_INTERNAL_ERROR;
    }

    ESP_LOGI(TAG, "BLE command %u result %u", cmd.command_id, s_last_response.result);
    return err;
}

esp_err_t ble_manager_build_status_payload(uint8_t *out, uint16_t out_len)
{
    if (out == NULL || out_len < SDHS_BLE_STATUS_PAYLOAD_LEN) {
        return ESP_ERR_INVALID_SIZE;
    }

    sdhs_status_t status = state_machine_get_status();
    ble_protocol_encode_status(&status, out);
    return ESP_OK;
}
