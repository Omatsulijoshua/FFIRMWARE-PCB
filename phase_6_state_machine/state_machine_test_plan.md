# State Machine Test Plan

## Unit Tests

| Test ID | Scenario | Expected Result |
| --- | --- | --- |
| SM-UT-001 | Startup with valid sensors and no fault. | `STARTUP` transitions to `IDLE`. |
| SM-UT-002 | Startup with stale sensor. | Transitions to `FAULT`; heater off. |
| SM-UT-003 | Start shoes program from `IDLE`. | Transitions to `PREHEAT`; fan target set before heater request. |
| SM-UT-004 | Start fan-only program from `IDLE`. | Transitions to `DRYING`; heater remains off. |
| SM-UT-005 | Start hygiene from `IDLE`. | Transitions to `HYGIENE`; duration clamped to hygiene max. |
| SM-UT-006 | Temperature target reached in `PREHEAT`. | Transitions to `DRYING`. |
| SM-UT-007 | Humidity dry condition held for required ticks. | Transitions to `COOLDOWN`. |
| SM-UT-008 | Stop heated cycle. | Transitions to `COOLDOWN`; heater off. |
| SM-UT-009 | Stop fan-only cycle. | Transitions to `COMPLETE`; heater off. |
| SM-UT-010 | Cooldown temperature reached. | Transitions to `COMPLETE`; fan off. |
| SM-UT-011 | Overtemperature during active cycle. | Transitions to `FAULT`; heater off. |
| SM-UT-012 | Clear fault while sensor still invalid. | Remains in `FAULT`. |
| SM-UT-013 | Clear fault after self-test pass. | Transitions to `IDLE`. |

## Integration Tests

- BLE start command maps to a valid `sdhs_start_command_t`.
- WiFi start command follows the same state-machine path as BLE.
- OTA request is rejected during `PREHEAT`, `DRYING`, `HYGIENE`, and `COOLDOWN`.
- Diagnostics record state and sensor snapshot for fault transition.
- Factory test mode cannot run heater while normal state machine owns outputs.

## Hardware-In-The-Loop Tests

- Disconnect SHT41 during drying.
- Disconnect fan tach during drying.
- Block outlet airflow.
- Simulate heater-zone overtemperature.
- Power cycle during active cycle.
- Trigger watchdog reset during active heated cycle.
- Press stop during preheat.
- Press stop during hygiene.
- Complete cooldown with real thermal decay.

## Acceptance Criteria

- Heater GPIO is low in every non-heating state.
- Heater GPIO goes low within one control tick after any critical fault.
- No command path directly drives heater GPIO outside heater manager.
- State transitions are deterministic and logged.
- App-visible status matches internal state after each transition.

