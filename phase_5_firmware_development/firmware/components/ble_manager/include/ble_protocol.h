#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "sdhs_types.h"

#define SDHS_BLE_UUID_BASE_SUFFIX "5f4d-4f7a-9b6d-3b0c7d9f1000"

#define SDHS_BLE_SERVICE_DEVICE_INFO "7b6a0001-5f4d-4f7a-9b6d-3b0c7d9f1000"
#define SDHS_BLE_SERVICE_CONTROL     "7b6a0100-5f4d-4f7a-9b6d-3b0c7d9f1000"
#define SDHS_BLE_SERVICE_STATUS      "7b6a0200-5f4d-4f7a-9b6d-3b0c7d9f1000"
#define SDHS_BLE_SERVICE_WIFI        "7b6a0300-5f4d-4f7a-9b6d-3b0c7d9f1000"
#define SDHS_BLE_SERVICE_OTA         "7b6a0400-5f4d-4f7a-9b6d-3b0c7d9f1000"
#define SDHS_BLE_SERVICE_DIAG        "7b6a0500-5f4d-4f7a-9b6d-3b0c7d9f1000"

typedef enum {
    SDHS_BLE_CMD_START_PROGRAM = 0x01,
    SDHS_BLE_CMD_STOP = 0x02,
    SDHS_BLE_CMD_CLEAR_FAULT = 0x03,
    SDHS_BLE_CMD_START_CUSTOM = 0x04,
    SDHS_BLE_CMD_ACK_COMPLETE = 0x05,
} sdhs_ble_command_id_t;

typedef enum {
    SDHS_BLE_RESULT_OK = 0x00,
    SDHS_BLE_RESULT_INVALID_LENGTH = 0x01,
    SDHS_BLE_RESULT_INVALID_COMMAND = 0x02,
    SDHS_BLE_RESULT_INVALID_STATE = 0x03,
    SDHS_BLE_RESULT_UNAUTHORIZED = 0x04,
    SDHS_BLE_RESULT_INTERNAL_ERROR = 0x05,
} sdhs_ble_result_t;

typedef struct {
    sdhs_ble_command_id_t command_id;
    sdhs_program_t program;
    uint32_t duration_seconds;
    uint8_t fan_percent;
    int16_t target_temp_centi_c;
    uint8_t flags;
} sdhs_ble_control_command_t;

typedef struct {
    sdhs_ble_command_id_t command_id;
    sdhs_ble_result_t result;
} sdhs_ble_command_response_t;

#define SDHS_BLE_CONTROL_COMMAND_LEN 10
#define SDHS_BLE_STATUS_PAYLOAD_LEN 18

bool ble_protocol_decode_control(const uint8_t *data, uint16_t len, sdhs_ble_control_command_t *out);
void ble_protocol_encode_status(const sdhs_status_t *status, uint8_t out[SDHS_BLE_STATUS_PAYLOAD_LEN]);

