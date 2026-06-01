import 'package:flutter/material.dart';

import '../../domain/models/device_enums.dart';
import '../../domain/models/program_profile.dart';
import '../../shared/utils/time_format.dart';
import '../../shared/widgets/app_scaffold.dart';

class ProgramsScreen extends StatelessWidget {
  const ProgramsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return AppScaffold(
      title: 'Programs',
      child: ListView.separated(
        itemBuilder: (context, index) {
          final profile = programProfiles[index];
          return ListTile(
            contentPadding: EdgeInsets.zero,
            title: Text(profile.program.label),
            subtitle: Text(
              '${profile.targetTempC.toStringAsFixed(0)} C, '
              '${profile.fanPercent}% fan, '
              '${formatDurationSeconds(profile.defaultDurationSeconds)}',
            ),
            trailing: const Icon(Icons.chevron_right),
          );
        },
        separatorBuilder: (_, __) => const Divider(),
        itemCount: programProfiles.length,
      ),
    );
  }
}
