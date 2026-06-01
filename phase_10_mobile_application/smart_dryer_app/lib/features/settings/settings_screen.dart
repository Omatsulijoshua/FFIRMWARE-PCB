import 'package:flutter/material.dart';

import '../../shared/widgets/app_scaffold.dart';

class SettingsScreen extends StatelessWidget {
  const SettingsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return const AppScaffold(
      title: 'Settings',
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          ListTile(
            contentPadding: EdgeInsets.zero,
            leading: Icon(Icons.wifi),
            title: Text('WiFi'),
            subtitle: Text('Provision and manage local network'),
          ),
          ListTile(
            contentPadding: EdgeInsets.zero,
            leading: Icon(Icons.bluetooth),
            title: Text('Bluetooth'),
            subtitle: Text('Pairing and recovery'),
          ),
        ],
      ),
    );
  }
}
