import 'package:go_router/go_router.dart';

import '../features/device_control/device_control_screen.dart';
import '../features/diagnostics/diagnostics_screen.dart';
import '../features/firmware_update/firmware_update_screen.dart';
import '../features/hygiene/hygiene_screen.dart';
import '../features/home/home_screen.dart';
import '../features/onboarding/onboarding_screen.dart';
import '../features/programs/programs_screen.dart';
import '../features/settings/settings_screen.dart';
import '../features/splash/splash_screen.dart';
import '../features/timer/timer_screen.dart';

final appRouter = GoRouter(
  initialLocation: '/',
  routes: [
    GoRoute(
      path: '/',
      builder: (context, state) => const SplashScreen(),
    ),
    GoRoute(
      path: '/onboarding',
      builder: (context, state) => const OnboardingScreen(),
    ),
    GoRoute(
      path: '/home',
      builder: (context, state) => const HomeScreen(),
    ),
    GoRoute(
      path: '/control',
      builder: (context, state) => const DeviceControlScreen(),
    ),
    GoRoute(
      path: '/programs',
      builder: (context, state) => const ProgramsScreen(),
    ),
    GoRoute(
      path: '/timer',
      builder: (context, state) => const TimerScreen(),
    ),
    GoRoute(
      path: '/hygiene',
      builder: (context, state) => const HygieneScreen(),
    ),
    GoRoute(
      path: '/settings',
      builder: (context, state) => const SettingsScreen(),
    ),
    GoRoute(
      path: '/diagnostics',
      builder: (context, state) => const DiagnosticsScreen(),
    ),
    GoRoute(
      path: '/firmware',
      builder: (context, state) => const FirmwareUpdateScreen(),
    ),
  ],
);
