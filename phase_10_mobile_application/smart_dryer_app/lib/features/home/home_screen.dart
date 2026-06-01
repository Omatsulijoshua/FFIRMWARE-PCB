import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';

import '../../app/providers.dart';
import '../../domain/models/device_enums.dart';
import '../../shared/widgets/brand_logo.dart';
import '../../shared/widgets/device_preview_card.dart';
import '../../shared/widgets/app_scaffold.dart';

class HomeScreen extends ConsumerWidget {
  const HomeScreen({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final status = ref.watch(deviceStatusProvider);
    return AppScaffold(
      title: 'Smart Dryer',
      actions: [
        IconButton(
          tooltip: 'Settings',
          onPressed: () => context.go('/settings'),
          icon: const Icon(Icons.settings),
        ),
      ],
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          const Row(
            children: [
              BrandLogo(size: 48, showShadow: false),
              SizedBox(width: 12),
              Expanded(
                child: Text(
                  'DryGuard',
                  style: TextStyle(fontSize: 26, fontWeight: FontWeight.w800),
                ),
              ),
            ],
          ),
          const SizedBox(height: 16),
          const Center(
            child: SizedBox(
              width: 360,
              child: DevicePreviewCard(),
            ),
          ),
          const SizedBox(height: 16),
          status.when(
            data: (value) => Card(
              child: Padding(
                padding: const EdgeInsets.all(16),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Text('State: ${value.state.name}'),
                    Text('Program: ${value.program.label}'),
                    Text(
                      'Temperature: ${value.outletTempC.toStringAsFixed(1)} C',
                    ),
                    Text('Humidity: ${value.humidityRh.toStringAsFixed(1)} %'),
                  ],
                ),
              ),
            ),
            loading: () => const Center(child: CircularProgressIndicator()),
            error: (error, stackTrace) => Text('Device unavailable: $error'),
          ),
          const SizedBox(height: 16),
          FilledButton.icon(
            onPressed: () => context.go('/control'),
            icon: const Icon(Icons.tune),
            label: const Text('Control Device'),
          ),
          OutlinedButton.icon(
            onPressed: () => context.go('/programs'),
            icon: const Icon(Icons.inventory_2),
            label: const Text('Programs'),
          ),
          OutlinedButton.icon(
            onPressed: () => context.go('/diagnostics'),
            icon: const Icon(Icons.monitor_heart),
            label: const Text('Diagnostics'),
          ),
        ],
      ),
    );
  }
}
