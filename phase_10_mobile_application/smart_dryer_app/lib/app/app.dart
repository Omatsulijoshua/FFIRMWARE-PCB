import 'package:flutter/material.dart';

import 'app_routes.dart';
import 'app_theme.dart';

class SmartDryerApp extends StatelessWidget {
  const SmartDryerApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp.router(
      title: 'DryGuard',
      theme: buildAppTheme(),
      routerConfig: appRouter,
      debugShowCheckedModeBanner: false,
    );
  }
}
