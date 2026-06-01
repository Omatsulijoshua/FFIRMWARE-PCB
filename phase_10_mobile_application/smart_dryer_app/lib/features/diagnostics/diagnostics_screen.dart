import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../../app/providers.dart';
import '../../shared/widgets/app_scaffold.dart';

class DiagnosticsScreen extends ConsumerWidget {
  const DiagnosticsScreen({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final status = ref.watch(deviceStatusProvider);
    return AppScaffold(
      title: 'Diagnostics',
      child: status.when(
        data: (value) => Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text('Fault: ${value.fault.name}'),
            Text('Fan: ${value.fanPercent}%'),
            Text('Heater: ${value.heaterOn ? 'on' : 'off'}'),
          ],
        ),
        loading: () => const Center(child: CircularProgressIndicator()),
        error: (error, stackTrace) => Text('Diagnostics unavailable: $error'),
      ),
    );
  }
}
