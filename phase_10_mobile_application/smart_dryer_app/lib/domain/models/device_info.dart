class DeviceInfo {
  const DeviceInfo({
    required this.name,
    required this.firmwareVersion,
    required this.hardwareRevision,
    required this.wifiConnected,
    required this.wifiState,
    required this.rssi,
  });

  factory DeviceInfo.fromJson(Map<String, dynamic> json) {
    return DeviceInfo(
      name: json['name'] as String? ?? 'SDHS',
      firmwareVersion: json['firmwareVersion'] as String? ?? '',
      hardwareRevision: json['hardwareRevision'] as String? ?? '',
      wifiConnected: json['wifiConnected'] as bool? ?? false,
      wifiState: json['wifiState'] as String? ?? 'UNKNOWN',
      rssi: (json['rssi'] as num? ?? 0).toInt(),
    );
  }

  final String name;
  final String firmwareVersion;
  final String hardwareRevision;
  final bool wifiConnected;
  final String wifiState;
  final int rssi;
}
