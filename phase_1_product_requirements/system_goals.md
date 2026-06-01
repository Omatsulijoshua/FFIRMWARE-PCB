# System Goals

## Primary Goal

Create a production-ready smart drying and hygiene appliance that safely dries gear and clothing accessories using controlled heat, managed airflow, humidity sensing, local intelligence, and mobile connectivity.

## Product Goals

| ID | Goal | Success Metric |
| --- | --- | --- |
| SG-001 | Dry common gear safely and predictably. | System completes selected cycles without exceeding program temperature limits. |
| SG-002 | Reduce user guesswork. | App and onboard logic recommend drying profile from item type and humidity behavior. |
| SG-003 | Protect users, gear, and property. | Heater shuts down under all defined fault conditions. |
| SG-004 | Work without internet. | User can control core functions locally over BLE and onboard controls. |
| SG-005 | Support long-term field maintenance. | Firmware can be updated with rollback and fault diagnostics. |
| SG-006 | Be manufacturable at consumer scale. | Design includes test points, end-of-line test flow, and certification-aware safety architecture. |
| SG-007 | Be approachable for non-technical users. | App presents clear programs, progress, and fault actions without exposing engineering complexity. |

## Experience Goals

- Starting a cycle should take less than 30 seconds after opening the app.
- The appliance should communicate status clearly through the app and minimal onboard indicators.
- A user should always understand whether the device is idle, running, cooling, complete, or in fault.
- Fault messages should explain what happened, what the device did to stay safe, and what the user should do next.
- The app should feel like a practical appliance controller, not a technology demo.

## Engineering Goals

- Use ESP32-S3 as the main controller.
- Use ESP-IDF for production firmware.
- Keep heater control, fan control, sensors, communications, OTA, safety, and diagnostics as separate firmware modules.
- Design the system so heater power is removed by default unless firmware and safety checks allow operation.
- Use layered safety: physical thermal cutoff, electronic sensing, firmware limits, watchdog, and fault latching.
- Support manufacturing programming, calibration, and functional test.
- Avoid mandatory cloud dependency in the first release.

## Performance Goals

| Area | Goal |
| --- | --- |
| Drying temperature | 35 C to 65 C program-controlled outlet air. |
| Hygiene temperature | 58 C to 65 C controlled outlet air, no sterilization claim without later validation. |
| Airflow | 10 CFM to 70+ CFM system range depending on fan and duct design. |
| Cycle length | 15 minutes to 8 hours depending on mode. |
| Sensor update cadence | Temperature and humidity processed at least every 5 seconds. |
| Control loop cadence | Heater and fan safety supervision at least every 1 second. |
| Fault reaction | Critical heater faults shut off heater immediately through firmware command and independent safety path. |
| OTA recovery | Device remains bootable after interrupted or failed update. |

## Business And Manufacturing Goals

- Keep preliminary BOM scalable toward a mainstream consumer retail price.
- Prefer components with reliable supply and credible alternates.
- Design for automated PCB assembly.
- Design for safe separation of AC mains and low-voltage electronics.
- Prepare documentation for CE, FCC, RoHS, and appliance safety review.
- Keep enclosure, airflow, heater, and firmware decisions aligned from the start.

## Local Smart Features Goal

The product shall implement useful smart behavior locally:

- Humidity-based auto stop.
- Estimated completion time.
- Adaptive drying profile changes.
- Item-type drying recommendations.
- Sensor fault degradation to conservative timed mode.

Cloud features may be added later, but the first production architecture must remain useful and safe without cloud AI.

