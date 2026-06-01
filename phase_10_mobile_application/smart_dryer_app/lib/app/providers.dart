import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../domain/models/device_status.dart';
import '../domain/repositories/device_repository.dart';
import '../domain/repositories/local_device_repository.dart';
import '../services/local_api/local_api_client.dart';

final localApiBaseUriProvider = StateProvider<Uri>(
  (ref) => Uri.parse('http://192.168.4.1'),
);

final localApiClientProvider = Provider<LocalApiClient>((ref) {
  return LocalApiClient(baseUri: ref.watch(localApiBaseUriProvider));
});

final deviceRepositoryProvider = Provider<DeviceRepository>((ref) {
  return LocalDeviceRepository(ref.watch(localApiClientProvider));
});

final deviceStatusProvider = FutureProvider.autoDispose<DeviceStatus>((ref) {
  return ref.watch(deviceRepositoryProvider).getStatus();
});
