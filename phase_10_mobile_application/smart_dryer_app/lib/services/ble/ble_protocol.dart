import 'dart:typed_data';

import '../../domain/models/device_enums.dart';
import '../../domain/models/device_status.dart';

class BleProtocol {
  static const serviceDeviceInfo = '7b6a0001-5f4d-4f7a-9b6d-3b0c7d9f1000';
  static const serviceControl = '7b6a0100-5f4d-4f7a-9b6d-3b0c7d9f1000';
  static const serviceStatus = '7b6a0200-5f4d-4f7a-9b6d-3b0c7d9f1000';
  static const serviceWifi = '7b6a0300-5f4d-4f7a-9b6d-3b0c7d9f1000';
  static const serviceOta = '7b6a0400-5f4d-4f7a-9b6d-3b0c7d9f1000';
  static const serviceDiagnostics = '7b6a0500-5f4d-4f7a-9b6d-3b0c7d9f1000';

  static Uint8List startProgram(DryingProgram program, int durationSeconds) {
    final data = ByteData(10);
    data.setUint8(0, 0x01);
    data.setUint8(1, _programId(program));
    data.setUint32(2, durationSeconds, Endian.little);
    data.setUint8(6, 0);
    data.setInt16(7, 0, Endian.little);
    data.setUint8(9, 0);
    return data.buffer.asUint8List();
  }

  static Uint8List stop() {
    final data = ByteData(10)..setUint8(0, 0x02);
    return data.buffer.asUint8List();
  }

  static Uint8List clearFault() {
    final data = ByteData(10)..setUint8(0, 0x03);
    return data.buffer.asUint8List();
  }

  static DeviceStatus decodeStatus(List<int> payload) {
    if (payload.length < 18) {
      return DeviceStatus.idle;
    }
    final data = ByteData.sublistView(Uint8List.fromList(payload));
    return DeviceStatus(
      state: _enumAt(DeviceState.values, data.getUint8(0), DeviceState.unknown),
      program:
          _enumAt(DryingProgram.values, data.getUint8(1), DryingProgram.none),
      fault: _enumAt(FaultCode.values, data.getUint8(2), FaultCode.unknown),
      elapsedSeconds: data.getUint32(3, Endian.little),
      remainingSeconds: data.getUint32(7, Endian.little),
      outletTempC: data.getInt16(11, Endian.little) / 100.0,
      humidityRh: data.getInt16(13, Endian.little) / 100.0,
      fanPercent: data.getUint8(15),
      heaterOn: data.getUint8(16) != 0,
      completionReason: _enumAt(
        CompletionReason.values,
        data.getUint8(17),
        CompletionReason.unknown,
      ),
    );
  }

  static int _programId(DryingProgram program) => program.index;

  static T _enumAt<T>(List<T> values, int index, T fallback) {
    if (index < 0 || index >= values.length) {
      return fallback;
    }
    return values[index];
  }
}
