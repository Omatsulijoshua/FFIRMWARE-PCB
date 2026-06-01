enum DeviceState {
  startup,
  idle,
  preheat,
  drying,
  hygiene,
  cooldown,
  complete,
  fault,
  unknown,
}

enum DryingProgram {
  none,
  shoes,
  helmet,
  gloves,
  sportsGear,
  technicalGear,
  clothing,
  hygiene,
  fanOnly,
}

enum FaultCode {
  none,
  sensorStale,
  tempSensorInvalid,
  humiditySensorInvalid,
  overtemp,
  fanFailure,
  watchdogReset,
  storage,
  ota,
  unknown,
}

enum CompletionReason {
  none,
  timer,
  humidityDry,
  userStop,
  fault,
  cooldownDone,
  unknown,
}

extension DryingProgramName on DryingProgram {
  String get apiName {
    switch (this) {
      case DryingProgram.shoes:
        return 'SHOES';
      case DryingProgram.helmet:
        return 'HELMET';
      case DryingProgram.gloves:
        return 'GLOVES';
      case DryingProgram.sportsGear:
        return 'SPORTS_GEAR';
      case DryingProgram.technicalGear:
        return 'TECHNICAL_GEAR';
      case DryingProgram.clothing:
        return 'CLOTHING';
      case DryingProgram.hygiene:
        return 'HYGIENE';
      case DryingProgram.fanOnly:
        return 'FAN_ONLY';
      case DryingProgram.none:
        return 'NONE';
    }
  }

  String get label {
    switch (this) {
      case DryingProgram.sportsGear:
        return 'Sports Gear';
      case DryingProgram.technicalGear:
        return 'Technical Gear';
      case DryingProgram.fanOnly:
        return 'Fan Only';
      case DryingProgram.none:
        return 'None';
      default:
        final name = apiName.toLowerCase().replaceAll('_', ' ');
        return name[0].toUpperCase() + name.substring(1);
    }
  }
}

DeviceState deviceStateFromApi(String value) {
  switch (value) {
    case 'STARTUP':
      return DeviceState.startup;
    case 'IDLE':
      return DeviceState.idle;
    case 'PREHEAT':
      return DeviceState.preheat;
    case 'DRYING':
      return DeviceState.drying;
    case 'HYGIENE':
      return DeviceState.hygiene;
    case 'COOLDOWN':
      return DeviceState.cooldown;
    case 'COMPLETE':
      return DeviceState.complete;
    case 'FAULT':
      return DeviceState.fault;
    default:
      return DeviceState.unknown;
  }
}

DryingProgram dryingProgramFromApi(String value) {
  for (final program in DryingProgram.values) {
    if (program.apiName == value) {
      return program;
    }
  }
  return DryingProgram.none;
}

FaultCode faultCodeFromApi(String value) {
  switch (value) {
    case 'NONE':
      return FaultCode.none;
    case 'SENSOR_STALE':
      return FaultCode.sensorStale;
    case 'TEMP_SENSOR_INVALID':
      return FaultCode.tempSensorInvalid;
    case 'HUMIDITY_SENSOR_INVALID':
      return FaultCode.humiditySensorInvalid;
    case 'OVERTEMP':
      return FaultCode.overtemp;
    case 'FAN_FAILURE':
      return FaultCode.fanFailure;
    case 'WATCHDOG_RESET':
      return FaultCode.watchdogReset;
    case 'STORAGE':
      return FaultCode.storage;
    case 'OTA':
      return FaultCode.ota;
    default:
      return FaultCode.unknown;
  }
}

CompletionReason completionReasonFromApi(String value) {
  switch (value) {
    case 'NONE':
      return CompletionReason.none;
    case 'TIMER':
      return CompletionReason.timer;
    case 'HUMIDITY_DRY':
      return CompletionReason.humidityDry;
    case 'USER_STOP':
      return CompletionReason.userStop;
    case 'FAULT':
      return CompletionReason.fault;
    case 'COOLDOWN_DONE':
      return CompletionReason.cooldownDone;
    default:
      return CompletionReason.unknown;
  }
}
