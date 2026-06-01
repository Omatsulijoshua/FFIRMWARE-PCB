#include "ble_protocol.h"

#include <string.h>

static uint32_t read_u32_le(const uint8_t *p)
{
    return ((uint32_t)p[0]) |
           ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) |
           ((uint32_t)p[3] << 24);
}

static int16_t read_i16_le(const uint8_t *p)
{
    return (int16_t)(((uint16_t)p[0]) | ((uint16_t)p[1] << 8));
}

static void write_u32_le(uint8_t *p, uint32_t value)
{
    p[0] = (uint8_t)(value & 0xff);
    p[1] = (uint8_t)((value >> 8) & 0xff);
    p[2] = (uint8_t)((value >> 16) & 0xff);
    p[3] = (uint8_t)((value >> 24) & 0xff);
}

static void write_i16_le(uint8_t *p, int16_t value)
{
    p[0] = (uint8_t)(((uint16_t)value) & 0xff);
    p[1] = (uint8_t)((((uint16_t)value) >> 8) & 0xff);
}

bool ble_protocol_decode_control(const uint8_t *data, uint16_t len, sdhs_ble_control_command_t *out)
{
    if (data == NULL || out == NULL || len != SDHS_BLE_CONTROL_COMMAND_LEN) {
        return false;
    }

    memset(out, 0, sizeof(*out));
    out->command_id = (sdhs_ble_command_id_t)data[0];
    out->program = (sdhs_program_t)data[1];
    out->duration_seconds = read_u32_le(&data[2]);
    out->fan_percent = data[6];
    out->target_temp_centi_c = read_i16_le(&data[7]);
    out->flags = data[9];
    return true;
}

void ble_protocol_encode_status(const sdhs_status_t *status, uint8_t out[SDHS_BLE_STATUS_PAYLOAD_LEN])
{
    if (status == NULL || out == NULL) {
        return;
    }

    memset(out, 0, SDHS_BLE_STATUS_PAYLOAD_LEN);
    out[0] = (uint8_t)status->state;
    out[1] = (uint8_t)status->program;
    out[2] = (uint8_t)status->fault;
    write_u32_le(&out[3], status->elapsed_seconds);
    write_u32_le(&out[7], status->remaining_seconds);
    write_i16_le(&out[11], (int16_t)(status->outlet_temp_c * 100.0f));
    write_i16_le(&out[13], (int16_t)(status->humidity_rh * 100.0f));
    out[15] = status->fan_percent;
    out[16] = status->heater_on ? 1 : 0;
    out[17] = (uint8_t)status->completion_reason;
}

