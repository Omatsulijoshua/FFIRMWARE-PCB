# Firmware Architecture

## Platform

- MCU module: ESP32-S3-WROOM-1-N16R8
- Framework: ESP-IDF
- Language: C
- RTOS: FreeRTOS through ESP-IDF
- Storage: NVS
- OTA: ESP-IDF OTA partition scheme with rollback support

## Module Structure

```text
firmware/
  CMakeLists.txt
  sdkconfig.defaults
  partitions.csv
  main/
    app_main.c
    app_config.h
  components/
    common/
    state_machine/
    sensor_manager/
    heater_manager/
    fan_manager/
    safety_manager/
    diagnostics_manager/
    storage_manager/
    ble_manager/
    wifi_manager/
    ota_manager/
```

## Runtime Tasks

| Task | Period | Responsibility |
| --- | --- | --- |
| `control_task` | 1 second | Read sensors, evaluate safety, update fan/heater, step state machine. |
| Connectivity tasks | Event-driven | BLE/WiFi command and status exchange. |
| OTA task | Event-driven | Download, verify, and stage firmware update. |

## Firmware Control Rule

Connectivity modules never directly drive heater or fan GPIO. They submit commands to the state machine. The safety manager is the authority for heater permission.

## Status

This phase provides production-grade scaffolding and baseline logic. Hardware-specific pin assignments, final sensor drivers, BLE GATT table details, and OTA server policy will be refined in later phases.

