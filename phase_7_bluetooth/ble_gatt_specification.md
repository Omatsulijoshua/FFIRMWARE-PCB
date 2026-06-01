# BLE GATT Specification

## UUID Namespace

Base namespace:

```text
7b6axxxx-5f4d-4f7a-9b6d-3b0c7d9f1000
```

The `xxxx` field identifies services and characteristics.

## Services

| Service | UUID | Purpose |
| --- | --- | --- |
| Device Information | `7b6a0001-5f4d-4f7a-9b6d-3b0c7d9f1000` | Identity, firmware, hardware, serial. |
| Control | `7b6a0100-5f4d-4f7a-9b6d-3b0c7d9f1000` | Start/stop cycles, fan, program selection. |
| Status | `7b6a0200-5f4d-4f7a-9b6d-3b0c7d9f1000` | Live status notifications and reads. |
| WiFi Provisioning | `7b6a0300-5f4d-4f7a-9b6d-3b0c7d9f1000` | Send SSID/password and connection command. |
| OTA | `7b6a0400-5f4d-4f7a-9b6d-3b0c7d9f1000` | Firmware update check/start/status. |
| Diagnostics | `7b6a0500-5f4d-4f7a-9b6d-3b0c7d9f1000` | Faults, sensor health, reset reason. |

## Device Information Characteristics

| Characteristic | UUID | Properties | Payload |
| --- | --- | --- | --- |
| Device ID | `7b6a0002-5f4d-4f7a-9b6d-3b0c7d9f1000` | Read | UTF-8 device ID. |
| Firmware Version | `7b6a0003-5f4d-4f7a-9b6d-3b0c7d9f1000` | Read | UTF-8 semver/build. |
| Hardware Revision | `7b6a0004-5f4d-4f7a-9b6d-3b0c7d9f1000` | Read | UTF-8 hardware revision. |
| Capabilities | `7b6a0005-5f4d-4f7a-9b6d-3b0c7d9f1000` | Read | Bitmask. |

## Control Characteristics

| Characteristic | UUID | Properties | Payload |
| --- | --- | --- | --- |
| Command | `7b6a0101-5f4d-4f7a-9b6d-3b0c7d9f1000` | Write, Write Without Response | `ble_control_command_t`. |
| Command Response | `7b6a0102-5f4d-4f7a-9b6d-3b0c7d9f1000` | Read, Notify | `ble_command_response_t`. |
| Custom Program | `7b6a0103-5f4d-4f7a-9b6d-3b0c7d9f1000` | Write | `ble_custom_program_t`. |

## Status Characteristics

| Characteristic | UUID | Properties | Payload |
| --- | --- | --- | --- |
| Live Status | `7b6a0201-5f4d-4f7a-9b6d-3b0c7d9f1000` | Read, Notify | `ble_status_payload_t`. |
| Sensor Status | `7b6a0202-5f4d-4f7a-9b6d-3b0c7d9f1000` | Read, Notify | Temperature/humidity snapshot. |

## WiFi Provisioning Characteristics

| Characteristic | UUID | Properties | Payload |
| --- | --- | --- | --- |
| WiFi Credentials | `7b6a0301-5f4d-4f7a-9b6d-3b0c7d9f1000` | Write | Encrypted or paired-session SSID/password payload. |
| WiFi Command | `7b6a0302-5f4d-4f7a-9b6d-3b0c7d9f1000` | Write | Connect, disconnect, forget. |
| WiFi Status | `7b6a0303-5f4d-4f7a-9b6d-3b0c7d9f1000` | Read, Notify | Connection state, RSSI, error code. |

## OTA Characteristics

| Characteristic | UUID | Properties | Payload |
| --- | --- | --- | --- |
| OTA Command | `7b6a0401-5f4d-4f7a-9b6d-3b0c7d9f1000` | Write | Check, start, cancel, recovery. |
| OTA Status | `7b6a0402-5f4d-4f7a-9b6d-3b0c7d9f1000` | Read, Notify | Version, progress, result. |

## Diagnostics Characteristics

| Characteristic | UUID | Properties | Payload |
| --- | --- | --- | --- |
| Fault Summary | `7b6a0501-5f4d-4f7a-9b6d-3b0c7d9f1000` | Read, Notify | Current fault, class, state. |
| Fault History | `7b6a0502-5f4d-4f7a-9b6d-3b0c7d9f1000` | Read | Recent fault records. |
| Factory Reset | `7b6a0503-5f4d-4f7a-9b6d-3b0c7d9f1000` | Write | Confirmation token. |

## Control Command Payload

```text
offset  size  field
0       1     command_id
1       1     program_id
2       4     duration_seconds, little-endian
6       1     fan_percent
7       2     target_temp_centi_c, little-endian signed
9       1     flags
```

Command IDs:

| ID | Command |
| --- | --- |
| `0x01` | Start program using `program_id` and optional duration. |
| `0x02` | Stop active cycle. |
| `0x03` | Clear fault. |
| `0x04` | Start custom program using fan/temp/flags. |
| `0x05` | Acknowledge complete. |

Flags:

| Bit | Meaning |
| --- | --- |
| 0 | Heater enabled. |
| 1 | Humidity auto-stop enabled. |

## Live Status Payload

```text
offset  size  field
0       1     state
1       1     program
2       1     fault
3       4     elapsed_seconds
7       4     remaining_seconds
11      2     outlet_temp_centi_c
13      2     humidity_centi_rh
15      1     fan_percent
16      1     heater_on
17      1     completion_reason
```

## BLE Limits

- Status notifications should be no faster than 1 Hz during active cycles.
- Command writes must be idempotent where practical.
- Payloads must fit within negotiated MTU; baseline payloads fit default ATT MTU.
- Long WiFi credentials may use prepared writes or chunking if needed.

