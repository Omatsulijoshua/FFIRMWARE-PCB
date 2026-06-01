import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../../app/providers.dart';
import '../../domain/models/device_enums.dart';
import '../../shared/widgets/app_scaffold.dart';

class DeviceControlScreen extends ConsumerWidget {
  const DeviceControlScreen({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final repo = ref.watch(deviceRepositoryProvider);
    return AppScaffold(
      title: 'Control',
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          FilledButton.icon(
            onPressed: () async {
              await repo.startProgram(DryingProgram.shoes, 7200);
              ref.invalidate(deviceStatusProvider);
            },
            icon: const Icon(Icons.play_arrow),
            label: const Text('Start Shoes'),
          ),
          OutlinedButton.icon(
            onPressed: () async {
              await repo.stopCycle();
              ref.invalidate(deviceStatusProvider);
            },
            icon: const Icon(Icons.stop),
            label: const Text('Stop'),
          ),
          OutlinedButton.icon(
            onPressed: () async {
              await repo.clearFault();
              ref.invalidate(deviceStatusProvider);
            },
            icon: const Icon(Icons.restart_alt),
            label: const Text('Clear Fault'),
          ),
        ],
      ),
    );
  }
}
