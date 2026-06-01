# WiFi Test Plan

## Unit Tests

| Test ID | Scenario | Expected Result |
| --- | --- | --- |
| WIFI-UT-001 | Save valid credentials. | Credentials are stored in NVS. |
| WIFI-UT-002 | Load missing credentials. | Returns not found and stays unprovisioned. |
| WIFI-UT-003 | Parse valid start-cycle JSON. | Produces state-machine start request. |
| WIFI-UT-004 | Parse unknown program. | Rejects with HTTP 400. |
| WIFI-UT-005 | Encode status JSON. | Fields match local API spec. |
| WIFI-UT-006 | OTA request during active cycle. | Rejected. |

## Integration Tests

- Provision WiFi over BLE.
- Reboot and reconnect using saved credentials.
- Disconnect router during active drying; cycle continues.
- Reconnect router; app recovers status over LAN.
- Start cycle over LAN.
- Stop cycle over LAN.
- Clear recoverable fault over LAN.
- Query status repeatedly during active cycle.
- Confirm BLE remains available when WiFi fails.

## Network Tests

- Wrong password.
- Hidden SSID if supported.
- Weak RSSI.
- Router reboot.
- DHCP lease renewal.
- App and device on same subnet.
- App and device on isolated guest network.

## Acceptance Criteria

- Core safety and cycle logic are independent of WiFi availability.
- LAN commands go through state-machine APIs.
- No LAN endpoint directly drives heater/fan GPIO.
- WiFi reconnect does not reset an active cycle.
- OTA transport is available only through approved manager flow.

