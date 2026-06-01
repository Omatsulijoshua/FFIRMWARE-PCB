import 'package:flutter_secure_storage/flutter_secure_storage.dart';

class AppStorage {
  const AppStorage(this._storage);

  final FlutterSecureStorage _storage;

  Future<void> saveDeviceAddress(String address) {
    return _storage.write(key: 'device_address', value: address);
  }

  Future<String?> readDeviceAddress() {
    return _storage.read(key: 'device_address');
  }

  Future<void> saveLanBaseUrl(String baseUrl) {
    return _storage.write(key: 'lan_base_url', value: baseUrl);
  }

  Future<String?> readLanBaseUrl() {
    return _storage.read(key: 'lan_base_url');
  }

  Future<void> clear() {
    return _storage.deleteAll();
  }
}
