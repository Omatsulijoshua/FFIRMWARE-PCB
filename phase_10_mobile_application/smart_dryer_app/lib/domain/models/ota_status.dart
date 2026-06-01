class OtaStatus {
  const OtaStatus({
    required this.state,
    required this.error,
    required this.currentVersion,
    required this.targetVersion,
    required this.progress,
  });

  factory OtaStatus.fromJson(Map<String, dynamic> json) {
    return OtaStatus(
      state: json['state'] as String? ?? 'idle',
      error: json['error'] as String? ?? 'none',
      currentVersion: json['currentVersion'] as String? ?? '',
      targetVersion: json['targetVersion'] as String? ?? '',
      progress: (json['progress'] as num? ?? 0).toInt(),
    );
  }

  final String state;
  final String error;
  final String currentVersion;
  final String targetVersion;
  final int progress;
}
