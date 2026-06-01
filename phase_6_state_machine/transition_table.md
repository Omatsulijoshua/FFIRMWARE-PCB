# Transition Table

## Events

| Event | Description |
| --- | --- |
| `EV_SELF_TEST_PASS` | Boot self-test and safety checks pass. |
| `EV_SELF_TEST_FAIL` | Boot self-test or safety checks fail. |
| `EV_START_HEATED` | User starts a heated drying profile. |
| `EV_START_FAN_ONLY` | User starts fan-only drying. |
| `EV_START_HYGIENE` | User starts hygiene refresh. |
| `EV_TARGET_REACHED` | Outlet temperature reaches program target band. |
| `EV_TIMER_DONE` | Program duration expires. |
| `EV_HUMIDITY_DRY` | Humidity dry condition is held for required ticks. |
| `EV_STOP` | User stops the active cycle. |
| `EV_COOLDOWN_DONE` | Outlet temperature is safe or cooldown timeout expires. |
| `EV_ACK_COMPLETE` | User acknowledges complete state or timeout expires. |
| `EV_FAULT` | Safety fault occurs. |
| `EV_CLEAR_FAULT` | User requests fault clear and self-test passes. |

## Transition Matrix

| Current State | Event | Next State | Actions |
| --- | --- | --- | --- |
| `STARTUP` | `EV_SELF_TEST_PASS` | `IDLE` | Outputs off. |
| `STARTUP` | `EV_SELF_TEST_FAIL` | `FAULT` | Outputs off, record fault. |
| `IDLE` | `EV_START_HEATED` | `PREHEAT` | Start fan, heater requested after safety permission. |
| `IDLE` | `EV_START_FAN_ONLY` | `DRYING` | Start fan, heater disabled. |
| `IDLE` | `EV_START_HYGIENE` | `HYGIENE` | Start fan, request hygiene target. |
| `PREHEAT` | `EV_TARGET_REACHED` | `DRYING` | Continue fan and heater regulation. |
| `PREHEAT` | `EV_TIMER_DONE` | `COOLDOWN` | Heater off, fan cooldown. |
| `PREHEAT` | `EV_STOP` | `COOLDOWN` | Heater off, fan cooldown. |
| `PREHEAT` | `EV_FAULT` | `FAULT` | Heater off, record fault. |
| `DRYING` | `EV_TIMER_DONE` | `COOLDOWN` | Heater off, fan cooldown. |
| `DRYING` | `EV_HUMIDITY_DRY` | `COOLDOWN` | Heater off, fan cooldown, completion reason dry. |
| `DRYING` | `EV_STOP` | `COOLDOWN` or `COMPLETE` | Cool down if heater was enabled; otherwise complete. |
| `DRYING` | `EV_FAULT` | `FAULT` | Heater off, record fault. |
| `HYGIENE` | `EV_TIMER_DONE` | `COOLDOWN` | Heater off, fan cooldown. |
| `HYGIENE` | `EV_STOP` | `COOLDOWN` | Heater off, fan cooldown. |
| `HYGIENE` | `EV_FAULT` | `FAULT` | Heater off, record fault. |
| `COOLDOWN` | `EV_COOLDOWN_DONE` | `COMPLETE` | Fan off, heater off. |
| `COOLDOWN` | `EV_FAULT` | `FAULT` | Heater off, record fault. |
| `COMPLETE` | `EV_ACK_COMPLETE` | `IDLE` | Clear active program context. |
| `COMPLETE` | `EV_START_HEATED` | `PREHEAT` | Start new cycle. |
| `COMPLETE` | `EV_START_FAN_ONLY` | `DRYING` | Start new cycle. |
| `COMPLETE` | `EV_START_HYGIENE` | `HYGIENE` | Start new cycle. |
| `FAULT` | `EV_CLEAR_FAULT` | `IDLE` | Clear fault only after self-test pass. |

## Completion Reasons

| Reason | Meaning |
| --- | --- |
| `NONE` | No completion reason. |
| `TIMER` | Program duration expired. |
| `HUMIDITY_DRY` | Humidity dry threshold was held. |
| `USER_STOP` | User stopped cycle. |
| `FAULT` | Cycle terminated because of fault. |
| `COOLDOWN_DONE` | Cooldown completed after previous stop reason. |

