import 'package:flutter/material.dart';

import '../../shared/widgets/app_scaffold.dart';

class TimerScreen extends StatelessWidget {
  const TimerScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return const AppScaffold(
      title: 'Timer',
      child: Center(child: Text('Timer controls')),
    );
  }
}
