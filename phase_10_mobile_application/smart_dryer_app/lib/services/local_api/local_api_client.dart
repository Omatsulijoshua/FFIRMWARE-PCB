import 'dart:convert';

import 'package:http/http.dart' as http;

import '../../domain/models/device_info.dart';
import '../../domain/models/device_status.dart';
import '../../domain/models/ota_status.dart';
import '../../domain/models/device_enums.dart';

class LocalApiClient {
  LocalApiClient({
    required this.baseUri,
    http.Client? httpClient,
  }) : _httpClient = httpClient ?? http.Client();

  final Uri baseUri;
  final http.Client _httpClient;

  Uri _uri(String path) => baseUri.replace(path: path);

  Future<DeviceStatus> getStatus() async {
    final response = await _httpClient.get(_uri('/api/v1/status'));
    _ensureOk(response);
    return DeviceStatus.fromJson(
      jsonDecode(response.body) as Map<String, dynamic>,
    );
  }

  Future<DeviceInfo> getDeviceInfo() async {
    final response = await _httpClient.get(_uri('/api/v1/device'));
    _ensureOk(response);
    return DeviceInfo.fromJson(
      jsonDecode(response.body) as Map<String, dynamic>,
    );
  }

  Future<void> startProgram(DryingProgram program, int durationSeconds) async {
    final response = await _httpClient.post(
      _uri('/api/v1/cycle/start'),
      headers: {'content-type': 'application/json'},
      body: jsonEncode({
        'program': program.apiName,
        'durationSeconds': durationSeconds,
      }),
    );
    _ensureOk(response);
  }

  Future<void> stopCycle() async {
    final response = await _httpClient.post(_uri('/api/v1/cycle/stop'));
    _ensureOk(response);
  }

  Future<void> clearFault() async {
    final response = await _httpClient.post(_uri('/api/v1/fault/clear'));
    _ensureOk(response);
  }

  Future<OtaStatus> getOtaStatus() async {
    final response = await _httpClient.get(_uri('/api/v1/ota/status'));
    _ensureOk(response);
    return OtaStatus.fromJson(
      jsonDecode(response.body) as Map<String, dynamic>,
    );
  }

  Future<void> startOta(String manifestUrl) async {
    final response = await _httpClient.post(
      _uri('/api/v1/ota/start'),
      headers: {'content-type': 'application/json'},
      body: jsonEncode({'url': manifestUrl}),
    );
    _ensureOk(response);
  }

  void _ensureOk(http.Response response) {
    if (response.statusCode < 200 || response.statusCode >= 300) {
      throw LocalApiException(response.statusCode, response.body);
    }
  }
}

class LocalApiException implements Exception {
  const LocalApiException(this.statusCode, this.body);

  final int statusCode;
  final String body;

  @override
  String toString() => 'LocalApiException($statusCode): $body';
}
