# OTA Test Plan

## Unit Tests

| Test ID | Scenario | Expected Result |
| --- | --- | --- |
| OTA-UT-001 | Valid manifest. | Parsed and accepted. |
| OTA-UT-002 | Product mismatch. | Rejected. |
| OTA-UT-003 | Hardware mismatch. | Rejected. |
| OTA-UT-004 | Invalid SHA-256 length. | Rejected. |
| OTA-UT-005 | OTA requested during drying. | Rejected. |
| OTA-UT-006 | OTA requested while WiFi disconnected. | Rejected. |
| OTA-UT-007 | Same version without override. | Rejected or ignored. |

## Integration Tests

- Download manifest over HTTPS.
- Download valid image and reboot into new firmware.
- Mark new firmware valid after self-test.
- Interrupt power during download.
- Interrupt power after staging before reboot.
- Simulate self-test failure in new image and verify rollback.
- Verify BLE recovery after failed WiFi connection.
- Verify local API reports OTA status.

## Hardware Tests

- OTA while appliance is plugged into real AC but idle.
- OTA blocked during `PREHEAT`.
- OTA blocked during `DRYING`.
- OTA blocked during `HYGIENE`.
- OTA blocked during `COOLDOWN`.
- Heater GPIO remains off during OTA.

## Acceptance Criteria

- Failed OTA cannot brick the device.
- Previous valid firmware remains bootable.
- New firmware is marked valid only after self-test.
- OTA cannot run during active heating or cooldown.
- OTA progress and result are visible to app through BLE or LAN.

