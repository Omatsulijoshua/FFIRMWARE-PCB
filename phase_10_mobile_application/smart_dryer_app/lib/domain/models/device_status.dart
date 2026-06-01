import 'device_enums.dart';

class DeviceStatus {
  const DeviceStatus({
    required this.state,
    required this.program,
    required this.fault,
    required this.elapsedSeconds,
    required this.remainingSeconds,
    required this.outletTempC,
    required this.humidityRh,
    required this.fanPercent,
    required this.heaterOn,
    required this.completionReason,
  });

  factory DeviceStatus.fromJson(Map<String, dynamic> json) {
    return DeviceStatus(
      state: deviceStateFromApi(json['state'] as String? ?? ''),
      program: dryingProgramFromApi(json['program'] as String? ?? ''),
      fault: faultCodeFromApi(json['fault'] as String? ?? ''),
      elapsedSeconds: (json['elapsedSeconds'] as num? ?? 0).toInt(),
      remainingSeconds: (json['remainingSeconds'] as num? ?? 0).toInt(),
      outletTempC: (json['outletTempC'] as num? ?? 0).toDouble(),
      humidityRh: (json['humidityRh'] as num? ?? 0).toDouble(),
      fanPercent: (json['fanPercent'] as num? ?? 0).toInt(),
      heaterOn: json['heaterOn'] as bool? ?? false,
      completionReason: completionReasonFromApi(
        json['completionReason'] as String? ?? '',
      ),
    );
  }

  final DeviceState state;
  final DryingProgram program;
  final FaultCode fault;
  final int elapsedSeconds;
  final int remainingSeconds;
  final double outletTempC;
  final double humidityRh;
  final int fanPercent;
  final bool heaterOn;
  final CompletionReason completionReason;

  static const idle = DeviceStatus(
    state: DeviceState.idle,
    program: DryingProgram.none,
    fault: FaultCode.none,
    elapsedSeconds: 0,
    remainingSeconds: 0,
    outletTempC: 0,
    humidityRh: 0,
    fanPercent: 0,
    heaterOn: false,
    completionReason: CompletionReason.none,
  );
}
