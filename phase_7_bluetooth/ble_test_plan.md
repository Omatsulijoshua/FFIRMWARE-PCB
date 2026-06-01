# BLE Test Plan

## Unit Tests

| Test ID | Scenario | Expected Result |
| --- | --- | --- |
| BLE-UT-001 | Decode valid start command. | Produces `state_machine_start_program()` request. |
| BLE-UT-002 | Decode stop command. | Calls `state_machine_stop()`. |
| BLE-UT-003 | Decode clear fault command. | Calls `state_machine_clear_fault()`. |
| BLE-UT-004 | Decode malformed command length. | Rejects with invalid-length response. |
| BLE-UT-005 | Encode live status. | Payload matches GATT spec offsets. |
| BLE-UT-006 | Encode negative/invalid temperatures. | Uses signed centi-degree format safely. |

## Integration Tests

- Android app scans and discovers device.
- iOS app scans and discovers device.
- Pairing is required before privileged writes.
- Start Shoes program over BLE.
- Stop active cycle over BLE.
- Start Fan Only program over BLE.
- Start Hygiene program over BLE.
- Read live temperature and humidity.
- Receive 1 Hz status notifications during active cycle.
- Clear recoverable fault over BLE after safety condition clears.
- Factory reset clears pairing and WiFi credentials.

## Failure Tests

- Write command while unpaired.
- Write unknown command ID.
- Disconnect during active cycle.
- Reconnect during active cycle and recover status.
- Send repeated start commands.
- Send OTA command during active cycle.
- Send oversized WiFi credential payload.
- Attempt control from second phone without authorization.

## Acceptance Criteria

- BLE control never bypasses state-machine and safety-manager APIs.
- Heater state cannot be changed directly by BLE handler.
- App-visible status matches firmware state within 2 seconds.
- Device remains controllable over BLE with WiFi disabled.
- Bond/factory reset behavior is deterministic.

