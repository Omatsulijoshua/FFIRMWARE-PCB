import '../../domain/models/ota_status.dart';
import '../../domain/repositories/device_repository.dart';

class OtaService {
  const OtaService(this._repository);

  final DeviceRepository _repository;

  Future<OtaStatus> status() => _repository.getOtaStatus();

  Future<void> start(String manifestUrl) => _repository.startOta(manifestUrl);
}
