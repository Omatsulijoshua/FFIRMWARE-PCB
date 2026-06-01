import 'package:flutter_blue_plus/flutter_blue_plus.dart';

import '../../domain/models/device_enums.dart';
import '../../domain/models/device_status.dart';
import 'ble_protocol.dart';

class BleDeviceService {
  Stream<List<ScanResult>> scanForDevices() {
    FlutterBluePlus.startScan(
      withServices: [Guid(BleProtocol.serviceDeviceInfo)],
      timeout: const Duration(seconds: 8),
    );
    return FlutterBluePlus.scanResults;
  }

  Future<void> stopScan() => FlutterBluePlus.stopScan();

  Future<void> startProgram(
    BluetoothCharacteristic commandCharacteristic,
    DryingProgram program,
    int durationSeconds,
  ) {
    return commandCharacteristic.write(
      BleProtocol.startProgram(program, durationSeconds),
      withoutResponse: false,
    );
  }

  Future<void> stop(BluetoothCharacteristic commandCharacteristic) {
    return commandCharacteristic.write(
      BleProtocol.stop(),
      withoutResponse: false,
    );
  }

  Future<void> clearFault(BluetoothCharacteristic commandCharacteristic) {
    return commandCharacteristic.write(
      BleProtocol.clearFault(),
      withoutResponse: false,
    );
  }

  Stream<DeviceStatus> subscribeStatus(
    BluetoothCharacteristic statusCharacteristic,
  ) {
    statusCharacteristic.setNotifyValue(true);
    return statusCharacteristic.onValueReceived.map(
      BleProtocol.decodeStatus,
    );
  }
}
