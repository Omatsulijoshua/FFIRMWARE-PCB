import 'device_enums.dart';

class ProgramProfile {
  const ProgramProfile({
    required this.program,
    required this.targetTempC,
    required this.fanPercent,
    required this.defaultDurationSeconds,
    required this.maxDurationSeconds,
    required this.humidityAutoStop,
  });

  final DryingProgram program;
  final double targetTempC;
  final int fanPercent;
  final int defaultDurationSeconds;
  final int maxDurationSeconds;
  final bool humidityAutoStop;
}

const programProfiles = [
  ProgramProfile(
    program: DryingProgram.shoes,
    targetTempC: 50,
    fanPercent: 80,
    defaultDurationSeconds: 7200,
    maxDurationSeconds: 14400,
    humidityAutoStop: true,
  ),
  ProgramProfile(
    program: DryingProgram.helmet,
    targetTempC: 42,
    fanPercent: 60,
    defaultDurationSeconds: 3600,
    maxDurationSeconds: 7200,
    humidityAutoStop: true,
  ),
  ProgramProfile(
    program: DryingProgram.gloves,
    targetTempC: 45,
    fanPercent: 70,
    defaultDurationSeconds: 5400,
    maxDurationSeconds: 10800,
    humidityAutoStop: true,
  ),
  ProgramProfile(
    program: DryingProgram.sportsGear,
    targetTempC: 55,
    fanPercent: 90,
    defaultDurationSeconds: 10800,
    maxDurationSeconds: 21600,
    humidityAutoStop: true,
  ),
  ProgramProfile(
    program: DryingProgram.technicalGear,
    targetTempC: 38,
    fanPercent: 50,
    defaultDurationSeconds: 5400,
    maxDurationSeconds: 14400,
    humidityAutoStop: true,
  ),
  ProgramProfile(
    program: DryingProgram.clothing,
    targetTempC: 45,
    fanPercent: 70,
    defaultDurationSeconds: 7200,
    maxDurationSeconds: 18000,
    humidityAutoStop: true,
  ),
  ProgramProfile(
    program: DryingProgram.hygiene,
    targetTempC: 62,
    fanPercent: 80,
    defaultDurationSeconds: 2700,
    maxDurationSeconds: 5400,
    humidityAutoStop: false,
  ),
  ProgramProfile(
    program: DryingProgram.fanOnly,
    targetTempC: 0,
    fanPercent: 70,
    defaultDurationSeconds: 3600,
    maxDurationSeconds: 28800,
    humidityAutoStop: false,
  ),
];
