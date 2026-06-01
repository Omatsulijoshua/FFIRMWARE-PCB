import '../models/device_enums.dart';
import '../models/device_info.dart';
import '../models/device_status.dart';
import '../models/ota_status.dart';

abstract class DeviceRepository {
  Future<DeviceStatus> getStatus();
  Future<DeviceInfo> getDeviceInfo();
  Future<void> startProgram(DryingProgram program, int durationSeconds);
  Future<void> stopCycle();
  Future<void> clearFault();
  Future<OtaStatus> getOtaStatus();
  Future<void> startOta(String manifestUrl);
}
