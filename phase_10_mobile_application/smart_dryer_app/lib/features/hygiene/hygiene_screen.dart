import 'package:flutter/material.dart';

import '../../shared/widgets/app_scaffold.dart';

class HygieneScreen extends StatelessWidget {
  const HygieneScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return const AppScaffold(
      title: 'Hygiene',
      child: Text('Hygiene refresh controls'),
    );
  }
}
