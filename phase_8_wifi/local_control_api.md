# Local Control API

## Transport

- Protocol: HTTP over LAN.
- Encoding: JSON.
- Version prefix: `/api/v1`.
- Default port: `80` for first firmware baseline; production may use HTTPS or signed requests where feasible.

## Device Status

### `GET /api/v1/status`

Response:

```json
{
  "state": "DRYING",
  "program": "SHOES",
  "fault": "NONE",
  "elapsedSeconds": 600,
  "remainingSeconds": 6600,
  "outletTempC": 48.5,
  "humidityRh": 42.2,
  "fanPercent": 80,
  "heaterOn": true,
  "completionReason": "NONE"
}
```

## Start Cycle

### `POST /api/v1/cycle/start`

Request:

```json
{
  "program": "SHOES",
  "durationSeconds": 7200
}
```

Response:

```json
{
  "ok": true
}
```

## Stop Cycle

### `POST /api/v1/cycle/stop`

Response:

```json
{
  "ok": true
}
```

## Clear Fault

### `POST /api/v1/fault/clear`

Response:

```json
{
  "ok": true
}
```

## Device Info

### `GET /api/v1/device`

Response:

```json
{
  "name": "SDHS",
  "firmwareVersion": "0.1.0",
  "hardwareRevision": "HW_MAIN_REV_A",
  "wifiConnected": true
}
```

## OTA

### `GET /api/v1/ota/status`

Response:

```json
{
  "state": "idle",
  "currentVersion": "0.1.0",
  "progress": 0
}
```

### `POST /api/v1/ota/start`

Request:

```json
{
  "url": "https://updates.example.com/sdhs/manifest.json"
}
```

Response:

```json
{
  "ok": true
}
```

## Program Names

Accepted program strings:

- `SHOES`
- `HELMET`
- `GLOVES`
- `SPORTS_GEAR`
- `TECHNICAL_GEAR`
- `CLOTHING`
- `HYGIENE`
- `FAN_ONLY`

## Safety Rules

- The API cannot force heater on.
- `POST /api/v1/ota/start` is rejected unless the device is idle or complete.
- Unknown programs are rejected.
- Commands return failure if the state machine rejects them.
