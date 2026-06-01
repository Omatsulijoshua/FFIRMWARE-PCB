# Mobile Architecture

## Platform

- Framework: Flutter.
- Targets: Android and iOS.
- Architecture: Feature-first modules with shared domain services.
- State management: Riverpod, Bloc, or equivalent production-grade pattern to be selected in Phase 10. Initial recommendation is Riverpod for testability and clear dependency injection.
- Connectivity: BLE for onboarding/control and WiFi/LAN for local control and OTA workflow.

## Mobile App Architecture

```mermaid
flowchart TD
    UI["Flutter UI Screens"]
    State["App State / View Models"]
    Domain["Domain Layer\nDevices / Programs / Diagnostics"]
    BLE["BLE Service"]
    WiFi["LAN Device Service"]
    OTA["OTA Update Service"]
    Storage["Secure Storage + Local Cache"]
    Device["Smart Dryer"]

    UI --> State
    State --> Domain
    Domain --> BLE
    Domain --> WiFi
    Domain --> OTA
    Domain --> Storage
    BLE <--> Device
    WiFi <--> Device
    OTA <--> Device
```

## Mobile Feature Areas

| Feature | Responsibility |
| --- | --- |
| Splash | App boot, device cache loading, permission checks. |
| Onboarding | BLE discovery, pairing, device naming, WiFi provisioning. |
| Home | Device list and selected device status. |
| Device Control | Start/stop cycle, live status, fan, temperature, humidity. |
| Programs | Item-specific drying profiles and custom settings. |
| Timer | Manual duration selection. |
| Hygiene | Hygiene refresh workflow and safety language. |
| Settings | Device name, WiFi, units, factory reset, app preferences. |
| Diagnostics | Fault history, sensor status, firmware/hardware version. |
| Firmware Updates | Update availability, release notes, progress, rollback status. |

## Mobile Data Model

| Model | Key Fields |
| --- | --- |
| Device | id, name, BLE address/id, LAN address, firmware version, hardware revision. |
| DeviceStatus | state, program, elapsed, remaining estimate, temperature, humidity, fan level, heater status. |
| ProgramProfile | id, name, temperature band, fan range, max duration, auto-stop enabled. |
| FaultRecord | code, severity, description, timestamp, recommended action. |
| OtaInfo | current version, available version, required update flag, progress, rollback status. |

## BLE-First Onboarding Flow

```mermaid
sequenceDiagram
    participant User
    participant App
    participant Device

    User->>App: Add device
    App->>Device: Scan BLE advertisements
    Device-->>App: Provisioning advertisement
    User->>App: Select device
    App->>Device: Pair / authenticate
    App->>Device: Set friendly name
    App->>Device: Send WiFi credentials if requested
    Device-->>App: Provisioning result
    App-->>User: Device ready
```

## Runtime Control Flow

```mermaid
flowchart TD
    Home["Home"] --> Control["Device Control"]
    Control --> Program["Select Program"]
    Program --> Start["Start Cycle"]
    Start --> Monitor["Monitor Progress"]
    Monitor --> Complete["Complete / Cooldown Done"]
    Monitor --> Fault["Fault View"]
    Control --> Settings["Settings"]
    Settings --> Firmware["Firmware Updates"]
    Control --> Diagnostics["Diagnostics"]
```

## Mobile Safety Rules

- App cannot override firmware safety limits.
- App shall display cooldown as an active safety state.
- App shall block OTA update while a cycle is running.
- App shall present hygiene mode as hygiene refresh, not sterilization.
- App shall require explicit confirmation for factory reset.
- App shall show critical faults prominently with recommended actions.

## Permissions

| Platform | Permission Area |
| --- | --- |
| Android | Bluetooth scan/connect, nearby devices, location where OS requires it for BLE scan, local network if needed. |
| iOS | Bluetooth, local network access where applicable. |

## Mobile Project Structure

```text
mobile_app/
  lib/
    main.dart
    app/
      router.dart
      theme.dart
    features/
      onboarding/
      home/
      device_control/
      programs/
      timer/
      hygiene/
      settings/
      diagnostics/
      firmware_update/
    domain/
      models/
      repositories/
      use_cases/
    services/
      ble/
      lan/
      ota/
      storage/
    shared/
      widgets/
      utils/
  test/
  integration_test/
```

