import '../../services/local_api/local_api_client.dart';
import '../models/device_enums.dart';
import '../models/device_info.dart';
import '../models/device_status.dart';
import '../models/ota_status.dart';
import 'device_repository.dart';

class LocalDeviceRepository implements DeviceRepository {
  const LocalDeviceRepository(this._client);

  final LocalApiClient _client;

  @override
  Future<DeviceStatus> getStatus() => _client.getStatus();

  @override
  Future<DeviceInfo> getDeviceInfo() => _client.getDeviceInfo();

  @override
  Future<void> startProgram(DryingProgram program, int durationSeconds) {
    return _client.startProgram(program, durationSeconds);
  }

  @override
  Future<void> stopCycle() => _client.stopCycle();

  @override
  Future<void> clearFault() => _client.clearFault();

  @override
  Future<OtaStatus> getOtaStatus() => _client.getOtaStatus();

  @override
  Future<void> startOta(String manifestUrl) => _client.startOta(manifestUrl);
}
